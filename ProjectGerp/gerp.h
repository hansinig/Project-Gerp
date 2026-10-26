/*
 * gerp.h
 * Jessica Charalel and Hansini Gundavarapu
 * 12/09/2025
 *
 * CS 15 Project Gerp
 *
 * Header file for the Gerp class. This class manages the indexing and
 * searching of words across a directory tree. It provides functionality
 * to build an index from a directory structure and handle user queries
 * for both case-sensitive and case-insensitive word searches.
 */
#ifndef GERP_H
#define GERP_H

#include "HashTable.h"
#include "FSTree.h"
#include "processing.h"
#include "DirNode.h"
#include <cctype>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

 class Gerp {
    public:
        Gerp();
        ~Gerp();
        void buildIndex(const string &directory);
        void queryLoop(const string &outputFileName);

    private:
        HashTable hash;
        vector<string> allFilePaths;
        vector<string> allLineTexts;
        std::ofstream output;
        std::string input;
        std::string directoryName;
        std::string outFileName;

        void traverseAndIndex(DirNode* node, const string& currentPath);
        void indexFile(const string& filePath);
        void indexLine(const string& line, size_t fileIdx, size_t lineNum);
        
        void processCommand(const string &command, istringstream &iss, 
            ofstream &out);
        void handleQuit(ofstream &out);
        void handleChangeFile(istringstream &iss, ofstream &out);
        void handleInsensitiveSearch(istringstream &iss, ofstream &out);
        void handleSensitiveSearch(const string &query, ofstream &out);

    // ====== Utility helper functions ======
        void printResults(ofstream &out, const vector<WordOccurrence> &results);

};

#endif
