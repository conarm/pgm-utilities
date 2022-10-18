/* we include pgmlib here since we use PGMFile in our declarations */
#include "pgmlib.h"

/* library for I/O routines        */
#include <stdio.h>

/* library for memory routines     */
#include <stdlib.h>

/* library for string operations   */
#include <string.h>

/* library for error handling      */
#include "errorHandling.h"

void initPGMStruct(PGMFile* this_file);
int initDerivativeStruct(PGMFile* reduced_file, PGMFile* this_file);
int readFile(PGMFile* this_file, char* filename);
int getHeader(PGMFile* this_file, FILE *inputFile, char* filename);
int getMagicNumber(PGMFile* this_file, FILE *inputFile, char* filename);
int getComment(PGMFile* this_file, FILE *inputFile, char* filename);
int getDimensionsMaxGray(PGMFile* this_file, FILE *inputFile, char* filename);
int allocateImageData(PGMFile* this_file);
int getAsciiImageData(PGMFile* this_file, FILE *inputFile, char* filename);
int getBinaryImageData(PGMFile* this_file, FILE *inputFile, char* filename);
int writeHeader (PGMFile* this_file, FILE* output_file, char* output_filename);
int writeFile(PGMFile* this_file, char* output_filename);
int writeAsciiImageData(PGMFile* this_file, FILE *outputFile, char* output_filename);
int writeBinaryImageData(PGMFile* this_file, FILE *outputFile, char* output_filename);
void freeMemory(PGMFile* this_file);
