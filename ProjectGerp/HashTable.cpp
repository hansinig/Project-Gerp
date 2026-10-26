/*
 * HashTable.cpp
 * Jessica Charalel and Hansini Gundavarapu
 * 12/09/25
 *
 * CS 15 Project Gerp
 *
 * Implements the HashTable class; This class implements a hash table
 * using open addressing with linear probing. It stores word occurrences
 * (file index, line number, and line content index) and provides methods
 * for insertion and both case-sensitive and case-insensitive searching.
 * The table dynamically resizes when needed to maintain performance.
 */

#include "HashTable.h"
#include "processing.h"
#include <cctype>

/*
 * name:      HashTable(size_t initialCapacity)
 * purpose:   parameterized constructor for the HashTable class
 * arguments: initialCapacity - the initial size of the hash table
 * returns:   none
 * effects:   initializes a hash table with the specified capacity
 * other:     constructor sets numElements to 0 and tableSize to the 
 * given capacity
 */
HashTable::HashTable(size_t initialCapacity) : numElements(0), 
tableSize(initialCapacity) {
    table.resize(initialCapacity);
}

/*
 * name:      HashTable()
 * purpose:   default constructor for the HashTable class
 * arguments: none
 * returns:   none
 * effects:   initializes a hash table with default capacity of 1000
 * other:     default capacity chosen to balance memory usage and performance
 */
HashTable::HashTable() : numElements(0), tableSize(1000) {
    table.resize(1000);
}

/*
 * name:      ~HashTable()
 * purpose:   to destruct the HashTable instance when called
 * arguments: none
 * returns:   none
 * effects:   deallocates all memory for hash table entries
 * other:     destructor, needs no arguments due to no pointers used
 */
HashTable::~HashTable() {
}

/*
 * name:      insert(const string &k, const WordOccurrence &occurrence)
 * purpose:   to insert a word occurrence into the hash table
 * arguments: k - the word key to insert
 *            occurrence - the WordOccurrence struct containing file/line info
 * returns:   none
 * effects:   adds occurrence to existing key or creates new entry, resizes
 *            table if load factor exceeds threshold
 * other:     uses linear probing for collision resolution
 */
void HashTable::insert(const string &k, const WordOccurrence &occurrence){
    bool found = false;
    size_t index = findSlot(k, found);

    if (found) {
        //key already exists at this index
        table[index].occurrences.push_back(occurrence);
    } else {
        //new key at empty slot
        table[index].key = k;
        table[index].isOccupied = true;
        table[index].occurrences.push_back(occurrence);
        numElements++;

        if (needsResize()) {
            expand();
        }
    }
}

/*
 * name:      searchSens(const std::string &k, bool& found)
 * purpose:   to perform a case-sensitive search for a word in the hash table
 * arguments: k - the word key to search for
 *            found - reference to bool indicating if word was found
 * returns:   vector of WordOccurrence structs for all matching instances
 * effects:   sets found to true if word exists, false otherwise
 * other:     uses linear probing to search through collisions
 */
vector<WordOccurrence> HashTable::searchSens(const std::string &k, bool& found)
{
    size_t index = hash(k);
    size_t probes = 0;

    while (probes < table.size()){
        if (not table[index].isOccupied) {
            found = false;
            return std::vector<WordOccurrence>();
        }

        if (table[index].key == k) {
            found = true;
            return table[index].occurrences;
        }

        index = (index + 1) % table.size();
        probes++;
    }

    found = false;
    return std::vector<WordOccurrence>();
}

/*
 * name:      searchInsens(const string &k, bool &found)
 * purpose:   to perform a case-insensitive search for a word in the hash table
 * arguments: k - the word key to search for
 *            found - reference to bool indicating if word was found
 * returns:   vector of WordOccurrence structs for all matching instances
 * effects:   searches entire table for case-insensitive matches, sets found
 *            to true if any matches exist
 * other:     converts all words to lowercase for comparison
 */
vector<WordOccurrence> HashTable::searchInsens(const string &k, bool &found) {
    string lowerKey = toLowerCase(k);
    vector<WordOccurrence> allMatches;

    for (size_t i = 0; i < table.size(); i++) {
        if (table[i].isOccupied) {
            if (toLowerCase(table[i].key) == lowerKey) {
                allMatches.insert(allMatches.end(),
                table[i].occurrences.begin(), table[i].occurrences.end());
            }
        }
    }
    found = not allMatches.empty();
    return allMatches;
}

