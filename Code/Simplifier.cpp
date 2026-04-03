#include "Simplifier.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <unordered_set>

namespace
{
    using CandidateQueue = std::priority_queue<
        CollapseCandidate,
        std::vector<CollapseCandidate>,
        CandidateCompare>;

    int CellCoord(double value, double cellSize)
    {
        return static_cast<int>(std::floor(value / cellSize));
    }

    bool SameUndirectedEdge(EdgeRef const& edge,
                            VertexNode const* u,
                            VertexNode const* v)
    {
        return (edge.a == u && edge.b == v) || (edge.a == v && edge.b == u);
    }

    // bool EdgeTouchesAny(EdgeRef const& edge,
    //                     VertexNode const* a,
    //                     VertexNode const* b,
    //                     VertexNode const* c,
    //                     VertexNode const* d)
    // {
    //     return edge.a == a || edge.a == b || edge.a == c || edge.a == d ||
    //            edge.b == a || edge.b == b || edge.b == c || edge.b == d;
    // }

    void GatherCellsForSegment(Vec2 const& p,
                               Vec2 const& q,
                               double cellSize,
                               std::vector<long long>& keys,
                               auto const& keyMaker)
    {
        keys.clear();

        double minX = std::min(p.x, q.x);
        double maxX = std::max(p.x, q.x);
        double minY = std::min(p.y, q.y);
        double maxY = std::max(p.y, q.y);

        int x0 = CellCoord(minX, cellSize);
        int x1 = CellCoord(maxX, cellSize);
        int y0 = CellCoord(minY, cellSize);
        int y1 = CellCoord(maxY, cellSize);

        for (int x = x0; x <= x1; ++x)
        {
            for (int y = y0; y <= y1; ++y)
            {
                keys.push_back(keyMaker(x, y));
            }
        }
    }
}

SpatialHash::SpatialHash(double cellSize)
    : mCellSize(cellSize)
{
}

long long SpatialHash::CellKey(int x, int y) const
{
    return (static_cast<long long>(x) << 32) ^
           static_cast<unsigned int>(y);
}

void SpatialHash::InsertEdge(EdgeRef const& edge)
{
    if (edge.a == nullptr || edge.b == nullptr)
    {
        return;
    }

    std::vector<long long> keys;
    GatherCellsForSegment(edge.a->p, edge.b->p, mCellSize, keys,
        [this](int x, int y) { return CellKey(x, y); });

    for (long long key : keys)
    {
        mCells[key].push_back(edge);
    }
}

void SpatialHash::RemoveEdge(EdgeRef const& edge)
{
    if (edge.a == nullptr || edge.b == nullptr)
    {
        return;
    }

    std::vector<long long> keys;
    GatherCellsForSegment(edge.a->p, edge.b->p, mCellSize, keys,
        [this](int x, int y) { return CellKey(x, y); });

    for (long long key : keys)
    {
        auto it = mCells.find(key);
        if (it == mCells.end())
        {
            continue;
        }

        auto& bucket = it->second;
        bucket.erase(
            std::remove_if(bucket.begin(), bucket.end(),
                [&](EdgeRef const& e)
                {
                    return SameUndirectedEdge(e, edge.a, edge.b);
                }),
            bucket.end());

        if (bucket.empty())
        {
            mCells.erase(it);
        }
    }
}

std::vector<EdgeRef> SpatialHash::QuerySegment(Vec2 const& p, Vec2 const& q) const
{
    std::vector<EdgeRef> result;
    std::unordered_set<std::uintptr_t> seen;

    std::vector<long long> keys;
    GatherCellsForSegment(p, q, mCellSize, keys,
        [this](int x, int y) { return CellKey(x, y); });

    for (long long key : keys)
    {
        auto it = mCells.find(key);
        if (it == mCells.end())
        {
            continue;
        }

        for (EdgeRef const& edge : it->second)
        {
            std::uintptr_t aPtr = reinterpret_cast<std::uintptr_t>(edge.a);
            std::uintptr_t bPtr = reinterpret_cast<std::uintptr_t>(edge.b);
            if (aPtr > bPtr)
            {
                std::swap(aPtr, bPtr);
            }

            std::uintptr_t hash = aPtr ^ (bPtr << 1);
            if (seen.insert(hash).second)
            {
                result.push_back(edge);
            }
        }
    }

    return result;
}

