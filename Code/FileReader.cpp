/******************************************************************************
 * File:        FileReader.cpp
 * Project:     Data Structures Assigment 2
 *
 * Description:
 * This file contains the definitions of functions to input and output data for rings
 * It also converts vparsed vertices into a doubled circular linked list

 * Functions:
 * LoadFile            -- parse CSV -> vector of Ring objects
 * ConvertToLinkedList -- convert parsed vertices -> circular lists
 * OutputRings         -- print simplified rings to stdout
 * FreeRings           -- release all heap-allocated Node memory
 ******************************************************************************/

#include "FileReader.h"
#include "Collaspe.h"

#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>

std::vector<Ring> LoadFile(std::ifstream& inputFile)
{
    std::vector<Ring> ringVec;

    if (!inputFile.is_open())
    {
        std::cerr << "LoadFile: file is not open." << std::endl;
        return ringVec;
    }

    std::string line;

    //Skip the header line ("ring_id,vertex_id,x,y")
    std::getline(inputFile, line);

    while (std::getline(inputFile, line))
    {
        //Use a stringstream to split on commas
        std::stringstream ss(line);
        std::string token;

        int ring_id, vertex_id;
        double x, y;

        std::getline(ss, token, ',');
        ring_id = std::stoi(token);
        
        std::getline(ss, token, ',');
        vertex_id = std::stoi(token);
        
        std::getline(ss, token, ',');
        x = std::stod(token);
        
        std::getline(ss, token, ',');
        y = std::stod(token);
        
        //Grow the vector if we haven't seen this ring_id before
        if ((size_t)ring_id >= ringVec.size())
        {
            ringVec.resize(ring_id + 1);
            ringVec[ring_id].ringID = ring_id;
        }

        Vertice v;
        v.ring_id = ring_id;
        v.vertex_id = vertex_id;
        v.pos = Vec2(x, y);

        ringVec[ring_id].vertices.push_back(v);
    }

    //Convert vertex vectors to circular linked lists
    ConvertToLinkedList(ringVec);

    return ringVec;
}


void ConvertToLinkedList(std::vector<Ring>& ringVec)
{
    //Loop through each ring
    for (Ring& ring : ringVec)
    {
        if (ring.vertices.empty()) continue;

        Node* first = nullptr;  //first node in the new list
        Node* prev = nullptr;  //the node we built in the previous iteration

        for (const Vertice& v : ring.vertices)
        {
            Node* node = new Node();
            node->v = v;

            //Record the first node so we can close the circle later
            if (!first) first = node;

            //Link this node to the previous one
            if (prev)
            {
                prev->next = node;
                node->prev = prev;
            }

            prev = node;
        }

        //Close the circular list: last <-> first
        prev->next  = first;
        first->prev = prev;

        ring.head = first;
        ring.size = (int)ring.vertices.size();

        //Record the original area so we can compute displacement later
        ring.originalArea = ComputeRingArea(ring);

        //Free the temporary vertex storage
        ring.vertices.clear();
    }
}

void OutputRings(std::vector<Ring>& ringVec)
{
    std::cout << "ring_id,vertex_id,x,y" << std::endl;

    //Loop through each
    for (Ring& ring : ringVec)
    {
        if (!ring.head) continue;

        const Node* curr = ring.head;
        int vid = 0;

        do
        {
            //Using 10s.f
            std::cout << ring.ringID << ","
                        << vid++ << ","
                        
                        << std::setprecision(10) << std::defaultfloat
                        << curr->v.pos.x << ","
                        << curr->v.pos.y << std::endl;

            curr = curr->next;
        } while (curr != ring.head);
    }

    double inputArea = 0.0;
    for (const Ring& ring : ringVec)
        inputArea += ring.originalArea;

    double outputArea = ComputeTotalArea(ringVec);
    double displacement = ComputeDisplacement(ringVec);

    //Print in scientific notation as required by the spec
    std::cout << std::scientific;
    std::cout << "Total signed area in input: " << inputArea << std::endl;
    std::cout << "Total signed area in output: " << outputArea << std::endl;
    std::cout << "Total areal displacement: " << displacement << std::endl;
}

void FreeRings(std::vector<Ring>& ringVec)
{
    //Just loop through and call each ring's free func
    for (Ring& ring : ringVec)
        ring.FreeRing();
}
