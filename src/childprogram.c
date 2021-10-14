#include "myutils.h"

int main(int argc, char *argv[]) {
    extern int NUM_CHILD_PRGRM_ARGS;
    
    if (argc < NUM_CHILD_PRGRM_ARGS-1) {
        printf("Less number of arguments.\n");
        printf("./childProgram myDepth parentID startIdx endIdx myDataLen InputFileName\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    // parse arguments into useable data
    int curDepth = strtol(argv[1], NULL, 10);
    int id = strtol(argv[2], NULL, 10);
    char* strId = argv[2];
    int startIdx = strtol(argv[3], NULL, 10);
    int endIdx = strtol(argv[4], NULL, 10);
    int curNData = strtol(argv[5], NULL, 10);
    char* inputFileName = argv[6];

    FILE * fp;                                                     
    if ((fp = getFilePointer(inputFileName)) == NULL) {             // Open a file and return file pointer to the file
        exit(EXIT_FAILURE);
    }

    int masterNData;
    int masterDepth;
    if (getFileAttributes(fp, &masterNData, &masterDepth) == -1) {
        exit(EXIT_FAILURE);
    }

    int degrees[masterDepth];
    if (getDegreesFromFile(fp, 
    degrees, masterDepth) == -1) {
        exit(EXIT_FAILURE);
    }

    if (curDepth < masterDepth) {
        // this is an internal node - continue dividing data into child processes
        // childIds are stored so they do not have to be regenerating while merging
        char* childIds[degrees[curDepth]];

        // data for log
        char strCurrDepth[LineBufferSize];
        sprintf(strCurrDepth, "%d", curDepth);

        // common data setup for childProgram arguments
        char program[] = "childProgram";
        char programPath[] = "childProgram";
        char strDepth[LineBufferSize];
        sprintf(strDepth, "%d", curDepth + 1);

        pid_t pid;
        int dataPerProcess = curNData/degrees[curDepth];
        int childId = id * 10;

        for (int i = 0; i < degrees[curDepth]; i++) {
            childId += 1;
            // generate child string
            childIds[i] = (char *) malloc(sizeof(char) * LineBufferSize);
            sprintf(childIds[i], "%d", childId);

            pid = fork();

            if (pid == 0) {
            // child process sets up for executing childProgram
                if (i == degrees[curDepth] - 1) {
                    // the final node in the child set gets all of the remaining data
                    curNData = curNData - (degrees[curDepth] - 1) * dataPerProcess;
                    startIdx = endIdx-(curNData-1);
                } else {
                    curNData = dataPerProcess;
                    startIdx += i * dataPerProcess;
                    endIdx = startIdx + (dataPerProcess-1);
                }
        
                // Setup individual data for child program
                char strStartIdx[LineBufferSize], strEndIdx[LineBufferSize], strNData[LineBufferSize];

                sprintf(strStartIdx, "%d", startIdx);
                sprintf(strEndIdx, "%d", endIdx);
                sprintf(strNData, "%d", curNData);

                // Log spawning of child
                printf("Parent [%d] - Spawn Child [%s, %s, %s, %s, %s]\n", id, strDepth, childIds[i], strStartIdx, strEndIdx, strNData);

                if (execl(programPath, program, strDepth, childIds[i], strStartIdx, strEndIdx, strNData, inputFileName, NULL) == -1) {
                    fprintf(stderr, "ERROR: Failed to exec child program.");
                    freeStringArray(childIds, degrees[curDepth]);
                    fclose(fp);
                    exit(EXIT_FAILURE);
                }
            }
        }
        for (int i = 0; i < degrees[curDepth]; i++) {
            wait(NULL);
        }

        // merge sort and make a new output file
        if (merge(strId, childIds, curDepth, degrees[curDepth]) == 1) {
            freeStringArray(childIds, degrees[curDepth]);
        }

    } else {
        // this is a leaf node
        int input[curNData];
        if (getFileInput(fp, input, startIdx, endIdx) == -1) {
            exit(EXIT_FAILURE);
        }
        quickSort(input, 0, curNData-1);
        writeSortedResultToFile(strId, input, curNData);
        printf("Process [%s] - Quick Sort - Done\n", strId);
    }
    
    fclose(fp);

    return EXIT_SUCCESS;
}