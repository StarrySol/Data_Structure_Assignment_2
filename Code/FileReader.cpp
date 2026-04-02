#include "FileReader.h"

#include <sstream>
#include <string>
#include <iostream>

std::vector<Ring> LoadFile(std::ifstream& inputFile)
{
    std::vector<Ring> ringVec{};

    //Check if ifstream is open
    if (!inputFile.is_open())
    {
        std::cerr << "Unable to open file" <<std::endl;
        return ringVec;
    }

    std::string line;

    //Skip header line
    std::getline(inputFile, line);

    int maxRingID = -1;

    //Get line until EOF
    while (std::getline(inputFile, line))
    {
        std::stringstream ss(line); //Put them into stringstream for easier parsing
        std::string token;

        int ring_id;
        int vertex_id;
        double x, y;

        //Parse values split by ,
        std::getline(ss, token, ',');
        ring_id = std::stoi(token);

        std::getline(ss, token, ',');
        vertex_id = std::stoi(token);

        std::getline(ss, token, ',');
        x = std::stod(token);

        std::getline(ss, token, ',');
        y = std::stod(token);

        
        //Insert new ring if list dosnt have that ring number
        while (ring_id > maxRingID)
        {
            maxRingID++;

            Ring newRing;
            newRing.ringID = maxRingID;

            ringVec.push_back(newRing);

        }
        
        // Create vertex
        Vertice v;
        v.ring_id = ring_id;
        v.vertex_id = vertex_id;
        v.pos = Vec2(x, y);

        //Add new vertex into list
        ringVec[ring_id].vertices.push_back(v);
    }

    return ringVec;
}

void OutputVertices(std::vector<Ring>& ringVec)
{
    std::cout << "ring_id,vertex_id,x,y" << std::endl;

    for (Ring& ring : ringVec)
    {
        for (Vertice& vertice : ring.vertices)
        {
            std::cout << vertice.ring_id << "," << vertice.vertex_id << "," << vertice.pos.x << "," << vertice.pos.y <<std::endl;
        } 
    }

    std::cout << std::endl; //Additional newline
}