#include <iostream>
#include <fstream>

#include "Code/Shape.h"

int main(int argc, char* argv[])
{
    // Check correct number of arguments
    if (argc != 3)
    {
        std::cerr << "Usage: ./simplify <input_file.csv> <target_vertices>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    int targetVertices = std::stoi(argv[2]);

    (void)targetVertices;

    std::ifsstream file;
    file.open(inputFile, std::ios::in);
    if (!file.is_open())
    {
        std::cerr << "Unable to open: " << inputFile <<std::endl;
        return 1;
    }

    //Parse data
    



    std::cout << "ring_id,vertex_id,x,y" << std::endl;

    return 0;
}