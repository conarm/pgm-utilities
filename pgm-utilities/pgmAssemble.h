/* library for I/O routines        */
#include <stdio.h>

/* library for memory routines     */
#include <stdlib.h>

/* library for string routines     */
#include <string.h>

/* library for file operations     */
#include "fileOperations.h"

#include "errorHandling.h"

int validateArgs(int argc, char** argv);
int assemble(PGMFile* this_file, PGMFile** subImages, int numSubImages, char** argv);