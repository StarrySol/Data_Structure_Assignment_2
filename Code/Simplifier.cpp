#include "Simplifier.h"

#include <algorithm>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <vector>

namespace
{
    // double RingSignedArea(std::vector<Vec2> const& vertices)
    // {
    //     if (vertices.size() < 3)
    //     {
    //         return 0.0;
    //     }

    //     double area = 0.0;
    //     for (std::size_t i = 0; i < vertices.size(); ++i)
    //     {
    //         Vec2 const& a = vertices[i];
    //         Vec2 const& b = vertices[(i + 1) % vertices.size()];
    //         area += (a.x * b.y) - (b.x * a.y);
    //     }

    //     return 0.5 * area;
    // }

    std::size_t PrevIndex(std::size_t i, std::size_t n)
    {
        return (i + n - 1) % n;
    }

    std::size_t NextIndex(std::size_t i, std::size_t n)
    {
        return (i + 1) % n;
    }
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

    std::size_t currentTotalVertices = 0;
    for (std::size_t i = 0; i < polygon.RingCount(); ++i)
    {
        currentTotalVertices += polygon.GetRing(i).VertexCount();
    }

    if (targetTotalVertices >= currentTotalVertices)
    {
        result.polygon = polygon;
        result.outputSignedArea = polygon.TotalSignedArea();
        result.areaDisplacement = 0.0;
        return result;
    }

    std::vector<Ring> workingRings;
    workingRings.reserve(polygon.RingCount());

    for (std::size_t i = 0; i < polygon.RingCount(); ++i)
    {
        workingRings.push_back(polygon.GetRing(i));
    }

    result.areaDisplacement = 0.0;

    while (currentTotalVertices > targetTotalVertices)
    {
        double bestCost = std::numeric_limits<double>::max();
        std::size_t bestRingIndex = std::numeric_limits<std::size_t>::max();
        Ring bestReplacement;

        for (std::size_t ringIndex = 0; ringIndex < workingRings.size(); ++ringIndex)
        {
            Ring const& ring = workingRings[ringIndex];

            if (ring.VertexCount() <= 3)
            {
                continue;
            }

            double localDisplacement = 0.0;
            Ring simplified = SimplifyRing(ring, ring.VertexCount() - 1, localDisplacement);

            if (simplified.VertexCount() < ring.VertexCount() && localDisplacement < bestCost)
            {
                bestCost = localDisplacement;
                bestRingIndex = ringIndex;
                bestReplacement = simplified;
            }
        }

        if (bestRingIndex == std::numeric_limits<std::size_t>::max())
        {
            break;
        }

        currentTotalVertices -= 1;
        result.areaDisplacement += bestCost;
        workingRings[bestRingIndex] = bestReplacement;
    }

