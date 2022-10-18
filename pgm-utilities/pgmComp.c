/***********************************/
/* Main Routine                    */
/***********************************/

/* utility header                  */
#include "pgmComp.h"

/***********************************/
/* main routine                    */
/*                                 */
/* CLI parameters:                 */
/* argv[0]: executable name        */
/* argv[1]: first input file name  */
/* argv[2]: second input file name */
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
    /* create PGMFile struct and initialize variables */
    PGMFile* file1 = (PGMFile*) malloc(sizeof(PGMFile));
    initPGMStruct(file1);

    /* call readFile() to read the first file into the struct and return any errors */
    error = readFile(file1, argv[1]);
    if (error)
        return error;

    /* create PGMFile struct and initialize variables */
    PGMFile* file2 = (PGMFile*) malloc(sizeof(PGMFile));
    initPGMStruct(file2);

    /* call readFile() to read the second file into the other struct and return any errors */
    error = readFile(file2, argv[2]);
    if (error)
        return error;

    /* check if the files are identical */
    /* print appropriate success message and return 0 since echo completed successfully */
    if (identical(file1, file2))
        printf("IDENTICAL\n");
    else
        printf("DIFFERENT\n");
    return EXIT_NO_ERRORS;
}

int validateArgs(int argc, char** argv) {
    /* check for no args and print usage message */
    if (argc == 1) {
        printf("Usage: %s inputImage.pgm inputImage.pgm\n", argv[0]);
        return -1;
    }
    /* check for incorrect args, handle any errors and return */
    if (argc != 3) {
        return handle(EXIT_BAD_ARG_COUNT, "");
    }
    /* return 0 for valid args */
    return 0;
}

int identical(PGMFile* file1, PGMFile* file2) {
    /* compares each of file1's header values against corresponding file2 values */
    /* return false for any conflicts */
    if (file1->width != file2->width) {
	    return 0;
    }
    if (file1->height != file2->height) {
        return 0;
    }
    if (file1->maxGray != file2->maxGray) {
	    return 0;
    }

    /* if we get to this point then width and height are the same, so we can use either file's dimensions to iterate */
    for (int row = 0; row < file1->height; row++) {
        for (int col = 0; col < file1->width; col++) {
            if (file1->imageData[row][col] != file2->imageData[row][col]) {
                /* return false because we have reached a gray value that doesn't equal the corresponding gray value */
                return 0;
            }
        }
    }
    /* return true because files are identical */
    return 1;
}
