/* library for I/O routines        */
#include <stdio.h>

/* module header                   */
#include "errorHandling.h"
int handle(int code, char* text) {
    /* we pass error codes and any relevant information to the error to a switch */
    /* depending on the code, it will print the appropriate error message and return the code again */
    switch (code) {
        case 1:
            printf("ERROR: Bad Argument Count\n");
            break;
        case 2:
            printf("ERROR: Bad File Name (%s)\n", text);
            break;
        case 3:
            printf("ERROR: Bad Magic Number (%s)\n", text);
            break;
        case 4:
            printf("ERROR: Bad Comment Line (%s)\n", text);
            break;
        case 5:
            printf("ERROR: Bad Dimensions (%s)\n", text);
            break;
        case 6:
            printf("ERROR: Bad Max Gray Value (%s)\n", text);
            break;
        case 7:
            printf("ERROR: Image Malloc Failed\n");
            break;
        case 8:
            printf("ERROR: Bad Data (%s)\n", text);
            break;
        case 9:
            printf("ERROR: Output Failed (%s)\n", text);
            break;
        case 10:
            printf("ERROR: Bad Layout\n");
            break;
        case 100:
            printf("ERROR: Miscellaneous (%s)\n", text);
            break;
        default:
            break;
    }
    return code;
}
