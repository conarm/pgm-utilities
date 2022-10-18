/* definitions for all possible error codes for readability */
#define EXIT_NO_ERRORS 0
#define EXIT_BAD_ARG_COUNT 1
#define EXIT_BAD_FILE_NAME 2
#define EXIT_BAD_MAGIC_NUMBER 3
#define EXIT_BAD_COMMENT 4
#define EXIT_BAD_DIMENSIONS 5
#define EXIT_BAD_MAX_GRAY 6
#define EXIT_MALLOC_FAILED 7
#define EXIT_BAD_DATA 8
#define EXIT_OUTPUT_FAILED 9
#define EXIT_BAD_LAYOUT 10
#define EXIT_MISCELLANEOUS 100

/* other definitions for validation */
#define MAGIC_NUMBER_RAW_PGM 0x3550
#define MAGIC_NUMBER_ASCII_PGM 0x3250
#define MIN_IMAGE_DIMENSION 1
#define MAX_IMAGE_DIMENSION 65536
#define MAX_COMMENT_LINE_LENGTH 128

/* PGMFile is defined here so that we can manipulate pgm data in a standardised way between modules and utilities */
typedef struct _pgmFile {
    unsigned char magic_number[2];
    char* commentLine;
    unsigned int width;
    unsigned int height;
    unsigned maxGray;
    unsigned char** imageData;
} PGMFile;