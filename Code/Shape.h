#include "Vertice.h"
#include <list>

struct Shape
{
    int shapeID = 0;
    std::list<Vertice> vertices{};

    Shape();
};