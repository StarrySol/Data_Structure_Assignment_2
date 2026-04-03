#pragma once

#include <vector>
#include <fstream>
#include <string>
#include "Polygon.h"

/*
CSV files are "comma-separated internally"
spreadsheet programs hide the commas and show the data in columns

e.g. 

ring_id,vertex_id,x,y
0,0,0,0
0,1,200,-50
0,2,400,0

*/

//Read into this then convert to Polygon/Ring
struct CSVRow{
    unsigned int ring_id{}; //cant be negative
    unsigned int vertex_id{}; //cant be negative
    double x{};
    double y{};
};

//static member function design
// CSVReader is a utility class
/*
    no data members
    the function belongs to the class itself
    it does not need an object
    it cannot use this
    it can only access other static members directly
*/
class CSVReader
{
public:
    static Polygon ReadPolygon(const std::string& filename);
private:
    static std::vector<CSVRow> ReadRows(const std::string& filename); //parse raw CSV text into rows
    static Polygon BuildPolygon(const std::vector<CSVRow>& rows); //convert rows into polygon structure
};