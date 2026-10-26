# Gerp

A grep-style search tool written in C++. Gerp indexes every word in every file under a directory, then answers search queries interactively, printing each line where the word appears along with its file path and line number.

Built for CS 15 (Data Structures) at Tufts University by Hansini Gundavarapu and Jessica Charalel.

## How it works

- **Indexing:** Gerp walks the full directory tree, reads each file line by line, and stores every word in a custom hash table.
- **Hash table:** Implemented from scratch using open addressing with linear probing, so lookups stay fast even on large directories.
- **Queries:** Searches are case sensitive by default, with a case insensitive mode available.

## Build and run

```bash
cd ProjectGerp
make
./gerp <inputDirectory> <outputFile>
```

Then type queries at the `Query?` prompt:

| Command | What it does |
| --- | --- |
| `word` | Case sensitive search for `word` |
| `@i word` or `@insensitive word` | Case insensitive search |
| `@f newOutputFile` | Send results to a different output file |
| `@q` or `@quit` | Quit |

## Files

| File | Purpose |
| --- | --- |
| `main.cpp` | Reads command line arguments, builds the index, starts the query loop |
| `gerp.cpp`, `gerp.h` | Index building and query handling |
| `HashTable.cpp`, `HashTable.h` | Custom hash table (open addressing, linear probing) |
| `processing.cpp`, `processing.h` | Directory traversal and string cleanup helpers |
| `unit_tests.h` | Unit tests |
| `b.txt`, `c.txt`, `d.txt`, `y` | Sample files used for testing |
| `README` | Original course write-up with design notes |