std::vector<RingState> Simplifier::BuildRingStates(
    Polygon const& polygon,
    std::vector<std::unique_ptr<VertexNode>>& storage)
{
    std::vector<RingState> states;
    states.reserve(polygon.RingCount());

    std::size_t nextId = 0;

    for (std::size_t r = 0; r < polygon.RingCount(); ++r)
    {
        Ring const& ring = polygon.GetRing(r);

        RingState state{};
        state.ring_id = ring.GetRingID();
        state.vertex_count = ring.VertexCount();
        state.clockwise = ring.IsClockwise();

        if (ring.VertexCount() == 0)
        {
            states.push_back(state);
            continue;
        }

        VertexNode* first = nullptr;
        VertexNode* prev = nullptr;

        for (std::size_t i = 0; i < ring.VertexCount(); ++i)
        {
            storage.push_back(std::make_unique<VertexNode>());
            VertexNode* node = storage.back().get();

            node->p = ring.GetVertex(i);
            node->ring_id = ring.GetRingID();
            node->id = nextId++;
            node->version = 0;
            node->alive = true;

            if (first == nullptr)
            {
                first = node;
            }

            if (prev != nullptr)
            {
                prev->next = node;
                node->prev = prev;
            }

            prev = node;
        }

        first->prev = prev;
        prev->next = first;

        state.head = first;
        states.push_back(state);
    }

    return states;
}

CollapseCandidate Simplifier::MakeCandidate(VertexNode* A,
                                            VertexNode* B,
                                            VertexNode* C,
                                            VertexNode* D)
{
    CollapseCandidate cand{};
    cand.A = A;
    cand.B = B;
    cand.C = C;
    cand.D = D;
    cand.E = ComputeCollapsePoint(A->p, B->p, C->p, D->p);
    cand.displacement = ComputeCollapseDisplacement(A->p, B->p, C->p, D->p, cand.E);
    cand.b_version = B->version;
    cand.c_version = C->version;
    cand.valid = true;
    return cand;
}

Vec2 Simplifier::ComputeCollapsePoint(Vec2 const& A,
                                      Vec2 const& B,
                                      Vec2 const& C,
                                      Vec2 const& D)
{
    // Equation (1b) in Kronenfeld
// a*xE + b*yE + c = 0
    double a = D.y - A.y;
    double b = A.x - D.x;
    double c = -B.y * A.x + (A.y - C.y) * B.x + (B.y - D.y) * C.x + C.y * D.x;

    auto lineValue = [&](Vec2 const& p) -> double
    {
        return a * p.x + b * p.y + c;
    };

    // Build one concrete line E↔ using a point on the line and direction parallel to AD
    Vec2 e0;
    if (!NearlyEqual(a, 0.0))
    {
        e0 = Vec2(-c / a, 0.0);
    }
    else if (!NearlyEqual(b, 0.0))
    {
        e0 = Vec2(0.0, -c / b);
    }
    else
    {
        // Degenerate AD, safest fallback
        return B;
    }

    Vec2 e1 = e0 + (D - A);

    auto intersectAB = [&]() -> Vec2
    {
        // Singular case 6(b): B,C,D collinear -> optimal placement at B
        if (NearlyEqual(cross(B, C, D), 0.0))
        {
            return B;
        }

        double t = 0.0;
        double u = 0.0;
        if (!LineIntersectionParam(A, B, e0, e1, t, u))
        {
            return B;
        }
        return A + t * (B - A);
    };

    auto intersectCD = [&]() -> Vec2
    {
        // Singular case 6(b): B,C,D collinear -> optimal placement at B
        if (NearlyEqual(cross(B, C, D), 0.0))
        {
            return B;
        }

        double t = 0.0;
        double u = 0.0;
        if (!LineIntersectionParam(C, D, e0, e1, t, u))
        {
            return C;
        }
        return C + t * (D - C);
    };

    // Singular case 6(a): E↔ coincident with AD
    // Then any point between A and D is optimal; paper notes the rule yields A or D.
    if (NearlyEqual(lineValue(A), 0.0) && NearlyEqual(lineValue(D), 0.0))
    {
        return A;
    }

    int sideB = SideOfDirectedLine(A, D, B);
    int sideC = SideOfDirectedLine(A, D, C);
    int sideELine = SideOfDirectedLine(A, D, e0);

    double distB = DistancePointToLine(A, D, B);
    double distC = DistancePointToLine(A, D, C);

    // Paper pseudocode, page 11
    if (sideB == sideC)
    {
        if (distB > distC)
        {
            return intersectAB();
        }
        else if (distB < distC)
        {
            return intersectCD();
        }
        else
        {
            // Singular case 6(c): BC parallel to AD, any point between the
            // intersections with AB and CD is optimal.
            // Use AB deterministically.
            return intersectAB();
        }
    }
    else
    {
        if (sideB == sideELine)
        {
            return intersectAB();
        }
        else
        {
            return intersectCD();
        }
    }
}

