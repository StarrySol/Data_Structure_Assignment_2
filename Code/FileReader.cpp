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
        if ((size_t)ring_id >= ringVec.size())
        {
            ringVec.resize(ring_id + 1);
            ringVec[ring_id].ringID = ring_id;
        }
        
        //Create vertex
        Vertice v;
        v.ring_id = ring_id;
        v.vertex_id = vertex_id;
        v.pos = Vec2(x, y);

        //Add new vertex into list
        ringVec[ring_id].vertices.push_back(v);
    }

    return ringVec;
}

#include <vector>

//Convert vector of rings into circular doubly linked lists
void ConvertToLinkedList(std::vector<Ring>& ringVec)
{
    //Loop through each ring
    for (Ring& ring : ringVec)
    {
        //Skip empty rings
        if (ring.vertices.empty())
            continue;

        Node* first = nullptr;//first node in list
        Node* prev = nullptr;//previous node while building

        //Create nodes from vertices
        for (const Vertice& v : ring.vertices)
        {
            //Allocate new node
            Node* node = new Node();
            node->v = v;

            //Set first node
            if (!first)
                first = node;

            //Link with previous node
            if (prev)
            {
                prev->next = node;
                node->prev = prev;
            }

            prev = node;
        }

        //Close the loop (circular list)
        prev->next = first;
        first->prev = prev;

        //Assign ring head
        ring.head = first;

        //Set size
        ring.size = (int)ring.vertices.size();

        ring.vertices.clear();
    }
}

void OutputVertices(std::vector<Ring>& ringVec)
{
    std::cout << "ring_id,vertex_id,x,y" << std::endl;

    for (Ring& ring : ringVec)
    {
        if (!ring.head)
            continue;

        const Node* curr = ring.head;

        do
        {
            std::cout << curr->v.pos.x << "," << curr->v.pos.y << "\n";
            curr = curr->next;
        } while (curr != ring.head);
    }

    std::cout << std::endl; //Additional newline
}

void FreeRings(std::vector<Ring>& ringVec)
{
    for (Ring& ring : ringVec)
    {
        ring.FreeRing();
    }
}