/*
 * name:      getTableSize() 
 * purpose:   to retrieve the current capacity of the hash table
 * arguments: none
 * returns:   the current size/capacity of the hash table
 * effects:   none
 * other:     const function, returns tableSize member variable
 */
size_t HashTable::getTableSize() const{
    return tableSize;
}

/*
 * name:      getLoadFactor()
 * purpose:   to calculate the current load factor of the hash table
 * arguments: none
 * returns:   the load factor (ratio of elements to capacity)
 * effects:   none
 * other:     returns 0.0 if table size is 0 to avoid division by zero
 */
double HashTable::getLoadFactor() const{
    if (table.size() == 0) {
        return 0.0;
    }
    return static_cast<double>(numElements) / table.size();
}

/*
 * name:      isEmpty()
 * purpose:   to check if the hash table contains any elements
 * arguments: none
 * returns:   true if table is empty, false otherwise
 * effects:   none
 * other:     const function, checks numElements member variable
 */
bool HashTable::isEmpty() const {
    return numElements == 0;
}

/*
 * name:      hash(const std::string &k)
 * purpose:   to compute the hash value for a given key
 * arguments: k - the string key to hash
 * returns:   the hash index within the table bounds
 * effects:   none
 * other:     uses std::hash function and modulo to fit within table size,
 *            returns 0 if table is empty
 */
size_t HashTable::hash(const std::string &k) const{
    if (table.size() == 0) {
        return 0;
    }
    std::hash<std::string> hasher;
    return hasher(k) % table.size();
}

/*
 * name:      findSlot(const string &k, bool &found)
 * purpose:   to find the appropriate slot for a key using linear probing
 * arguments: k - the string key to find or place
 *            found - reference to bool indicating if key already exists
 * returns:   the index where the key exists or should be placed
 * effects:   sets found to true if key exists, false if slot is empty
 * other:     helper function for insert operation
 */
size_t HashTable::findSlot(const string &k, bool &found) {
    size_t index = hash(k);
    size_t probes = 0;

    while (table[index].isOccupied and probes < table.size()) {
        if (table[index].key == k) {
            found = true;
            return index;
        }
        index = (index + 1) % table.size();
        probes++;
    }

    found = false;
    return index;
}

/*
 * name:      expand()
 * purpose:   to resize the hash table when load factor exceeds threshold
 * arguments: none
 * returns:   none
 * effects:   doubles the table size and rehashes all existing entries
 * other:     uses default size of 1000 if expanding from 0, called by insert
 *            when needsResize returns true
 */
void HashTable::expand() {
    std::vector<HashEntry> oldTable = std::move(table);

    size_t newSize = oldTable.size() * 2;
    if (newSize == 0) {
        newSize = 1000;  // Fallback to default size if expanding from 0
    }

    table.clear();
    table.resize(newSize);
    tableSize = table.size();
    numElements = 0;

    for (size_t i = 0; i < oldTable.size(); i++) {
        if(oldTable[i].isOccupied) {
            for (const WordOccurrence &occurrence : oldTable[i].occurrences) {
                insert(oldTable[i].key, occurrence);
            }
        }
    }
}

/*
 * name:      toLowerCase(const string &str)
 * purpose:   to convert a string to lowercase for case-insensitive comparison
 * arguments: str - the string to convert
 * returns:   a new string with all characters converted to lowercase
 * effects:   none (const function)
 * other:     helper function for searchInsens
 */
string HashTable::toLowerCase(const string &str) const {
    string result = str;
    for (char &c : result) {
        c = std::tolower(c);
    }
    return result;
}

/*
 * name:      needsResize()
 * purpose:   to determine if the hash table should be expanded
 * arguments: none
 * returns:   true if load factor exceeds 0.7 or table size is 0
 * effects:   none (const function)
 * other:     threshold of 0.7 maintains good performance with linear probing
 */
bool HashTable::needsResize() const {
    if (table.size() == 0) {
        return true;
    }
    double loadFactor = static_cast<double>(numElements) / table.size();
    return loadFactor > 0.7;
}

/*
 * name:      size()
 * purpose:   to get the number of unique keys stored in the hash table
 * arguments: none
 * returns:   the count of unique keys (not total occurrences)
 * effects:   none (const function)
 */
size_t HashTable::size() const {
    return numElements;
}



