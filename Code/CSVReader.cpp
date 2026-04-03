#include "CSVReader.h"

#include <sstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>

std::vector<CSVRow> CSVReader::ReadRows(const std::string& filename)
{
    std::ifstream inputFile(filename);
    if (!inputFile)
    {
        throw std::runtime_error("Failed to open CSV file: " + filename);
    }

    std::vector<CSVRow> rows;
    std::string line;

    // Read and validate header
    if (!std::getline(inputFile, line))
    {
        throw std::runtime_error("CSV file is empty: " + filename);
    }

    if (!line.empty() && line.back() == '\r')
    {
        line.pop_back();
    }

    if (line != "ring_id,vertex_id,x,y")
    {
        throw std::runtime_error("Invalid CSV header. Expected: ring_id,vertex_id,x,y");
    }

    std::size_t lineNumber = 1; //for debugging

    while (std::getline(inputFile, line))
    {
        ++lineNumber;

        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        // Skip empty lines
        if (line.empty())
            continue;

        std::stringstream ss(line);
        std::string token;

        CSVRow row{}; //defauilt tmp obj

        // ring_id
        if (!std::getline(ss, token, ','))
        {
            throw std::runtime_error("Missing ring_id at line " + std::to_string(lineNumber));
        }
        //convert string to unsigned int
        row.ring_id = static_cast<unsigned int>(std::stoul(token));

        // vertex_id
        if (!std::getline(ss, token, ','))
        {
            throw std::runtime_error("Missing vertex_id at line " + std::to_string(lineNumber));
        }
        row.vertex_id = static_cast<unsigned int>(std::stoul(token));

        // x
        if (!std::getline(ss, token, ','))
        {
            throw std::runtime_error("Missing x at line " + std::to_string(lineNumber));
        }
        row.x = std::stod(token);

        // y
        if (!std::getline(ss, token, ','))
        {
            throw std::runtime_error("Missing y at line " + std::to_string(lineNumber));
        }
        row.y = std::stod(token);

        rows.push_back(row);
    }

    return rows;
}

//build a polygon from data values in a CSV file
Polygon CSVReader::BuildPolygon(const std::vector<CSVRow>& rows) 
{
    Polygon polygon{};

    if (rows.empty()){
        return polygon;
    }
        

    // Find largest ring id so we can resize once
    unsigned int maxRingID = 0;
    for ( CSVRow const& row : rows)
    {
        if (row.ring_id > maxRingID){
            maxRingID = row.ring_id;
        }            
    }

    std::vector<Ring> builtRings;
    builtRings.reserve(static_cast<std::size_t>(maxRingID) + 1);

    for (unsigned int i = 0; i <= maxRingID; ++i)
    {
        builtRings.emplace_back(i);
    }


    // Copy rows first so we can sort by ring_id, then vertex_id
    std::vector<CSVRow> sortedRows = rows;
    std::sort(sortedRows.begin(), sortedRows.end(),
        [](const CSVRow& a, const CSVRow& b)
        {
            if (a.ring_id != b.ring_id){
                 return a.ring_id < b.ring_id;
            }       
            return a.vertex_id < b.vertex_id;
        });
    for ( CSVRow const& row : sortedRows)
    {
        builtRings[row.ring_id].AddVertex(Vec2(row.x, row.y));
    }

    for ( Ring const& ring  : builtRings)
    {
        polygon.AddRing(ring);
    }

    return polygon; // return final polygon created from CSV data values
}

Polygon CSVReader::ReadPolygon(const std::string& filename)
{
    std::vector<CSVRow> rows = ReadRows(filename);
    return BuildPolygon(rows);
}