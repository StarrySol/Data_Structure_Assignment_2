#include <iostream>
#include <fstream>
#include <vector>

#include "Code/FileReader.h"

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

    std::ifstream file;
    file.open(inputFile, std::ios::in);
    if (!file.is_open())
    {
        std::cerr << "Unable to open: " << inputFile <<std::endl;
        return 1;
    }

    //Parse data
    std::vector<Ring> ringVec = LoadFile(file);
    file.close();


    OutputVertices(ringVec);

    FreeRings(ringVec);

    return 0;
}