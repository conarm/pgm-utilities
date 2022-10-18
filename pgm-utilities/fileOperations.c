/* module header */
#include "fileOperations.h"

/**********************************************************************************/
/* struct initialisation                                                          */
/**********************************************************************************/

void initPGMStruct(PGMFile* this_file) {
    /* initializes all of the values in the PGMStruct */
    this_file->magic_number[0] = '0';
    this_file->magic_number[1] = '0';
    this_file->commentLine = NULL;
    this_file->width = 0;
    this_file->height = 0;
    this_file->maxGray = 255;
    this_file->imageData = NULL;
}

int initDerivativeStruct(PGMFile* reduced_file, PGMFile* this_file) {
    /* initializes all of the values in the PGMStruct, deriving them from another */
    reduced_file->magic_number[0] = this_file->magic_number[0];
    reduced_file->magic_number[1] = this_file->magic_number[1];
    reduced_file->commentLine = NULL;
    reduced_file->maxGray = this_file->maxGray;

    return allocateImageData(reduced_file);
}

int allocateImageData(PGMFile* this_file) {
    long rowPointerBytes = this_file->height * sizeof(unsigned char**);
    long colPointerBytes;

    if (this_file->magic_number[1] == '2') {
        /* if we are dealing with a P2 (ASCII) file... */
        /* get the number of bytes and allocate imageData accordingly */
        colPointerBytes = this_file->width * sizeof(unsigned char);
        this_file->imageData = (unsigned char **) malloc(rowPointerBytes);
        if (this_file->imageData == NULL) {
            freeMemory(this_file);
            //fclose(inputFile);
            return handle(EXIT_MALLOC_FAILED, "");
        }
        for (int row = 0; row < this_file->height; row++) {
            this_file->imageData[row] = (unsigned char*) malloc(colPointerBytes);
            if (this_file->imageData[row] == NULL) {
                freeMemory(this_file);
                //fclose(inputFile);
                return handle(EXIT_MALLOC_FAILED, "");
            }
        }
    }
    else {
        /* if we are dealing with a P5 (BINARY) file... */
        /* get the number of bytes and allocate imageData accordingly */
        colPointerBytes = this_file->width;
        this_file->imageData = (unsigned char **) malloc(rowPointerBytes);
        if (this_file->imageData == NULL) {
            freeMemory(this_file);
            //fclose(inputFile);
            return handle(EXIT_MALLOC_FAILED, "");
        }
        for (int row = 0; row < this_file->height; row++) {
            this_file->imageData[row] = (unsigned char*) malloc(colPointerBytes);
            if (this_file->imageData[row] == NULL) {
                freeMemory(this_file);
                //fclose(inputFile);
                return handle(EXIT_MALLOC_FAILED, "");
            }
        }
    }
    return 0;
}

/**********************************************************************************/
/* read operations                                                                */
/**********************************************************************************/

int readFile(PGMFile* this_file, char* filename) {
    /* start reading in the data         */
    FILE *inputFile = fopen(filename, "r");
    if (inputFile == NULL)
        return handle(EXIT_BAD_FILE_NAME, filename);
    
    /* try to get the header using getHeader. return any errors */
    int error = getHeader(this_file, inputFile, filename);
    if (error)
        return error;

    /* allocate the data pointer and ensure we know the correct amount of bytes in imageData */
    unsigned short *magic_Number = (unsigned short *) this_file->magic_number;
    error = allocateImageData(this_file);
    if (error) {
        fclose(inputFile);
        return error;
    }
    /* ----- read imageData ----- */

    if (*magic_Number == MAGIC_NUMBER_ASCII_PGM) {
        /* if we are dealing with a P2 (ASCII) file... */
        /* attempt to run getAsciiImageData to fill this_file->imageData with data */
        /* return any errors */
        error = getAsciiImageData(this_file, inputFile, filename);
        if (error)
            return error;
    }
    else {
        /* if we are dealing with a P5 (BINARY) file... */
        /* attempt to run getBinaryImageData to fill this_file->imageData with data */
        /* return any errors */
        error = getBinaryImageData(this_file, inputFile, filename);
        if (error)
            return error;
    }
    /* close file and return 0 for success */
    fclose(inputFile);
    return 0;
}

int getHeader(PGMFile* this_file, FILE *inputFile, char* filename) {
    /* read in the magic number */
    int error = getMagicNumber(this_file, inputFile, filename);
    if (error) {
        return error;
    }

    /* scan whitespace if present */
    fscanf(inputFile, " ");

    /* check for a comment line */
    error = getComment(this_file, inputFile, filename);
    if (error) {
        return error;
    }

    /* read in width, height, grays */
    /* whitespace to skip blanks */
    error = getDimensionsMaxGray(this_file, inputFile, filename);
    if (error) {
        return error;
    }
    return 0;
}

