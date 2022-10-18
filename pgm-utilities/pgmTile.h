/* contants for readability */
#define ROW_TAG_LENGTH 5
#define COL_TAG_LENGTH 8

int validateArgs(int argc, char** argv);
int tile(PGMFile* this_file, char* input, int factor, char* templateOutput);
char* getName(char* original, char* finalName, char* row, char* col);