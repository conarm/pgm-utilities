/***********************************/
/* Main Routine                    */
/***********************************/

/* utility header                  */
#include "pgmb2a.h"

/***********************************/
/* main routine                    */
/*                                 */
/* CLI parameters:                 */
/* argv[0]: executable name        */
/* argv[1]: input file name        */
/* argv[2]: output file name       */
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
    PGMFile* this_file = (PGMFile*) malloc(sizeof(PGMFile));
    initPGMStruct(this_file);

    /* call readFile() to read the file into the struct and return any errors */
    error = readFile(this_file, argv[1]);
    if (error)
        return error;
    unsigned short *magic_Number = (unsigned short *) this_file->magic_number;
    if (*magic_Number != MAGIC_NUMBER_RAW_PGM) {
        /* since this is b2a, we only want to accept binary pgm files */
        return handle(EXIT_BAD_MAGIC_NUMBER, argv[1]);
    }

    /* change the magic number from P5 to P2 and call writeFile() */
    /* writeFile() will see the P2 and echo the struct in ASCII */
    this_file->magic_number[1] = '2';
    error = writeFile(this_file, argv[2]);
    if (error)
        return error;

    /* print success message and return 0 since echo completed successfully */
    printf("CONVERTED\n");
    return 0;        
}

int validateArgs(int argc, char** argv) {
    /* check for no args and print usage message */
    if (argc == 1) {
        printf("Usage: %s inputImage.pgm outputImage.pgm\n", argv[0]);
        return -1;
    }
    /* check for incorrect args, handle any error and return */
    if (argc != 3) {
        return handle(EXIT_BAD_ARG_COUNT, "");
    }
    /* return 0 for valid args */
    return 0;
}