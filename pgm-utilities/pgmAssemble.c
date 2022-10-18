/***********************************/
/* Main Routine                    */
/***********************************/

/* utility header                  */
#include "pgmAssemble.h"

/***********************************/
/* main routine                    */
/*                                 */
/* CLI parameters:                 */
/* argv[0]: executable name        */
/* argv[1]: output file name       */
/* argv[2]: output width           */
/* argv[3]: output height          */
/* arg[...]: row, col, img          */
/* returns 0 on success            */
/* non-zero error code on fail     */
/***********************************/
int main(int argc, char **argv) {
    /* validate args and return any errors */
    int error = validateArgs(argc, argv);
    if (error > 0)
        return error;
    else if (error == -1) {
        /* we want to return 0 after a usage message */
        return 0;
    }
    
    int numSubImages = (argc-4)/3;
    /* create PGMFile struct for final file */
    PGMFile* this_file = (PGMFile*) malloc(sizeof(PGMFile));

    /* create a 2D array of PGMFile structs for subfiles */
    PGMFile** subImages = (PGMFile**) malloc(sizeof(PGMFile*)*numSubImages);
    for (int i = 0; i < numSubImages; i++) {
        /* for each subimage... */
        /* allocate the correct amount of memory */
        subImages[i] = (PGMFile*)malloc(sizeof(PGMFile));
        /* read the appropriate file into the PGMStruct */
        error = readFile(subImages[i], argv[6 + i * 3]);
        if (error > 0)
            return error;
    }

    /* call assemble() to assemble the files and return the outcome code */
    return assemble(this_file, subImages, numSubImages, argv);
}

int validateArgs(int argc, char** argv) {
    /* correct invocation: [ ./pgmAssemble test.pgm 64 64 0 0 tile/output_0_0.pgm 0 32 tile/output_0_1.pgm 32 0 tile/output_1_0.pgm 32 32 tile/output_1_1.pgm ] */
    /* check for no args and print usage message */
    if (argc == 1) {
        printf("Usage: %s outputImage.pgm width height (row column inputImage.pgm)+\n", argv[0]);
        return -1;
    }
    /* check for incorrect args, handle any error and return */
    if (!(argc >= 7 ) || ((argc - 4) % 3 != 0)) {
        return handle(EXIT_BAD_ARG_COUNT, "");
    }
    /* return 0 for valid args */
    return 0;
}

int assemble(PGMFile* this_file, PGMFile** subImages, int numSubImages, char** argv) {
    /* set the height and width according to the command line arguments */
    this_file->height = (int)strtol(argv[3], NULL, 10);
    this_file->width = (int)strtol(argv[2], NULL, 10);

    /* initialise header according to the last subimage */
    initDerivativeStruct(this_file, subImages[numSubImages-1]);
    int ptrRow, ptrCol;

    for (int currentSubImage = 0; currentSubImage < numSubImages; currentSubImage++) {
        /* for every subImage... */
        ptrRow = (int)strtol(argv[4 + (currentSubImage * 3)], NULL, 10);
        ptrCol = (int)strtol(argv[5 + (currentSubImage * 3)], NULL, 10);
        for (int subRow = 0; subRow < subImages[currentSubImage]->height; subRow++) {
            /* for every row of that subImage... */
            for (int subCol = 0; subCol < subImages[currentSubImage]->width; subCol++) {
                /* for every column of that subImage row... */
                /* set the pixel pointed at in this_file->imageData to the pixel pointed at in subImage->imageData */
                this_file->imageData[ptrRow][ptrCol] = subImages[currentSubImage]->imageData[subRow][subCol];
                ptrCol++;
                if (ptrCol == (atoi(argv[5 + (currentSubImage * 3)])) + subImages[currentSubImage]->width) {
                    /* if the col pointer exceeds the width, we know to go to the first column of the next row */
                    ptrCol = (int)strtol(argv[5 + currentSubImage * 3], NULL, 10);
                    ptrRow++;
                }
            }
        }
    }
    /* call writeFile() (using the output filename in argv[1]) and return any errors */
    int error = writeFile(this_file, argv[1]);
    if (error > 0)
        return error;
    /* after success, print the success text and return 0 to indicate success */
    printf("ASSEMBLED\n");
    return EXIT_NO_ERRORS;
}