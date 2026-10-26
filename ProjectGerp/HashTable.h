/*
 * HashTable.h
 * Jessica Charalel and Hansini Gundavarapu
 * 12/09/2025
 *
 * CS 15 Project Gerp
 *
 * Header file for the HashTable class. This class implements a hash table
 * using open addressing with linear probing. It stores word occurrences
 * (file index, line number, and line content index) and provides methods
 * for insertion and both case-sensitive and case-insensitive searching.
 * The table dynamically resizes when needed to maintain performance.
 */

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>

using namespace std;

struct WordOccurrence {
    size_t fileIndex;
    size_t lineNumber;
    size_t lineContent;
};

struct HashEntry {
    string key;
    vector<WordOccurrence> occurrences;
    bool isOccupied;

    HashEntry() : isOccupied(false) {}
};

class HashTable {
public:
    HashTable(size_t initialCapacity);
    HashTable();
    ~HashTable();

    void insert(const std::string& key, const WordOccurrence& occurrence);
    vector<WordOccurrence> searchSens(const std::string& key, bool& found);
    vector<WordOccurrence> searchInsens(const std::string& key, bool& found);

    size_t initialCapacity = 1000;
    size_t size() const;
    bool isEmpty() const;
    size_t getTableSize() const;
    double getLoadFactor() const;
    
private:
    vector<HashEntry> table;
    size_t numElements;
    size_t tableSize;
    
    size_t hash(const std::string& key) const;              
    size_t findSlot(const string &k, bool& found);   
    bool needsResize() const;                                 
    void expand();                                           
    string toLowerCase(const string& str) const;
};

#endif
