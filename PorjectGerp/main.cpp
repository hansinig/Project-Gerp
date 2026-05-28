/*
 * main.cpp
 * Jessica Charalel and Hansini Gundavarapu
 * 12/09/2025
 *
 * CS 15 Project Gerp
 *
 * Main driver program for Gerp. This file takes a directory
 * path and an output file as command-line arguments, builds an index of all
 * words in all files within the directory tree, and allows the user to
 * search for words interactively.
 */

#include "gerp.h"
#include <iostream>
#include <cstdlib>

using namespace std;

int main (int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Usage: ./gerp inputDirectory outputFile" << endl;
        return EXIT_FAILURE;
    }

    string directoryName = argv[1];
    string outputFileName = argv[2];

    //gerp instance
    Gerp gerp;

    gerp.buildIndex(directoryName);
    gerp.queryLoop(outputFileName);

    return EXIT_SUCCESS;
}