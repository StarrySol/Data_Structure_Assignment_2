#include <exception>
#include <iostream>
#include <string>

#include "CSVReader.h"
#include "Simplifier.h"

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input.csv> <target_vertices>\n";
        return 1;
    }

    try
    {
        std::string inputFile = argv[1];
        std::size_t targetVertices = static_cast<std::size_t>(std::stoull(argv[2]));

        Polygon polygon = CSVReader::ReadPolygon(inputFile);
        SimplifyResult result = Simplifier::SimplifyPolygon(polygon, targetVertices);

        Simplifier::WriteOutput(std::cout, result);
    }
    catch (std::exception const& ex)
    {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}