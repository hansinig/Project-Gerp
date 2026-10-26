/*
 * gerp.cpp
 * Jessica Charalel and Hansini Gundavarapu
 * 12/09/25
 *
 * CS 15 Project Gerp
 *
 * Implements the gerp class; This class manages the indexing and
 * searching of words across a directory tree. It provides functionality
 * to build an index from a directory structure and handle user queries
 * for both case-sensitive and case-insensitive word searches.
 */

#include "gerp.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set> 

using namespace std;

/*
 * name:      Gerp()
 * purpose:   constructor for the Gerp class instance
 * arguments: none 
 * returns:   none
 * effects:   default constructor for the Gerp class instance, to appease 
 * compiler more
 */
Gerp::Gerp() {
}

/*
 * name:      ~Gerp()
 * purpose:   to destruct the gerp instance when called
 * arguments: none
 * returns:   none
 * effects:   deallocates all memory to objects, and subsequent file and 
 * line paths
 * other:     destructor, needs no arguments due to no pointers used
 */
Gerp::~Gerp() {
    if (output.is_open()) {
        output.close();
    }
    allFilePaths.clear();
    allLineTexts.clear();
}

/*
 * name:      buildIndex(const string &directory)
 * 
 * purpose:   to build a FSTree with the given directory, making the root 
 * a directory node and then traversing the resulting directory and files
 * 
 * arguments: a string reference to the given director
 * returns:   none
 * effects:   builds a tree with the given directory and following files
 * other:     had errors pertaining to the node that is chosen for the root
 */
void Gerp::buildIndex(const string &directory) {
    directoryName = directory;
    FSTree tree(directoryName);

    DirNode *root = tree.getRoot();
    traverseAndIndex(root, directoryName);
}

/*
 * name:      queryLoop(const string &outputFileName)
 * 
 * purpose:   To begin the query loop with the main user input functionality
 * with the correct action depending on the user's query
 * 
 * arguments: a string reference to the given output file name
 * returns:   none
 * effects:   depending on the user's query, will direct the program into a 
 * given path either quiting the program, or redirecting input into the 
 * the process command function. 
 * 
 * other:     at first we had the command loop in the queryloop function, but
 * then made it into two separate functions for modularity
 */
void Gerp::queryLoop(const string &outputFileName) {
    outFileName = outputFileName;
    output.open(outFileName);
    if (not output.is_open()){
        std::cerr << "Error: cannot open file " << outputFileName << std::endl;
        exit(EXIT_FAILURE);
    }

    while (true) {
        std::cout << "Query? ";

        if(not std::getline(std::cin, input)) {
            handleQuit(output);
            return;
        }

        std::istringstream iss(input);
        string command;

        if (iss >> command) {
            processCommand(command, iss, output);
        }
    }
}

/*
 * name:      processCommand(const string &command, istringstream &iss, 
 *   ofstream &out)
 * 
 * purpose:   to process the given command from the query loop, and feed it in
 * the output file for a result 
 * arguments: const string &command, istringstream &iss, 
 *   ofstream &out
 * returns:   none
 * effects:   either starts insensitive or sensitive search, and then runs
 * the following output to a given output stream
 * 
 * other:     At first had a huge function just for query loops, but by 
 * breaking down the code into different functions we found that our errors
 * went significantly down in number
 */
void Gerp::processCommand(const string &command, istringstream &iss, 
    ofstream &out) {
    if (command == "@q" or command == "@quit") {
        handleQuit(out);
        exit(EXIT_SUCCESS);
    }
    else if (command == "@f") {
        handleChangeFile(iss, out);
    }
    else if (command == "@i" or command == "@insensitive") {
        handleInsensitiveSearch(iss, out);
    }
    else {
        handleSensitiveSearch(command, out);
    }
}

/*
 * name:      indexFile
 * purpose:   to index all words in a single file
 * arguments: filePath - the full path to the file to index
 * returns:   none
 * effects:   opens file, reads each line, calls indexLine for each line
 * other:     skips files that cannot be opened (no error message)
 */
void Gerp::indexFile(const string &filePath) {
    std::ifstream file(filePath);
    if (not file.is_open()) {
        return;
    }

    // Store file path and get its index
    size_t fileIdx = allFilePaths.size();
    allFilePaths.push_back(filePath);

    std::string line;
    size_t lineNumber = 1;

    while (std::getline(file, line)) {
        indexLine(line, fileIdx, lineNumber);
        lineNumber++;
    }

    file.close();
}

/*
 * name:      indexLine(const string &line, size_t fileIdx, size_t lineNum)
 * purpose:   to index all words in a single line from a file
 * arguments: line - the text content of the line
 *            filePath - the full path to the file
 *            lineNumber - the line number within the file
 * returns:   none
 * effects:   creates LineInfo, parses words, strips non-alphanumeric chars,
 *            inserts each word into the index
 * other:     skips empty words after stripping
 */
