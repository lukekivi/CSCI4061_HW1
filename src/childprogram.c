#include "myutils.h"

int main(int argc, char *argv[]) {
    extern int NUM_CHILD_PRGRM_ARGS;
    
    if (argc < NUM_CHILD_PRGRM_ARGS-1) {
        printf("Less number of arguments.\n");
        printf("./childProgram myDepth parentID startIdx endIdx myDataLen InputFileName\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

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
        // this is an internal node
        char* childIds[degrees[curDepth]];

        pid_t pid;
        int dataPerProcess = curNData/degrees[curDepth];

        char program[] = "childProgram";
        char programPath[] = "childProgram";
        char strDepth[LineBufferSize];
        sprintf(strDepth, "%d", curDepth + 1);

        int childId = id * 10;

        for (int i = 0; i < degrees[curDepth]; i++) {
            childId += 1;
            char tempChildId[LineBufferSize];
            sprintf(tempChildId, "%d", childId);
            childIds[i] = tempChildId;
            
    
            pid = fork();

            if (pid == 0) {
                
                if (i == degrees[curDepth] - 1) {
                    curNData = curNData - (degrees[curDepth] - 1) * dataPerProcess;
                    startIdx = endIdx-(curNData-1);
                } else {
                    curNData = dataPerProcess;
                    startIdx += i * dataPerProcess;
                    endIdx = startIdx + (dataPerProcess-1);
                }
        
                
                char strStartIdx[LineBufferSize], strEndIdx[LineBufferSize], strNData[LineBufferSize];

                sprintf(strStartIdx, "%d", startIdx);
                sprintf(strEndIdx, "%d", endIdx);
                sprintf(strNData, "%d", curNData);

                char strCurrDepth[LineBufferSize];
                sprintf(strCurrDepth, "%d", curDepth);

                // Log spawning of child
                printf("Parent [%d] - Spawn Child [%s, %s, %s, %s, %s]\n", id, strDepth, childIds[i], strStartIdx, strEndIdx, strNData);

                if (execl(programPath, program, strDepth, childIds[i], strStartIdx, strEndIdx, strNData, inputFileName, NULL) == -1) {
                    fprintf(stderr, "ERROR: Failed to exec child program.");
                    free(degrees);
                    fclose(fp);
                    exit(EXIT_FAILURE);
                }
            }
        }
        for (int i = 0; i < degrees[curDepth]; i++) {
            wait(NULL);
        }

        // merge sort and make a new output file
        merge(strId, childIds, curDepth, degrees[curDepth]);

    } else {
        // this is a leaf node
        int* input = getFileInput(fp, startIdx, endIdx);
        quickSort(input, 0, curNData-1);
        writeSortedResultToFile(strId, input, curNData);
        printf("Process [%s] - Quick Sort - Done\n", strId);
        free(input);
    }
    
    free(degrees);
    fclose(fp);

    return EXIT_SUCCESS;
}