int getMagicNumber(PGMFile* this_file, FILE *inputFile, char* filename) {
    /* read in the magic number */
    unsigned short *magic_Number = (unsigned short *) this_file->magic_number;
    this_file->magic_number[0] = getc(inputFile);
    this_file->magic_number[1] = getc(inputFile);
    /* validate the number, returning any errors and freeing allocated memory */
    if ((*magic_Number != MAGIC_NUMBER_ASCII_PGM) && (*magic_Number != MAGIC_NUMBER_RAW_PGM)) {
        fclose(inputFile);
        return handle(EXIT_BAD_MAGIC_NUMBER, filename);
    }
    return 0;
}

int getComment(PGMFile* this_file, FILE *inputFile, char* filename) {
    /* check for a comment line */
    char nextChar = fgetc(inputFile);
    if (nextChar == '#') {
        /* if there is a comment line, store it in this_file */
        this_file->commentLine = (char *) malloc(MAX_COMMENT_LINE_LENGTH);
        char *commentString = fgets(this_file->commentLine, MAX_COMMENT_LINE_LENGTH+1, inputFile);
        /* validate the comment, freeing allocated memory if necessary and returning any errors */
        if (commentString == NULL || strlen(commentString) == MAX_COMMENT_LINE_LENGTH) {
            freeMemory(this_file);
            fclose(inputFile);
            return handle(EXIT_BAD_COMMENT, filename);
        }
    }
    else {
        /* if not return the character to the stream since we will need it */
        ungetc(nextChar, inputFile);
    }
    return 0;
}

int getDimensionsMaxGray(PGMFile* this_file, FILE *inputFile, char* filename) {
    /* read in width, height, grays */
    /* whitespace to skip blanks */
    int scanCount = fscanf(inputFile, " %u %u %u ", &(this_file->width), &(this_file->height), &(this_file->maxGray));

    /* if we didn't scan the right amount of variables OR */
    /* if width is smaller than the minimum (1) OR */
    /* if width is bigger than or equal to the maximum (65536) OR */
    /* if height is smaller than the minimum OR */
    /* if height is bigger than or equal to the maximum */
    if  ((scanCount != 3				)	||
        (this_file->width 	< MIN_IMAGE_DIMENSION	) 	||
        (this_file->width 	>= MAX_IMAGE_DIMENSION	) 	||
        (this_file->height < MIN_IMAGE_DIMENSION	) 	||
        (this_file->height >= MAX_IMAGE_DIMENSION	)) {
        /* if the dimensions are invalid compared to min and max */
        /* free allocated memory */ 
        freeMemory(this_file);
        /* close stream */ 
        fclose(inputFile);
        /*return the handled error */
        return handle(EXIT_BAD_DIMENSIONS, filename);
    }
    if (this_file->maxGray != 255) {
        /* if maxGray is not 255 */
        /* free allocated memory */ 
        free(this_file->commentLine);
        /* close stream */ 
        fclose(inputFile);
        /*return the handled error */
        return handle(EXIT_BAD_MAX_GRAY, filename);
    }
    return 0;
}

int getAsciiImageData(PGMFile* this_file, FILE *inputFile, char* filename) {
    int grayValue = -1;
    for (int row = 0; row < this_file->height; row++) {
        /* for every row... */
        for (int col = 0; col < this_file->width; col++) {
            /* for every column... */
            /* reset grayValue */
            grayValue = -1;
            /* scan the next piece of data into grayValue */
            int scanCount = fscanf(inputFile, " %u", &grayValue);
            
            if ((scanCount != 1) || (grayValue < 0) || (grayValue > 255)) {
                freeMemory(this_file);
                fclose(inputFile);
                return handle(EXIT_BAD_DATA, filename);
            }
            /* set the current pixel (imageData[i][j]) equal to grayValue */
            this_file->imageData[row][col] = grayValue;
        }
    }
    /* at this point we have stored the file according to the width and height in the header */
    /* there may be more data in the .pgm that we haven't scanned, which should return a data error */
    int scanCount = fscanf(inputFile, " %u", &grayValue);
    if ((scanCount == 1)) { /* fscanf failed */
        freeMemory(this_file);
        fclose(inputFile);
        return handle(EXIT_BAD_DATA, filename);
    }

    return 0;
}

