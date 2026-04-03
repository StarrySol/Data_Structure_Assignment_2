/******************************************************************************
 * File:        FileReader.h
 * Project:     Data Structures Assigment 2
 *
 * Description:
 * This file contains the deceration of functions to input and output data for rings
 * It also converts vparsed vertices into a doubled circular linked list

 * Functions:
 * LoadFile            -- parse CSV -> vector of Ring objects
 * ConvertToLinkedList -- convert parsed vertices -> circular lists
 * OutputRings         -- print simplified rings to stdout
 * FreeRings           -- release all heap-allocated Node memory
 ******************************************************************************/
#pragma once

#include "Ring.h"
#include <fstream>
#include <vector>

/************************************************************************/
/*!
\brief
Reads the CSV input file and returns a vector of Ring objects.

CSV FORMAT (one vertex per line, header line skipped):
ring_id, vertex_id, x, y

Vertices are grouped by ring_id.  Ring 0 is the exterior boundary
(counterclockwise); rings 1, 2, ... are holes (clockwise).

After all vertices are collected, ConvertToLinkedList is called to convert them into doubly circular linked list form.

\param inputFile
An open std::ifstream positioned at the start of the file.

\return           
A vector of Ring objects
Empty rings may be present if ring IDs are not in order.
*/
/************************************************************************/
std::vector<Ring> LoadFile(std::ifstream& inputFile);

/************************************************************************/
/*!
\brief
Converts the temporary vertex vectors inside each Ring into circular
doubly-linked lists.

Called automatically by LoadFile after parsing is complete.

For each ring:
1. Allocate one Node per vertex.
2. Link them into a doubly-linked chain.
3. Close the chain (last->next = first, first->prev = last).
4. Set ring.head, ring.size, and ring.originalArea.
5. Clear the temporary vertex vector to free memory.

\param ringVec
Vector of Ring objects whose vertex vectors are populated.
*/
/************************************************************************/
void ConvertToLinkedList(std::vector<Ring>& ringVec);

/************************************************************************/
/*!
\brief
Prints the simplified rings to standard output in the required CSV format.

Output format:
ring_id,vertex_id,x,y         
<ring_id>,<vid>,<x>,<y>

Total signed area in input:  <value>
Total signed area in output: <value>
Total areal displacement:    <value>

Coordinates are printed with up to 10 s.f

Area values are printed in scientific notation.

\param ringVec
Vector of Ring objects after simplification.
*/
/************************************************************************/
void OutputRings(std::vector<Ring>& ringVec);

/************************************************************************/
/*!
\brief
Deletes all created nodes in the vector of Rings

\param ringVec
Vector of Ring objects to free.
*/
/************************************************************************/
void FreeRings(std::vector<Ring>& ringVec);
