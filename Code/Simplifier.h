#pragma once

#include <cstddef>
#include <iosfwd>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>
#include "Polygon.h"

struct VertexNode
{
    Vec2 p;
    VertexNode* prev{};
    VertexNode* next{};
    unsigned int ring_id{};
    std::size_t id{};
    std::size_t version{};
    bool alive{true};
};

struct RingState
{
    unsigned int ring_id{};
    VertexNode* head{};
    std::size_t vertex_count{};
    bool clockwise{};
};

struct EdgeRef
{
    VertexNode* a{};
    VertexNode* b{};
    unsigned int ring_id{};
};

struct CollapseCandidate
{
    VertexNode* A{};
    VertexNode* B{};
    VertexNode* C{};
    VertexNode* D{};
    Vec2 E{};
    double displacement{};
    std::size_t b_version{};
    std::size_t c_version{};
    bool valid{true};
};

struct CandidateCompare
{
    bool operator()(CollapseCandidate const& lhs,
                    CollapseCandidate const& rhs) const
    {
        if (!NearlyEqual(lhs.displacement, rhs.displacement))
        {
            return lhs.displacement > rhs.displacement;
        }

        // deterministic tie-breaker
        return lhs.B->id > rhs.B->id;
    }
};

class SpatialHash
{
public:
    explicit SpatialHash(double cellSize = 100.0);

    void InsertEdge(EdgeRef const& edge);
    void RemoveEdge(EdgeRef const& edge);
    std::vector<EdgeRef> QuerySegment(Vec2 const& p, Vec2 const& q) const;

private:
    double mCellSize{100.0};
    std::unordered_map<long long, std::vector<EdgeRef>> mCells;

    long long CellKey(int x, int y) const;
};

struct SimplifyResult
{
    Polygon polygon;
    double inputSignedArea{};
    double outputSignedArea{};
    double areaDisplacement{};
};

class Simplifier
{
public:
    static SimplifyResult SimplifyPolygon(Polygon const& polygon,
                                          std::size_t targetTotalVertices);

    static void WriteOutput(std::ostream& os, SimplifyResult const& result);

private:
    static std::vector<RingState> BuildRingStates(
        Polygon const& polygon,
        std::vector<std::unique_ptr<VertexNode>>& storage);

    static CollapseCandidate MakeCandidate(VertexNode* A,
                                           VertexNode* B,
                                           VertexNode* C,
                                           VertexNode* D);

    static Vec2 ComputeCollapsePoint(Vec2 const& A,
                                     Vec2 const& B,
                                     Vec2 const& C,
                                     Vec2 const& D);

    static double ComputeCollapseDisplacement(Vec2 const& A,
                                              Vec2 const& B,
                                              Vec2 const& C,
                                              Vec2 const& D,
                                              Vec2 const& E);

    static bool IsCandidateStale(CollapseCandidate const& cand);

    static bool IsCollapseValid(CollapseCandidate const& cand,
                                SpatialHash const& index);

    static void ApplyCollapse(CollapseCandidate const& cand,
                              RingState& ring,
                              SpatialHash& index,
                              double& totalDisplacement);

    static void PushLocalCandidates(
        VertexNode* center,
        std::priority_queue<CollapseCandidate,
                            std::vector<CollapseCandidate>,
                            CandidateCompare>& pq);

    static Polygon BuildOutputPolygon(std::vector<RingState> const& rings);
};