void Gerp::indexLine(const string &line, size_t fileIdx, size_t lineNum) {
    size_t lineTextIdx = allLineTexts.size();
    allLineTexts.push_back(line);

    std::istringstream iss(line);
    std::string word;

    //reading in words and creating wordOccurances as a result
    while (iss >> word) {
        std::string cleanedWord = stripNonAlphaNum(word);
        if (not cleanedWord.empty()) {
            WordOccurrence occurrence;
            occurrence.fileIndex = fileIdx;
            occurrence.lineNumber = lineNum;
            occurrence.lineContent = lineTextIdx;
            hash.insert(cleanedWord, occurrence);
        }
    }
}

/*
 * name:      traverseAndIndex(DirNode* node, const string &currentPath)
 * purpose:   to recursively traverse directory tree and index all files
 * arguments: node - pointer to current directory node
 *            path - current path string for building full file paths
 * returns:   none
 * effects:   processes all files in current directory, then recursively
 *            processes all subdirectories
 * other:     recursive function, base case is no subdirectories
 */
void Gerp::traverseAndIndex(DirNode* node, const string &currentPath) {
    if (node == nullptr) {
        return;
    }

    for (int i = 0; i < node->numFiles(); i++) {
        string filePath = currentPath + "/" + node->getFile(i);
        indexFile(filePath);
    }

    for (int i = 0; i < node->numSubDirs(); i++) {
        DirNode* subdir = node->getSubDir(i);
        string subdirPath = currentPath + "/" + subdir->getName();
        traverseAndIndex(subdir, subdirPath);
    }
}

/*
 * name:      printResults(ofstream &out, const vector<WordOccurrence> 
 * &results)
 * purpose:   to print search results to the output file
 * arguments: output - output file stream for writing results
 *            results - vector of LineInfo objects containing search results
 * returns:   none
 * effects:   writes each result line to output file in format:
 *            filepath:lineNumber: lineContent
 * other:     prints all results even if same line appears multiple times
 */
void Gerp::printResults(ofstream &out, const vector<WordOccurrence> &results) {
    std::set<std::pair<size_t, size_t>> printed;

    for (const WordOccurrence &occ : results) {
        std::pair<size_t, size_t> key(occ.fileIndex, occ.lineNumber);
        if (printed.find(key) == printed.end()) {
            out << allFilePaths[occ.fileIndex] << ":" 
            << occ.lineNumber << ": " 
            << allLineTexts[occ.lineContent] << std::endl;
            printed.insert(key);
        }
    }
}

/*
 * name:      handleQuit(ofstream &out)
 * purpose:   to handle the quit command and exit the program gracefully
 * arguments: output - output file stream to close
 * returns:   none
 * effects:   prints goodbye message to stdout, closes output file
 * other:     called when @q, @quit, or EOF is encountered
 */
void Gerp::handleQuit(ofstream &out) {
    std::cout << "Goodbye! Thank you and have a nice day." << std::endl;
    if (out.is_open()) {
        out.close();
    }
}

/*
 * name:      handleChangeFile(istringstream &iss, ofstream &out)
 * purpose:   to change the output file for subsequent query results
 * arguments: iss - input string stream containing new filename
 *            output - output file stream to close and reopen
 * returns:   none
 * effects:   closes current output file, opens new file with given name
 * other:     exits with error if new file cannot be opened
 */
void Gerp::handleChangeFile(istringstream &iss, ofstream &out) {
    std::string newOutputFile;
    if (iss >> newOutputFile) {
        if (out.is_open()) {
            out.close();
        }

        out.open(newOutputFile);
        if (not out.is_open()) {
            std::cerr << "Error: cannot open file " << newOutputFile 
            << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

/*
 * name:      handleInsensitiveSearch(istringstream &iss, ofstream &out)
 * purpose:   to perform a case-insensitive search for a query word
 * arguments: iss - input string stream containing the query
 *            output - output file stream for writing results
 * returns:   none
 * effects:   reads query, strips non-alphanumeric chars, searches index
 *            case-insensitively, prints results or "Not Found" message
 * other:     skips empty queries after stripping
 */
void Gerp::handleInsensitiveSearch(istringstream &iss, ofstream &out) {
    std::string query;
    if (iss >> query) {
        query = stripNonAlphaNum(query);
        bool found = false;
        vector<WordOccurrence> results = hash.searchInsens(query, found);

        if (not found or results.empty()) {
            out << query << " Not Found." << std::endl;
        } else {
            printResults(out, results);
        }
    }
}

/*
 * name:      handleSensitiveSearch(const string &command, ofstream &out)
 * purpose:   to perform a case-sensitive search for a query word
 * arguments: command - the query word to search for
 *            output - output file stream for writing results
 * returns:   none
 * effects:   strips non-alphanumeric chars from query, searches index
 *            case-sensitively, prints results or "Not Found" message
 * other:     suggests @insensitive if not found
 */
void Gerp::handleSensitiveSearch(const string &command, ofstream &out) {
    std::string query = stripNonAlphaNum(command);
    bool found = false;
    std::vector<WordOccurrence> results = hash.searchSens(query, found);

    if (not found or results.empty()) {
        out << query << " Not Found. Try with @insensitive or @i." 
        << std::endl;
    } else {
        printResults(out, results);
    }
}


