#pragma once


#include "Vertice.h"
#include <vector>

struct Shape
{
    int shapeID = 0;
    std::vector<Vertice> vertices{};

    Shape();
};