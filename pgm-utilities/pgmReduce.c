/* utility header                     */
#include "pgmReduce.h"

/**************************************/
/* main routine                       */
/*                                    */
/* CLI parameters:                    */
/* argv[0]: executable name           */
/* argv[1]: input file name           */
/* argv[2]: factor                    */
/* argv[3]: output file name          */
/* returns 0 on success               */
/* non-zero error code on fail        */
/**************************************/
int main(int argc, char **argv) {
    /* validate args and return any errors */
    int error = validateArgs(argc, argv);
    if (error > 0)
        return error;
    else if (error == -1) {
        /* we want to return 0 after a usage message */
        return 0;
    }
    /* create PGMFile struct and initialize variables */
    PGMFile* this_file = (PGMFile*) malloc(sizeof(PGMFile));
    initPGMStruct(this_file);
    int factor = strtol(argv[2], NULL, 10);

    /* call readFile() to read the file into the struct and return any errors */
    error = readFile(this_file, argv[1]);
    if (error)
        return error;
        
    /* create a PGMFile struct for the reduced file and initialise it */
    PGMFile* reduced_file = (PGMFile*) malloc(sizeof(PGMFile));
    initReduceStruct(reduced_file, this_file, factor);
    
    /* attempt to reduce the PGMFile to an output file and return any errors */
    error = reduceFile(this_file, factor, reduced_file);
    if (error)
        return error;
    /* write this new file to the output, returning any errors or 0 if successful */
    error = writeFile(reduced_file, argv[3]);
    if (error)
        return error;
    /* print success message and return 0 since echo completed successfully */
    printf("REDUCED\n");
    return EXIT_NO_ERRORS;
}

int validateArgs(int argc, char** argv) {
    /* check for no args and print usage message */
    if (argc == 1) {
        printf("Usage: %s inputImage.pgm reduction_factor outputImage.pgm\n", argv[0]);
        return -1;
    }
    /* incorrect arg num check */
    if (argc != 4) {
        return handle(EXIT_BAD_ARG_COUNT, "");
    }
    /* numerical factor check */
    for (int i = 0; i < strlen(argv[2]); i++)
    {
        if (!isdigit(argv[2][i]))
            return handle(EXIT_MISCELLANEOUS, "");
    }
    /* negative factor check */
    if (atoi(argv[2]) < 0)
        return handle(EXIT_MISCELLANEOUS, "");
    /* return 0 for valid args */
    return 0;
}

int initReduceStruct(PGMFile* reduced_file, PGMFile* this_file, int factor) {
    /* give the new struct the width value of roundUp(width / factor) */
    reduced_file->width = (this_file->width - 1) / factor + 1;
    /* give the new struct the width value of roundUp(height / factor) */
    reduced_file->height = (this_file->height - 1) / factor + 1;
    /* derive values from the original struct */
    int error = initDerivativeStruct(reduced_file, this_file);
    if (error)
        return error;
    return 0;
}

int reduceFile(PGMFile* this_file, int factor, PGMFile* reduced_file) {
    /* initialise row and column pointers to 0 for the reduced pgmStruct */
    int ptrRow=0, ptrCol=0;

    for (int row = 0; row < this_file->height; row++) {
        /* for every row of the original pgmStruct's imageData */
        for (int col = 0; col < this_file->width; col++) {
            /* for every column  of the original pgmStruct's imageData */
            if ((row % factor == 0) && (col % factor == 0)) {
                /* if we are at a desired pixel... */
                /* take this pixel and put it in the reduced pgmStruct's imageData */
                reduced_file->imageData[ptrRow][ptrCol] = this_file->imageData[row][col];

                /* increment the reduced pgmStruct column pointer - if OOB then go to the start of the next row */
                ptrCol++;
                if (ptrCol == reduced_file->width) {
                    ptrCol = 0;
                    ptrRow++;
                }
            }
        }
    }
    return 0;
}