double Simplifier::ComputeCollapseDisplacement(Vec2 const& A,
                                               Vec2 const& B,
                                               Vec2 const& C,
                                               Vec2 const& D,
                                               Vec2 const& E)
{
    // Areal displacement = total area enclosed by ABCD and AED.
    // Use the closed boundary A -> B -> C -> D -> E -> A.
    //
    // Crucially, use UNSIGNED area. The old implementation let signed
    // cancellation drive some collapses to zero incorrectly.

    // Use absolute shoelace area of that boundary.

    Vec2 const poly[5] = {A, B, C, D, E};

    double twiceArea = 0.0;
    for (int i = 0; i < 5; ++i)
    {
        Vec2 const& p = poly[i];
        Vec2 const& q = poly[(i + 1) % 5];
        twiceArea += p.x * q.y - q.x * p.y;
    }

    return std::abs(0.5 * twiceArea);
}

bool Simplifier::IsCandidateStale(CollapseCandidate const& cand)
{
    if (!cand.valid ||
        cand.A == nullptr || cand.B == nullptr || cand.C == nullptr || cand.D == nullptr)
    {
        return true;
    }

    if (!cand.A->alive || !cand.B->alive || !cand.C->alive || !cand.D->alive)
    {
        return true;
    }

    if (cand.B->version != cand.b_version || cand.C->version != cand.c_version)
    {
        return true;
    }

    if (cand.A->next != cand.B || cand.B->next != cand.C || cand.C->next != cand.D)
    {
        return true;
    }

    if (cand.B->prev != cand.A || cand.C->prev != cand.B || cand.D->prev != cand.C)
    {
        return true;
    }

    return false;
}

bool Simplifier::IsCollapseValid(CollapseCandidate const& cand,
                                 SpatialHash const& index)
{
    Vec2 const& A = cand.A->p;
    Vec2 const& E = cand.E;
    Vec2 const& D = cand.D->p;

    auto isExcludedLocalEdge = [&](EdgeRef const& edge) -> bool
    {
        return SameUndirectedEdge(edge, cand.A, cand.B) ||
               SameUndirectedEdge(edge, cand.B, cand.C) ||
               SameUndirectedEdge(edge, cand.C, cand.D);
    };

    // Check AE against every existing edge except AB, BC, CD
    std::vector<EdgeRef> hitsAE = index.QuerySegment(A, E);
    for (EdgeRef const& edge : hitsAE)
    {
        if (isExcludedLocalEdge(edge))
        {
            continue;
        }

        // Allow touching only at A
        bool sharesA = (edge.a == cand.A || edge.b == cand.A);

        if (SegmentsIntersect(A, E, edge.a->p, edge.b->p))
        {
            if (!sharesA)
            {
                return false;
            }
        }
    }

    // Check ED against every existing edge except AB, BC, CD
    std::vector<EdgeRef> hitsED = index.QuerySegment(E, D);
    for (EdgeRef const& edge : hitsED)
    {
        if (isExcludedLocalEdge(edge))
        {
            continue;
        }

        // Allow touching only at D
        bool sharesD = (edge.a == cand.D || edge.b == cand.D);

        if (SegmentsIntersect(E, D, edge.a->p, edge.b->p))
        {
            if (!sharesD)
            {
                return false;
            }
        }
    }

    return true;
}

void Simplifier::ApplyCollapse(CollapseCandidate const& cand,
                               RingState& ring,
                               SpatialHash& index,
                               double& totalDisplacement)
{
    index.RemoveEdge({cand.A, cand.B, cand.A->ring_id});
    index.RemoveEdge({cand.B, cand.C, cand.B->ring_id});
    index.RemoveEdge({cand.C, cand.D, cand.C->ring_id});

    // B survives and becomes E
    cand.B->p = cand.E;
    cand.B->prev = cand.A;
    cand.B->next = cand.D;
    cand.B->version += 1;

    cand.A->next = cand.B;
    cand.A->version += 1;

    cand.D->prev = cand.B;
    cand.D->version += 1;

    cand.C->alive = false;
    cand.C->version += 1;

    ring.vertex_count -= 1;

    if (ring.head == cand.C || ring.head == nullptr)
    {
        ring.head = cand.B;
    }

    index.InsertEdge({cand.A, cand.B, cand.A->ring_id});
    index.InsertEdge({cand.B, cand.D, cand.B->ring_id});

    totalDisplacement += cand.displacement;
}

