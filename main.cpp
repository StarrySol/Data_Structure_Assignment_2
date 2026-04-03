/******************************************************************************
 * File:        main.cpp
 * Project:     Data Structures Assigment 2
 *
 * Description:
 * This file contains main() to run a program that simplifies a polygon from a 
 * input file to a desired vertice number using ASPC
 * to run do ./simplify <input_file.csv> <target_vertices>
 ******************************************************************************/

/*
 * ============================================================
 * main.cpp  --  polygon simplification entry point
 * ============================================================
 *
 * USAGE
 * -----
 *   ./simplify <input_file.csv> <target_vertices>
 *
 * INPUT
 * -----
 * A CSV file with columns: ring_id, vertex_id, x, y
 * Ring 0 is the exterior boundary (CCW); rings 1+ are holes (CW).
 *
 * OUTPUT (to stdout)
 * ------------------
 * The simplified polygon in the same CSV format, followed by:
 *   Total signed area in input:  <value>
 *   Total signed area in output: <value>
 *   Total areal displacement:    <value>
 *
 * ALGORITHM
 * ---------
 * Area-Preserving Segment Collapse (APSC), Kronenfeld et al. 2020.
 * Each collapse replaces four consecutive vertices A->B->C->D with
 * three vertices A->E->D, where E is chosen to preserve the ring's
 * signed area exactly while minimising areal displacement.
 *
 * A global min-heap selects the cheapest collapse at each step.
 * Topology (no self-intersections, no ring-ring crossings) is checked
 * before every collapse.
 * ============================================================
 */

#include <iostream>
#include <fstream>
#include <vector>

#include "Code/FileReader.h"
#include "Code/Collaspe.h"

int main(int argc, char* argv[])
{
    //Parse inputs
    if (argc != 3)
    {
        std::cerr << "Usage: ./simplify <input_file.csv> <target_vertices>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    int targetVertices = std::stoi(argv[2]);

    //Open file
    std::ifstream file(inputFile, std::ios::in);
    if (!file.is_open())
    {
        std::cerr << "Unable to open: " << inputFile << std::endl;
        return 1;
    }


    //Load file into vector of rings
    std::vector<Ring> ringVec = LoadFile(file);
    file.close();


    //APSC simplification.
    //Collapses are applied until the total vertex count reaches targetVertices (or no valid collapse).
    SimplifyAll(ringVec, targetVertices);


    //Print result
    OutputRings(ringVec);

    //Free all pointers in double link list
    FreeRings(ringVec);

    return 0;
}
