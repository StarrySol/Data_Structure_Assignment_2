#include <iostream>
#include <iomanip>
#include <exception>
#include "CSVReader.h"
#include "Polygon.h"

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input.csv>\n";
        return 1;
    }

    try
    {
        Polygon polygon = CSVReader::ReadPolygon(argv[1]);

        std::cout << "Polygon loaded successfully.\n";
        std::cout << "Ring count: " << polygon.RingCount() << "\n\n";

        for (std::size_t i = 0; i < polygon.RingCount(); ++i)
        {
            Ring const& ring = polygon.GetRings()[i];

            std::cout << "Ring " << i << "\n";
            std::cout << "  ring_id      : " << ring.GetRingID() << "\n";
            std::cout << "  vertex count : " << ring.VertexCount() << "\n";
            std::cout << "  is empty     : " << (ring.IsEmpty() ? "true" : "false") << "\n";
            std::cout << "  is closed    : " << (ring.IsClosed() ? "true" : "false") << "\n";
            std::cout << "  signed area  : " << ring.SignedArea() << "\n";
            std::cout << "  area         : " << ring.Area() << "\n";
            std::cout << "  clockwise    : " << (ring.IsClockwise() ? "true" : "false") << "\n";

            std::cout << "  vertices:\n";
            for (Vec2 const& v : ring.GetVertices())
            {
                std::cout << "    (" << v.x << ", " << v.y << ")\n";
            }

            std::cout << "\n";
        }
    }
    catch (std::exception const& ex)
    {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}