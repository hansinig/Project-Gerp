/*
 * processing.cpp
 * 
 * CS 15 Project Gerp
 * 
 * Authors: Hansini Gundavarupu (hgunda01), Jess Charalel (jchara02)
 * Date: November 2024
 * 
 * Purpose: Implementation of utility functions for file tree traversal
 *          and string processing for the gerp search engine.
 */

#include "FSTree.h"
#include "DirNode.h"
#include "processing.h"
#include <iostream>
#include <cctype>

using namespace std;

string stripNonAlphaNum(string input);
void traverseDirectory(string directory);
void traverseHelper(DirNode* node, string currentPath);

// Function declarations:
/*
 * name:      stripNonAlphaNum
 * purpose:   Removes all leading and trailing non-alphanumeric characters
 *            from a string
 * arguments: A string to process
 * returns:   The processed string
 * effects:   None
 */
string stripNonAlphaNum(string input) {
    if (input.empty()) {
        return input;
    }
    
    size_t start = 0;
    while (start < input.length() and not isalnum(input[start])) {
        start++;
    }
    
    if (start == input.length()) {
        return "";
    }
    
    size_t end = input.length() - 1;
    while (end > start and not isalnum(input[end])) {
        end--;
    }
    
    return input.substr(start, end - start + 1);
}

/*
 * name:      traverseDirectory
 * purpose:   Prints the full path of every file in a directory tree
 * arguments: The name of the directory to traverse
 * returns:   None
 * effects:   Prints file paths to std::cout; creates FSTree which may
 *            throw exception if directory doesn't exist
 */
void traverseDirectory(string directoryName) {
    FSTree tree(directoryName);
    traverseHelper(tree.getRoot(), directoryName);
}

/*
 * name:      traverseHelper
 * purpose:   Recursive helper function to traverse directory tree and
 *            print all file paths
 * arguments: Pointer to current DirNode, current path string
 * returns:   None
 * effects:   Prints file paths to std::cout
 */
void traverseHelper(DirNode* node, string currentPath) {
    if (node == nullptr) {
        return;
    }

    for (int i = 0; i < node->numFiles(); i++) {
        cout << currentPath << "/" << node->getFile(i) << endl;
    }
    for (int i = 0; i < node->numSubDirs(); i++) {
        DirNode* subdir = node->getSubDir(i);
        string subdirPath = currentPath + "/" + subdir->getName();
        traverseHelper(subdir, subdirPath);
    }
}