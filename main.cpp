#include <iostream>

#include "Code/Vertice.h"

int main(int argc, char* argv[])
{
    // Check correct number of arguments
    if (argc != 3)
    {
        std::cerr << "Usage: ./simplify <input_file.csv> <target_vertices>\n";
        return 1;
    }

    std::string inputFile = argv[1];
    int targetVertices = std::stoi(argv[2]);

    std::cout << "ring_id,vertex_id,x,y" << std::endl;

    return 0;
}