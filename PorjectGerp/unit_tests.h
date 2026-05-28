/*
 * unit_tests.h
 * Jessica Charalel and Hansini Gundavarapu
 * 12/09/2025
 *
 * CS 15 Project Gerp
 *
 * Purpose: This file holds all the unit tests for the functions implemented in
 * processing.cpp as well as for the HashTable class.
 */

#include "processing.h"
#include "DirNode.h"
#include "FSTree.h"
#include "HashTable.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cassert>

using namespace std;
/*
 * TEST: alphanumeric function with small input
 */
void nonAlpha_small() {
    string input = "******hjajwenja***";
    //cout << stripNonAlphaNum(input) << endl;
    assert(stripNonAlphaNum(input)== "hjajwenja");
}

/*
 * TEST: count_freqs with single character
 */
void nonAlpha_medium(){
    string input = " *((****(***(***hi****)***)))";
    assert(stripNonAlphaNum(input) == "hi");
}

/*
 * TEST: count_freqs with single character
 */
void nonAlpha_largeish(){
    string input = "*@!*@!*@!*@!*@!*@!*@!*@!*@!*@!*@!*@!*@!*@!*@!*@!*@!
    *@!*@!*@!hey-bro*@!*@!*@!*@!*@!*@!*@!*@!*@!*@!";
    assert(stripNonAlphaNum(input) == "hey-bro");
}

/*
 * TEST: count_freqs with single character
 */
void nonAlpha_medium_mid(){
    string input = " *((****(***(***h*-i****)***)))";
    assert(stripNonAlphaNum(input) == "h*-i");
}


void test_traverseDirectory() {
    cout << "\nTraversing a directory:" << endl;
    cout << "----------------------------------------" << endl;

    traverseDirectory("a");

    cout << "----------------------------------------" << endl;
    cout << "Verify that all files were printed above." << endl;
}

// ============================================================================
// HASHTABLE UNIT TESTS
// ============================================================================

/*
 * TEST: HashTable constructor creates empty table
 */
void hashTable_constructor_default() {
    HashTable ht;
    assert(ht.isEmpty());
    assert(ht.size() == 0);
}

/*
 * TEST: HashTable constructor with custom capacity
 */
void hashTable_constructor_custom_capacity() {
    HashTable ht(1000);
    assert(ht.isEmpty());
    assert(ht.size() == 0);
    assert(ht.getTableSize() == 1000);
}

/*
 * TEST: Insert single word and verify it's found
 */
void hashTable_insert_single() {
    HashTable ht;
    WordOccurrence occ = {0, 1, 0};
    ht.insert("hello", occ);

    assert(not ht.isEmpty());
    assert(ht.size() == 1);

    bool found = false;
    vector<WordOccurrence> results = ht.searchSens("hello", found);
    assert(found);
    assert(results.size() == 1);
    assert(results[0].fileIndex == 0);
    assert(results[0].lineNumber == 1);
}

/*
 * TEST: Insert multiple occurrences of same word
 */
void hashTable_insert_multiple_occurrences() {
    HashTable ht;
    WordOccurrence occ1 = {0, 1, 0};
    WordOccurrence occ2 = {1, 5, 10};
    WordOccurrence occ3 = {2, 10, 20};

    ht.insert("test", occ1);
    ht.insert("test", occ2);
    ht.insert("test", occ3);

    assert(ht.size() == 1); // Still one unique word

    bool found = false;
    vector<WordOccurrence> results = ht.searchSens("test", found);
    assert(found);
    assert(results.size() == 3);
}

/*
 * TEST: Insert different words
 */
void hashTable_insert_different_words() {
    HashTable ht;
    WordOccurrence occ1 = {0, 1, 0};
    WordOccurrence occ2 = {0, 2, 1};
    WordOccurrence occ3 = {0, 3, 2};

    ht.insert("apple", occ1);
    ht.insert("banana", occ2);
    ht.insert("cherry", occ3);

    assert(ht.size() == 3);

    bool found = false;
    ht.searchSens("apple", found);
    assert(found);

    ht.searchSens("banana", found);
    assert(found);

    ht.searchSens("cherry", found);
    assert(found);
}

/*
 * TEST: Search for non-existent word
 */
void hashTable_search_not_found() {
    HashTable ht;
    WordOccurrence occ = {0, 1, 0};
    ht.insert("hello", occ);

    bool found = false;
    vector<WordOccurrence> results = ht.searchSens("goodbye", found);
    assert(not found);
    assert(results.empty());
}

/*
 * TEST: Case-sensitive search
 */
void hashTable_search_case_sensitive() {
    HashTable ht;
    WordOccurrence occ1 = {0, 1, 0};
    WordOccurrence occ2 = {0, 2, 1};

    ht.insert("Hello", occ1);
    ht.insert("hello", occ2);

    assert(ht.size() == 2); // Different keys

    bool found = false;
    vector<WordOccurrence> results = ht.searchSens("Hello", found);
    assert(found);
    assert(results.size() == 1);

    results = ht.searchSens("hello", found);
    assert(found);
    assert(results.size() == 1);
}

