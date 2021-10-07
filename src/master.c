#include "myutils.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Less number of arguments.\n");
        printf("./master InputFileName\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    // Please keep this function call in the main.
    // This function initializes "output" folder
    // by deleting the folder and all its files,
    // and then creating a clean "output" folder.
    bookeepingCode();

    // Parse input file
    FILE * fp;                                                      // File pointer
    char *line = (char *)malloc(sizeof(char) * LineBufferSize);         // Line buffer where a new line is stored
    size_t len = LineBufferSize;                                        // The size of the line buffer
    ssize_t nread;                                                  // Length of a new line read

    char inputFileName[MaxFileNameLength];
    memset(inputFileName, '\0', MaxFileNameLength);
    sprintf(inputFileName, "input/%s", argv[1]);


    if ((fp = getFilePointer(inputFileName)) == NULL) {             // Open a file and return file pointer to the file
        exit(EXIT_FAILURE);
    }

    // Read the number of data and depth
    int nData;
    int depth = 0;
    if((nread = getLineFromFile(fp, line, len)) != -1) {            // Read next line and write it to line buffer
        sscanf(line, "%d %d\n", &nData, &depth);
    }

    // Read degrees of each level
    int *degrees = NULL;
    if (depth > 0) {
        if((nread = getLineFromFile(fp, line, len)) != -1) {            // Read next line and write it to line buffer
           degrees = stringToIntArray(line, depth);
            if (degrees == NULL) {
                exit(EXIT_FAILURE);
            }
        } else {
            // TODO: handle failure
        }
    } else {
        // if depth is 0 there will be an empty line that needs to be consumed.
        getLineFromFile(fp, line, len);
    }

    // Read input data
    int * input = (int *)malloc(sizeof(int) * nData);
    int aNumber;
    int idxInput = 0;
    while((nread = getLineFromFile(fp, line, len)) != -1) {
        sscanf(line, "%d\n", &aNumber);
        input[idxInput++] = aNumber;
    }

    free(line);
    fclose(fp);

    /*
     * Each process is assigned floor(nData/degrees[current_depth])
     */

    // TODO: Spawn child processes and launch childProgram if necessary

    pid_t pid;
    int id = 0;
    int startIdx = 0;
    int endIdx = nData-1;
    int level;

    for (level = 0; level < depth; level++) {
        id *= 10;
        int dataPerProcess = nData/degrees[level];
        for (int j = 0; j < degrees[level]; j++) {
            pid = fork();
            id += 1;
            
            if (pid == 0) {
                if (j == degrees[level] - 1) {
                    nData = nData - (degrees[level] - 1) * dataPerProcess;
                    startIdx = endIdx-(nData-1);
                } else {
                    nData = dataPerProcess;
                    startIdx += j*dataPerProcess;
                    endIdx = startIdx + (dataPerProcess-1);
                }
                if (level == depth-1) {
                    char strDepth[LineBufferSize], strId[LineBufferSize], strStartIdx[LineBufferSize];
                    char strEndIdx[LineBufferSize], strNData[LineBufferSize];

                    sprintf(strDepth, "%d", depth);
                    sprintf(strId, "%d", id);
                    sprintf(strStartIdx, "%d", startIdx);
                    sprintf(strEndIdx, "%d", endIdx);
                    sprintf(strNData, "%d", nData);

                    char * args[8];
                    args[0] = "childProgram";
                    args[1] = strDepth;
                    args[2] = strId;
                    args[3] = strStartIdx;
                    args[4] = strEndIdx;
                    args[5] = strNData;
                    args[6] = argv[1];
                    args[7] = NULL;

                    int flag = execv(args[0], args);
                    printf("%d\n", flag);
                }
               
                break;
            }
        }
        if (pid > 0) {
            id /= 10;
            // I think the ids probably weren't being reset for the parent processes.
            break;
        }

    }
    // Wait all child processes to terminate if necessary
    if (depth > 0) {
        for (int i = 0; i < degrees[level]; i++) {
            wait(NULL);
        }
    }
    // TODO: Merge sort or Quick sort (or other leaf node sorting algorithm)
    
    free(input);
    free(degrees);

    return EXIT_SUCCESS;
}

