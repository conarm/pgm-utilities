/***********************************/
/* Main Routine                    */
/***********************************/

/* utility header                  */
#include "pgmEcho.h"

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

    /* call echo() to echo the files and return the outcome code */
    return echo(this_file, argv[1], argv[2]);
}

int validateArgs(int argc, char** argv) {
    /* check for no args and print usage message */
    if (argc == 1) {
        printf("Usage: %s outputImage.pgm width height (row column inputImage.pgm)\n", argv[0]);
        return -1;
    }
    /* check for incorrect args, handle any error and return */
    if (argc != 3) {
        return handle(EXIT_BAD_ARG_COUNT, "");
    }
    /* return 0 for valid args */
    return 0;
}

int echo(PGMFile* this_file, char* input, char* output) {
    /* call readFile() and return any errors */
    int error = readFile(this_file, input);
    if (error)
        return error;

    /* call writeFile() and return any errors */
    error = writeFile(this_file, output);
    if (error)
        return error;

    /* print success message and return 0 since echo completed successfully */
    printf("ECHOED\n");
    return EXIT_NO_ERRORS;
}
