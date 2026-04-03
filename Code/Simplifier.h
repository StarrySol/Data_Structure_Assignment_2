
#pragma once
#include <cstddef>
#include <iosfwd>
#include "Polygon.h"

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
    static Ring SimplifyRing(Ring const& ring,
                             std::size_t targetVertices,
                             double& accumulatedArealDisplacement);

    static Vec2 ComputeCollapsePoint(Vec2 const& A,
                                     Vec2 const& B,
                                     Vec2 const& C,
                                     Vec2 const& D);

    static double ComputeCollapseDisplacement(Vec2 const& A,
                                             Vec2 const& B,
                                             Vec2 const& C,
                                             Vec2 const& D,
                                             Vec2 const& E);

    static bool IsCollapseValid(Ring const& ring,
                                std::size_t i,
                                Vec2 const& E);

    static bool SegmentIntersectsRing(Vec2 const& p1,
                                      Vec2 const& q1,
                                      Ring const& ring,
                                      std::size_t skipEdgeStart1,
                                      std::size_t skipEdgeStart2);
};