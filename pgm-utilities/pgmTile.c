/***********************************/
/* Main Routine                    */
/***********************************/

/* library for I/O routines        */
#include <stdio.h>

/* library for memory routines     */
#include <stdlib.h>

/* library for string routines     */
#include <string.h>

/* library for file operations     */
#include "fileOperations.h"

#include "errorHandling.h"

/* utility header                  */
#include "pgmTile.h"

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
    /* create PGMFile structure and initialize variables */
    PGMFile* this_file = (PGMFile*) malloc(sizeof(PGMFile));
    initPGMStruct(this_file);
    
    int factor = (int)strtol(argv[2], NULL, 10);
    /* attempt to read the input into the PGMFile struct and return any errors */
    error = readFile(this_file, argv[1]);
    if (error)
        return error;
    /* call tile() to tile the image and return the outcome code */
    return tile(this_file, argv[1], factor, argv[3]);
}

int validateArgs(int argc, char** argv) {
    /* correct invocation: [ ./pgmTile debug/balls.pgm 4 "output_<row>_<column>.pgm" ] */
    /* check for no args and print usage message */
    if (argc == 1) {
        printf("Usage: %s inputImage.pgm tiling_factor outputImage_<row>_<column>.pgm\n", argv[0]);
        return -1;
    }
    /* check for incorrect args, handle any error and return */
    if (argc != 4) {
        return handle(EXIT_BAD_ARG_COUNT, "");
    }
    /* return 0 for valid args */
    return 0;
}

int tile(PGMFile* this_file, char* input, int factor, char* templateOutput) {
    int subImageHeight = this_file->height/factor, subImageWidth = this_file->width / factor;
    for (int subImageRow = 0; subImageRow < factor; subImageRow++) {
        /* for each row of subimages */
        for (int subImageCol = 0; subImageCol < factor; subImageCol++) {
            /* for each column of subimages */
            int startRow = subImageRow * subImageHeight, startCol = subImageCol * subImageWidth;
            /* create a PGMFile struct of the calculated width and height and derive other variables from the original struct */
            PGMFile* sub_image = (PGMFile*) malloc(sizeof(PGMFile));
            sub_image->height=subImageHeight;
            sub_image->width=subImageWidth;
            initDerivativeStruct(sub_image, this_file);

            int ptrRow = 0, ptrCol = 0;

            for (int imageRow = startRow; imageRow < startRow + subImageHeight; imageRow++) {
                /* for each row of the original image (starting at the calculated start row) until the calculated stop row */
                for (int imageCol = startCol; imageCol < startCol + subImageWidth; imageCol++) {
                    /* for each column of the original image (starting at the calculated start column) until the calculated stop column */
                    /* set the current subimage value to the current image value */
                    sub_image->imageData[ptrRow][ptrCol] = this_file->imageData[imageRow][imageCol];
                    /* increment the column pointer - if OOB then go to the start of the next row */
                    ptrCol++;
                    if (ptrCol == sub_image->width) {
                        ptrCol = 0;
                        ptrRow++;
                    }
                }
            }
            /* store the current row and column numbers as separate strings so that their length can be calculated */
            char rowString[20], colString[20];
            sprintf(rowString, "%d", subImageRow);
            sprintf(colString, "%d", subImageCol);
            /* the final string length is len(original) - 13 (for <row> and <column>) + len(rowNum) + len(colNum) (for the numbers) + 1 (for string terminator) */
            int newLength = strlen(templateOutput) - 13 + strlen(rowString) + strlen(colString) + 1;
            /* create a string of this length and initialise all characters to '.' */
            char finalName[newLength];
            for (int ptr = 0; ptr < newLength; ptr++) {
                finalName[ptr] = '.';
            }

            /* create a string for the output filename and initialise it through getName() */
            char* output_filename = getName(templateOutput, finalName, rowString, colString);
            /* check for a naming failure, e.g. incorrect template and return a misc error */
            if (strcmp(output_filename, "") == 0) {
                return handle(EXIT_MISCELLANEOUS, "");
            }
            /* call writeFile() and return any errors */
            int error = writeFile(sub_image, output_filename);
            if (error)
                return error;
        }
    }
    /* print success message and return 0 since tile completed successfully */
    printf("TILED\n");
    return EXIT_NO_ERRORS;
}

char* getName(char* original, char* finalName, char* row, char* col) {    
    /* get pointers to the first occurences of "<row>" and "column" */
    char* ptrRow = strstr(original, "<row>"); 
    char* ptrCol = strstr(original, "<column>");
    if (ptrRow == NULL || ptrCol == NULL) {
        return "";
    }

    /* work out the positions in the string by subtracting the initial address from these address */
    int positionRow = ptrRow-original, positionCol = ptrCol-original, ptrFinal  = 0;

    for (int ptrOriginal = 0; ptrOriginal <positionRow; ptrOriginal++) {
        /* add the letters up to the first row tag to finalName */
        finalName[ptrFinal] = original[ptrOriginal];
        ptrFinal++;
    }

    for (int ptrRow = 0; ptrRow < strlen(row); ptrRow++) {
        /* for each digit in the current row number, add it to the next index of finalName */
        finalName[ptrFinal] = row[ptrRow];
        ptrFinal++;
    }

    if (positionCol != positionRow + ROW_TAG_LENGTH) {
        /* if there are characters between <row> and <column> */
        for (int ptrOriginal = (positionRow + ROW_TAG_LENGTH); ptrOriginal < positionCol; ptrOriginal++) {
            finalName[ptrFinal] = original[ptrOriginal];
            ptrFinal++;
        }
    }

    for (int ptrCol = 0; ptrCol < strlen(col); ptrCol++) {
        /* for each digit in the current col number, add it to the next index of finalName */
        finalName[ptrFinal] = col[ptrCol];
        ptrFinal++;
    }

    for (int ptrOriginal = positionCol + COL_TAG_LENGTH; ptrOriginal < strlen(original); ptrOriginal++) {
        /* add the remaining characters to finalName */
        finalName[ptrFinal] = original[ptrOriginal];
        ptrFinal++;
    }

    /* add a string terminator and return finalName */
    finalName[ptrFinal]='\0';
    return finalName;
}