/*
 * TEST: Case-insensitive search
 */
void hashTable_search_case_insensitive() {
    HashTable ht;
    WordOccurrence occ1 = {0, 1, 0};
    WordOccurrence occ2 = {0, 2, 1};
    WordOccurrence occ3 = {0, 3, 2};

    ht.insert("Hello", occ1);
    ht.insert("hello", occ2);
    ht.insert("HELLO", occ3);

    bool found = false;
    vector<WordOccurrence> results = ht.searchInsens("hello", found);
    assert(found);
    assert(results.size() == 3); // Should find all case variations
}

/*
 * TEST: Case-insensitive search with mixed cases
 */
void hashTable_search_insensitive_mixed() {
    HashTable ht;
    WordOccurrence occ1 = {0, 1, 0};
    WordOccurrence occ2 = {1, 5, 10};

    ht.insert("WoRlD", occ1);
    ht.insert("world", occ2);

    bool found = false;
    vector<WordOccurrence> results = ht.searchInsens("WORLD", found);
    assert(found);
    assert(results.size() == 2);
}

/*
 * TEST: Empty table search
 */
void hashTable_search_empty() {
    HashTable ht;

    bool found = false;
    vector<WordOccurrence> results = ht.searchSens("anything", found);
    assert(not found);
    assert(results.empty());
}

/*
 * TEST: Load factor increases with insertions
 */
void hashTable_load_factor() {
    HashTable ht(10);
    WordOccurrence occ = {0, 1, 0};

    double initialLoad = ht.getLoadFactor();
    assert(initialLoad == 0.0);

    ht.insert("word1", occ);
    assert(ht.getLoadFactor() > 0.0);

    ht.insert("word2", occ);
    ht.insert("word3", occ);
    assert(ht.getLoadFactor() == 0.3);
}

/*
 * TEST: Table expansion (stress test with many insertions)
 */
void hashTable_expansion() {
    HashTable ht(10);
    WordOccurrence occ = {0, 1, 0};

    size_t initialSize = ht.getTableSize();

    // Insert enough words to trigger expansion
    for (int i = 0; i < 100; i++) {
        ht.insert("word" + to_string(i), occ);
    }

    assert(ht.size() == 100);
    assert(ht.getTableSize() > initialSize); // Table should have expanded

    // Verify all words are still searchable
    bool found = false;
    for (int i = 0; i < 100; i++) {
        ht.searchSens("word" + to_string(i), found);
        assert(found);
    }
}

/*
 * TEST: Insert and search with special characters
 */
void hashTable_special_characters() {
    HashTable ht;
    WordOccurrence occ = {0, 1, 0};

    ht.insert("can't", occ);
    ht.insert("it's", occ);
    ht.insert("hello-world", occ);

    bool found = false;
    ht.searchSens("can't", found);
    assert(found);

    ht.searchSens("it's", found);
    assert(found);

    ht.searchSens("hello-world", found);
    assert(found);
}

/*
 * TEST: Insert empty string
 */
void hashTable_empty_string() {
    HashTable ht;
    WordOccurrence occ = {0, 1, 0};

    ht.insert("", occ);

    bool found = false;
    vector<WordOccurrence> results = ht.searchSens("", found);
    assert(found);
    assert(results.size() == 1);
}

/*
 * TEST: Multiple occurrences with different file indices
 */
void hashTable_multiple_files() {
    HashTable ht;

    for (size_t i = 0; i < 5; i++) {
        WordOccurrence occ = {i, 10 + i, 100 + i};
        ht.insert("common", occ);
    }

    bool found = false;
    vector<WordOccurrence> results = ht.searchSens("common", found);
    assert(found);
    assert(results.size() == 5);

    // Verify each occurrence has correct file index
    for (size_t i = 0; i < 5; i++) {
        assert(results[i].fileIndex == i);
    }
}

/*
 * TEST: Very long word
 */
void hashTable_long_word() {
    HashTable ht;
    string longWord(1000, 'a'); // 1000 'a's
    WordOccurrence occ = {0, 1, 0};

    ht.insert(longWord, occ);

    bool found = false;
    vector<WordOccurrence> results = ht.searchSens(longWord, found);
    assert(found);
    assert(results.size() == 1);
}

/*
 * TEST: Numbers in words
 */
void hashTable_alphanumeric() {
    HashTable ht;
    WordOccurrence occ = {0, 1, 0};

    ht.insert("test123", occ);
    ht.insert("456word", occ);
    ht.insert("mix3d", occ);

    bool found = false;
    ht.searchSens("test123", found);
    assert(found);

    ht.searchSens("456word", found);
    assert(found);

    ht.searchSens("mix3d", found);
    assert(found);
}