int getBinaryImageData(PGMFile* this_file, FILE *inputFile, char* filename) {
    long nImageBytes = this_file->width * this_file->height;
    unsigned char* dataString = NULL;
    dataString = (unsigned char*) malloc(nImageBytes);
    /* read the data into dataString */
    int scanCount = fread(dataString, 1, nImageBytes, inputFile);
    if ((scanCount != nImageBytes)) { /* fscanf failed */
        freeMemory(this_file);
        free(dataString);
        fclose(inputFile);
        return handle(EXIT_BAD_DATA, filename);
    }
    /* split up the data into the this_file->imageData 2D array */
    int count = 0;
    for (int row = 0; row < this_file->height; row++) {
        for(int col = 0; col < this_file->width; col++) {
            this_file->imageData[row][col] = dataString[count];
            count++;
        }
    }

    /* at this point we have stored the file according to the width and height in the header */
    /* there may be more data in the .pgm that we haven't scanned, which should return a data error */
    int grayValue = -1;
    /* scan it into grayValue */
    scanCount = fscanf(inputFile, " %u", &grayValue);
    if ((scanCount == 1)) { /* fscanf failed */
        freeMemory(this_file);
        free(dataString);
        fclose(inputFile);
        return handle(EXIT_BAD_DATA, filename);
    }
    return 0;
}

/**********************************************************************************/
/* write operations                                                               */
/**********************************************************************************/

int writeFile(PGMFile* this_file, char* output_filename) {
    /* open a write stream for the output, returning any errors and freeing allocated data */
    FILE *outputFile = fopen(output_filename, "w");
    if (outputFile == NULL) {
        freeMemory(this_file);
        return handle(EXIT_BAD_FILE_NAME, output_filename);
    }

    /* try to write the header using writeHeader. return any errors */
    int error = writeHeader(this_file, outputFile, output_filename);
    if (error)
        return error;
    unsigned short *magic_Number = (unsigned short *) this_file->magic_number;

    /* ----- write imageData ----- */

    if (*magic_Number == MAGIC_NUMBER_ASCII_PGM) {
        /* if we are dealing with a P2 (ASCII) file... */
        /* attempt to run writeAsciiData to write imageData to the file as ASCII */
        /* return any errors */
        error = writeAsciiImageData(this_file, outputFile, output_filename);
        if (error)
            return error;
    }
    else {
        /* if  we are dealing with a P5 (BINARY) file... */
        /* attempt to run writeBinaryData to write imageData to the file as binary */
        /* return any errors */
        error = writeBinaryImageData(this_file, outputFile, output_filename);
        if (error)
            return error;
    }
    fclose(outputFile);
    /* at this point, we are done and can exit with a success code */
    return EXIT_NO_ERRORS;
}

int writeHeader (PGMFile* this_file, FILE* outputFile, char* output_filename) {
    /* write magic number, size & gray value */
    size_t nBytesWritten = fprintf(outputFile, "%c%c\n%d %d\n%d\n", this_file->magic_number[0], this_file->magic_number[1], this_file->width, this_file->height, this_file->maxGray);
    
    if (nBytesWritten < 0) {
        /* if we didn't write anything, free allocated data and return the handled error */
        freeMemory(this_file);
        return handle(EXIT_OUTPUT_FAILED, output_filename);
    }
    return 0;
}

int writeAsciiImageData(PGMFile* this_file, FILE *outputFile, char* output_filename) {
    int nextCol, nBytesWritten;
    for (int row = 0; row < this_file->height; row++) {
        /* for each row */
        for (int col = 0; col < this_file->width; col++) {
            /* for each column */
            if (col+1 == this_file->width) {
                /* if this IS NOT the final column, i.e there are columns after this... */
                /* set nextCol to false */
                nextCol = 0;
            }
            else {
                /* otherwise, set nextCol to true */
                nextCol = 1;
            }
            nBytesWritten = fprintf(outputFile, "%d%c", this_file->imageData[row][col], (nextCol ? ' ' : '\n') );
            if (nBytesWritten < 0) {
                freeMemory(this_file);
                return handle(EXIT_OUTPUT_FAILED, output_filename);
            }
        }
    }
    return 0;
}

int writeBinaryImageData(PGMFile* this_file, FILE *outputFile, char* output_filename) {
    unsigned char* grayValue;
    int nBytesWritten;
    for (int row = 0; row < this_file->height; row++) {
        /* for each row */
        for (int col = 0; col < this_file->width; col++) {
            /* for each column */
            /* set grayValue to the gray value in this location of imageData */
            grayValue = &this_file->imageData[row][col];
            /* write this value to the file as a binary value */
            nBytesWritten = fwrite(grayValue, 1, 1, outputFile);
        }
    }
    /* if we didn't write anything, free allocated data and return the handled error */
    if (nBytesWritten < 0) {
            freeMemory(this_file);
            return handle(EXIT_OUTPUT_FAILED, output_filename);
    }
    return 0;
}

/**********************************************************************************/
/* clean-up                                                                       */
/**********************************************************************************/

void freeMemory(PGMFile* this_file) {
    if (this_file->commentLine != NULL)
        free(this_file->commentLine);
    if (this_file->imageData != NULL) {
        for (int row = 0; row < this_file->width; row++) {
            free(this_file->imageData[row]);
        }
        free(this_file->imageData);
    }
}