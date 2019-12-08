# Hash table and complexity
---
## Overview
This program allows to show the use of an unordered_map and to show its behavior in terms of complexity. To show it, the program uses a file in which it will retrieve the words and then count the number of occurrences of each word using an unordered_map.

### OS Free
This program only uses the 2011 c++ standard, so it can compile and run on any machine with a suitable c++ compiler.

### Easy to use
This program has been designed to be as simple as possible to use, to effectively show the properties of a hash table.

---

## Compilation
### Require
A c++ compiler that manages at least the 2011 standard.

### Compilation script
You can easily compile the project with the `compile.sh`(Linux) and `compile.bat`(Windows) files that use g++.<br/>
The built executable will be in the `bin` directory with name `main.exe`

---

## Getting Started
If you run the program without parameters, a help message will be displayed.

### Commands
The possible commands are:

+ `-c` or `--complexity` which performs the insertion and search measurement in an unordered_map.
+ `-s` or `--search` which asks the user for a word to search in the unordered_map beforehand filled in.
<br/>
<br/>

#### Complexity
For the **complexity** command the parameters to be given to the program are:

+ `<path_to_input_file>` - The path to the input file that will fill the hash map with words.
+ `<path_to_output_file>` - The path to the output file that will be filled with the values output by the program.
+ `<nb_test>` - The number of times Hash Map on which the complexity test will be performed. (The result will be an average of these times)

#### Search
For the **search** command the parameters to be given to the program are:

+ `<path_to_input_file>` - The path to the input file that will fill the hash map with words.
