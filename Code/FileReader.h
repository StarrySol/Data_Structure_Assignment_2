#pragma once

#include "Shape.h"
#include <vector>
#include <fstream>

/************************************************************************/
/*!
\brief
Handles input for csv file and returns a vector of shapes in the file

\param inputFile
ifstream of input file, file needs to already be open, 
it does not manage it by opening or closing it

\return
std::vector of all shapes within the file

*/
/************************************************************************/
std::vector<Shape> LoadFile(std::ifstream& inputFile);

/************************************************************************/
/*!
\brief
Handles output of a vector of shapes 

\param inputFile
ifstream of input file, file needs to already be open, 
it does not manage it by opening or closing it

\param shapeVec
std::vector of all shapes and vertices to print
*/
/************************************************************************/
void OutputVertices(std::vector<Shape>& shapeVec);