    for (Ring const& ring : workingRings)
    {
        result.polygon.AddRing(ring);
    }

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

Ring Simplifier::SimplifyRing(Ring const& ring,
                              std::size_t targetVertices,
                              double& accumulatedareaDisplacement)
{
    accumulatedareaDisplacement = 0.0;

    if (ring.VertexCount() <= 3 || targetVertices >= ring.VertexCount())
    {
        return ring;
    }

    std::vector<Vec2> vertices = ring.GetVertices();

    while (vertices.size() > targetVertices && vertices.size() > 3)
    {
        double bestCost = std::numeric_limits<double>::max();
        std::size_t bestB = std::numeric_limits<std::size_t>::max();
        Vec2 bestE{};

        for (std::size_t b = 0; b < vertices.size(); ++b)
        {
            std::size_t a = PrevIndex(b, vertices.size());
            std::size_t c = NextIndex(b, vertices.size());
            std::size_t d = NextIndex(c, vertices.size());

            Vec2 const& A = vertices[a];
            Vec2 const& B = vertices[b];
            Vec2 const& C = vertices[c];
            Vec2 const& D = vertices[d];

            Vec2 E = ComputeCollapsePoint(A, B, C, D);

            Ring temp(ring.GetRingID());
            for (std::size_t i = 0; i < vertices.size(); ++i)
            {
                if (i == b)
                {
                    temp.AddVertex(E);
                }
                else if (i == c)
                {
                    continue;
                }
                else
                {
                    temp.AddVertex(vertices[i]);
                }
            }

            if (!IsCollapseValid(temp, 0, E))
            {
                continue;
            }

            double cost = ComputeCollapseDisplacement(A, B, C, D, E);
            if (cost < bestCost)
            {
                bestCost = cost;
                bestB = b;
                bestE = E;
            }
        }

        if (bestB == std::numeric_limits<std::size_t>::max())
        {
            break;
        }

        std::size_t bestC = NextIndex(bestB, vertices.size());

        vertices[bestB] = bestE;
        vertices.erase(vertices.begin() + static_cast<std::ptrdiff_t>(bestC));
        accumulatedareaDisplacement += bestCost;
    }

    Ring result(ring.GetRingID());
    for (Vec2 const& v : vertices)
    {
        result.AddVertex(v);
    }

    return result;
}

Vec2 Simplifier::ComputeCollapsePoint(Vec2 const& A,
                                      Vec2 const& B,
                                      Vec2 const& C,
                                      Vec2 const& D)
{
    double a = D.y - A.y;
    double b = A.x - D.x;
    double c = -B.y * A.x + (A.y - C.y) * B.x + (B.y - D.y) * C.x + C.y * D.x;

    auto chooseAB = [&]() -> Vec2
    {
        double valueA = a * A.x + b * A.y + c;
        double valueB = a * B.x + b * B.y + c;

        if (NearlyEqual(valueA, valueB))
        {
            return B;
        }

        double t = -valueA / (valueB - valueA);
        return A + t * (B - A);
    };

    auto chooseCD = [&]() -> Vec2
    {
        double valueC = a * C.x + b * C.y + c;
        double valueD = a * D.x + b * D.y + c;

        if (NearlyEqual(valueC, valueD))
        {
            return C;
        }

        double t = -valueC / (valueD - valueC);
        return C + t * (D - C);
    };

    int sideB = SideOfDirectedLine(A, D, B);
    int sideC = SideOfDirectedLine(A, D, C);

    if (sideB == sideC)
    {
        double distB = DistancePointToLine(A, D, B);
        double distC = DistancePointToLine(A, D, C);

        if (distB >= distC)
        {
            return chooseAB();
        }

        return chooseCD();
    }

    double testX;
    double testY;
    if (!NearlyEqual(a, 0.0))
    {
        testX = -c / a;
        testY = 0.0;
    }
    else
    {
        testX = 0.0;
        testY = -c / b;
    }

    Vec2 linePoint(testX, testY);
    int sideELine = SideOfDirectedLine(A, D, linePoint);

    if (sideB == sideELine)
    {
        return chooseAB();
    }

    return chooseCD();
}

double Simplifier::ComputeCollapseDisplacement(Vec2 const& A,
                                               Vec2 const& B,
                                               Vec2 const& C,
                                               Vec2 const& D,
                                               Vec2 const& E)
{
    double oldArea = std::abs(SignedTriangleArea(A, B, C)) +
                     std::abs(SignedTriangleArea(A, C, D));

    double newArea = std::abs(SignedTriangleArea(A, E, D));

    return std::abs(oldArea - newArea) +
           TriangleArea(A, B, E) +
           TriangleArea(E, C, D);
}

bool Simplifier::IsCollapseValid(Ring const& ring,
                                 std::size_t,
                                 Vec2 const&)
{
    std::size_t n = ring.VertexCount();
    if (n < 3)
    {
        return false;
    }

    for (std::size_t i = 0; i < n; ++i)
    {
        Vec2 const& a1 = ring.GetVertex(i);
        Vec2 const& a2 = ring.GetVertex((i + 1) % n);

        for (std::size_t j = i + 1; j < n; ++j)
        {
            if (j == i || (j + 1) % n == i || (i + 1) % n == j)
            {
                continue;
            }

            Vec2 const& b1 = ring.GetVertex(j);
            Vec2 const& b2 = ring.GetVertex((j + 1) % n);

            if (SegmentsIntersect(a1, a2, b1, b2))
            {
                return false;
            }
        }
    }

    return true;
}

bool Simplifier::SegmentIntersectsRing(Vec2 const& p1,
                                       Vec2 const& q1,
                                       Ring const& ring,
                                       std::size_t skipEdgeStart1,
                                       std::size_t skipEdgeStart2)
{
    std::size_t n = ring.VertexCount();

    for (std::size_t i = 0; i < n; ++i)
    {
        if (i == skipEdgeStart1 || i == skipEdgeStart2)
        {
            continue;
        }

        Vec2 const& p2 = ring.GetVertex(i);
        Vec2 const& q2 = ring.GetVertex((i + 1) % n);

        if (SegmentsIntersect(p1, q1, p2, q2))
        {
            return true;
        }
    }

    return false;
}