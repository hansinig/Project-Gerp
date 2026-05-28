/*
 * processing.h
 *
 * CS 15 Project Gerp
 *
 * Authors: Hansini Gundavarupu (hgunda01), Jess Charalel (jchara02)
 * Date: November 2024
 *
 * Purpose: Interface for utility functions for file tree traversal
 *          and string processing for the gerp search engine.
 */

#ifndef PROCESSING_H
#define PROCESSING_H

#include <string>
#include "DirNode.h"

// Function declerations:
std::string stripNonAlphaNum(std::string input);
void traverseDirectory(std::string directory);
void traverseHelper(DirNode* node, std::string currentPath);

#endif