void Simplifier::PushLocalCandidates(
    VertexNode* center,
    std::priority_queue<CollapseCandidate,
                        std::vector<CollapseCandidate>,
                        CandidateCompare>& pq)
{
if (center == nullptr || !center->alive)
    {
        return;
    }

    // After collapsing BC -> E, the paper says:
    // remove all 4-vertex sequences involving B or C
    // add all 4-vertex sequences involving E
    //
    // So regenerate all windows in the local neighborhood around center.
    VertexNode* starts[5] =
    {
        center->prev != nullptr ? center->prev->prev : nullptr,
        center->prev,
        center,
        center->next,
        (center->next != nullptr) ? center->next->next : nullptr
    };

    for (VertexNode* A : starts)
    {
        if (A == nullptr || !A->alive)
        {
            continue;
        }

        VertexNode* B = A->next;
        VertexNode* C = (B != nullptr) ? B->next : nullptr;
        VertexNode* D = (C != nullptr) ? C->next : nullptr;

        if (B == nullptr || C == nullptr || D == nullptr)
        {
            continue;
        }

        if (!B->alive || !C->alive || !D->alive)
        {
            continue;
        }

        if (A == B || B == C || C == D || D == A || A == C || B == D)
        {
            continue;
        }

        pq.push(MakeCandidate(A, B, C, D));
    }
}

Polygon Simplifier::BuildOutputPolygon(std::vector<RingState> const& rings)
{
    Polygon polygon;

    for (RingState const& ringState : rings)
    {
        Ring ring(ringState.ring_id);

        if (ringState.head == nullptr || ringState.vertex_count == 0)
        {
            polygon.AddRing(ring);
            continue;
        }

        VertexNode* current = ringState.head;
        std::size_t visited = 0;

        do
        {
            if (current->alive)
            {
                ring.AddVertex(current->p);
                ++visited;
            }
            current = current->next;
        }
        while (current != ringState.head && visited < ringState.vertex_count);

        polygon.AddRing(ring);
    }

    return polygon;
}

SimplifyResult Simplifier::SimplifyPolygon(Polygon const& polygon,
                                           std::size_t targetTotalVertices)
{
    SimplifyResult result{};
    result.inputSignedArea = polygon.TotalSignedArea();

    if (polygon.IsEmpty())
    {
        result.outputSignedArea = 0.0;
        result.areaDisplacement = 0.0;
        return result;
    }

    std::vector<std::unique_ptr<VertexNode>> storage;
    std::vector<RingState> rings = BuildRingStates(polygon, storage);

    SpatialHash index(100.0);
    for (RingState const& ring : rings)
    {
        if (ring.head == nullptr || ring.vertex_count < 2)
        {
            continue;
        }

        VertexNode* cur = ring.head;
        std::size_t visited = 0;
        do
        {
            index.InsertEdge({cur, cur->next, ring.ring_id});
            cur = cur->next;
            ++visited;
        }
        while (cur != ring.head && visited < ring.vertex_count);
    }

    CandidateQueue pq;
    for (RingState const& ring : rings)
    {
        if (ring.head == nullptr || ring.vertex_count < 4)
        {
            continue;
        }

        VertexNode* cur = ring.head;
        std::size_t visited = 0;
        do
        {
            PushLocalCandidates(cur, pq);
            cur = cur->next;
            ++visited;
        }
        while (cur != ring.head && visited < ring.vertex_count);
    }

    std::size_t currentTotalVertices = polygon.VertexCount();
    result.areaDisplacement = 0.0;

    while (currentTotalVertices > targetTotalVertices && !pq.empty())
    {
        CollapseCandidate cand = pq.top();
        pq.pop();

        if (IsCandidateStale(cand))
        {
            continue;
        }

        if (!IsCollapseValid(cand, index))
        {
            continue;
        }

        RingState* ringPtr = nullptr;
        for (RingState& ring : rings)
        {
            if (ring.ring_id == cand.B->ring_id)
            {
                ringPtr = &ring;
                break;
            }
        }

        if (ringPtr == nullptr)
        {
            throw std::runtime_error("Internal error: ring id not found.");
        }

        ApplyCollapse(cand, *ringPtr, index, result.areaDisplacement);
        --currentTotalVertices;

        PushLocalCandidates(cand.A->prev, pq);
        PushLocalCandidates(cand.A, pq);
        PushLocalCandidates(cand.B, pq);
        PushLocalCandidates(cand.D, pq);
        PushLocalCandidates(cand.D->next, pq);
    }

    result.polygon = BuildOutputPolygon(rings);
    result.outputSignedArea = result.polygon.TotalSignedArea();
    return result;
}

void Simplifier::WriteOutput(std::ostream& os, SimplifyResult const& result)
{
    os << "ring_id,vertex_id,x,y\n";

    for (std::size_t r = 0; r < result.polygon.RingCount(); ++r)
    {
        Ring const& ring = result.polygon.GetRing(r);

        for (std::size_t v = 0; v < ring.VertexCount(); ++v)
        {
            Vec2 const& p = ring.GetVertex(v);
            os << r << "," << v << "," << p.x << "," << p.y << "\n";
        }
    }

    os << std::scientific << std::setprecision(6);
    os << "Total signed area in input: " << result.inputSignedArea << "\n";
    os << "Total signed area in output: " << result.outputSignedArea << "\n";
    os << "Total areal displacement: " << result.areaDisplacement << "\n";
}