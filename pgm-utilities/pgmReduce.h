/* library for I/O routines           */
#include <stdio.h>

/* library for numerical factor check */
#include <ctype.h>

/* library for string operations      */
#include <string.h>

/* library for memory routines        */
#include <stdlib.h>

/* library for file operations        */
#include "fileOperations.h"

/* library for error handling         */
#include "errorHandling.h"

int validateArgs(int argc, char** argv);
int initReduceStruct(PGMFile* reduced_file, PGMFile* this_file, int factor);
int reduceFile(PGMFile* this_file, int factor, PGMFile* reduced_file);