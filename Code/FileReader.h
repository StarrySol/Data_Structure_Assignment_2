#pragma once

#include "Ring.h"
#include <vector>
#include <fstream>

/************************************************************************/
/*!
\brief
Handles input for csv file and returns a vector of ring in the file

\param inputFile
ifstream of input file, file needs to already be open, 
it does not manage it by opening or closing it

\return
std::vector of all rings within the file
*/
/************************************************************************/
std::vector<Ring> LoadFile(std::ifstream& inputFile);

/************************************************************************/
/*!
\brief
Converts the vector of rings into a doubly circular linked list

\param ringVec
vector of rings

\return
std::vector of all rings within the file
*/
/************************************************************************/
void ConvertToLinkedList(std::vector<Ring>& ringVec);

/************************************************************************/
/*!
\brief
Handles output of a vector of rings 

\param ringVec
std::vector of all rings and vertices to print
*/
/************************************************************************/
void OutputVertices(std::vector<Ring>& ringVec);

/************************************************************************/
/*!
\brief
Handles freeing the ringss

\param ringVec
std::vector of all rings and vertices to free
*/
/************************************************************************/
void FreeRings(std::vector<Ring>& ringVec);