#include "myutils.h"

int main(int argc, char *argv[]) {

    if (argc < 7) {
        printf("Less number of arguments.\n");
        printf("./childProgram myDepth parentID startIdx endIdx myDataLen InputFileName\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    int myDepth = strtol(argv[1], NULL, 10);
    int myID = strtol(argv[2], NULL, 10);
    char* strMyID = argv[2];
    int myStartIdx = strtol(argv[3], NULL, 10);
    int myEndIdx = strtol(argv[4], NULL, 10);
    int myDataLen = strtol(argv[5], NULL, 10);
    char* inputFileName = argv[6];

    // TODO: Write child program code here


    return EXIT_SUCCESS;
}