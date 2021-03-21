Nume: Olaru Gabriel Iulian
Grupă: 334CC

# Assignment 1 SO PREPROCESSOR

General Structure
-

***Code structure***
1. The goal of the project is to implement a multiplatform, minimalistic version of the GCC Preprocessor.
2. The logic is devided across multiple source files.

* ├── GNUmakefile
* ├── LICENSE
* ├── Makefile
* ├── README.md
* └── src
    * ├── hash_map.c
    * ├── hash_map.h
    * ├── main.c
    * ├── utils.c
    * └── utils.h

***Flow***

First, all command line parameters are interpreted.

Then, the input file are being processed.
* If there were no input file, the programs takes input from stdin.
* If there were input files:
    * The prgrams tries to open them. If oane fails, it will return error.
    * After an input file is opened, it is processed line by line.
        * If it is a directive (starts with #), then a specific handler in called
              A description for each directive is provided in the bales below.
        * If it is a normal line, all defined macros are replaced and it is added to the final result.

Lastly, the output file is opened, and the result of the previous step is printed to it.
* If there was no output file, the program will out to stdout.
* If there was an output file (received via -o or as parameter) it will be opened.


***Efficency***

The implementation is efficient since it limits the number of strcpy calls, because,
whenever possible, pointers are linked to the memory region reather that creating a new
string and all replaces are done in-place. The map is also implemented as an array of pointers
for the same reasom - more on this below.

***Corner cases***

There is no clear specification for the case that the output file is given as a parameter.
To distinguish between it and the input file, the extension is checker. Also, the second mentioned
file is treated as output.


Implementation Details
-

All the required features were Implemented.

***The Definition Table***
* The hash map is implemented as an array of addresses of entries.
* Each entry has a key/ value pair and an associated hash code.
* If two entries have the same key, the value of the old entry is updated.
* This implementation provides lookup in O(1) and insertion in O(1) (+ overhead generating by the hash function and the memory allocation ¯\\_(ツ)_/¯ )

***Supported command line arguments:***
Flags | Parameter Meaning
------------ | -------------
-D<blank_space> / -D | When encountered, adds a definition to the deffinitions table
-I<blank_space> / -I | When encountered, adds the path to a directory to a table that stores all possible included paths.
<no flag> | When no flag preceeds the parameter, it will be treated as either an input or output file.

***Supported directives:***
Directive | Features
------------ | -------------
#define | The defined macro is broken intro a key / value pair and inserted in the map
#if <COND> / #elif <COND> / #else / #endif | The if condition is evaluated, and a true/ false branch is started. On the true branch, process every line and include it in the final result. On the false branch, process the lines untill else / elif is found. Evaluate the condition for elif and repeat the process. Just skip lines for else
#ifdef <SYMBOL> / #ifndef <SYMBOL> / #else / #endif | The if condition is evaluated by checking into the definitions map, and a true/ false branch is started. On the true branch, process every line and include it in the final result. On the false branch, process the lines untill else / elif is found. Evaluate the condition for elif and repeat the process. Just skip lines for else
 #include “HEADER” | Checks for the specific file in the same path as the input file. If not found, checks in all paths given as parameters (with -I). If the file is valid, it is processed.

Difficulties:
  * had to be really careful for failed mallocs and failed open calls.

Fun Facts:
  * C89 sucks.
  * VSCode has a nice feature for turning spaces to tabs
  * On windows, \r\n characters are not supported :(

How to run?
-

Three object files are linked:
  * hash_map.o -> the implementation of the hash map
  * utils.o -> contains implementation of all directive handles and other useful functions
  * main.o -> contains the main function, whitch controls the flow of the program

***To build***

The building process is not platform dependant, because of the two Makefiles
  * Makefile - for windows --> just type nmake
  * GNUmakefle - for linux --> just type make
 
***To run***

so-cpp  -D SYMBOL=MAPPING -I DIR INFILE -o OUTFILE
  
Bibliography
-

* Useful Links
    * For a hash function for strings: http://www.cse.yorku.ca/~oz/hash.html
    * For the Makefiles: https://ocw.cs.pub.ro/courses/so/laboratoare/laborator-01

Git
-

1. https://github.com/IulianOlaru249/SO_2020_2021_Homework_1/
