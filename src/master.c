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

    char inputFileName[MaxFileNameLength];
    memset(inputFileName, '\0', MaxFileNameLength);
    sprintf(inputFileName, "input/%s", argv[1]);


    if ((fp = getFilePointer(inputFileName)) == NULL) {             // Open a file and return file pointer to the file
        exit(EXIT_FAILURE);
    }

    int nData;
    int depth;
    int *degrees = NULL;
    getFileAttributes(fp, &nData, &depth, &degrees);

    // Spawn child processes and launch childProgram if necessary
    int* input = NULL;
    int startIdx = 0;
    int endIdx = nData-1;

    if (depth > 0) {
        pid_t pid;
        int id = 0;
        int dataPerProcess = nData/degrees[0];
        char program[] = "childProgram";
        char programPath[] = "childProgram";
        char strDepth[] = "1";

        for (int i = 0; i < degrees[0]; i++) {
            id += 1;
            pid = fork();

            if (pid == 0) {
                if (i == degrees[0] - 1) {
                    nData = nData - (degrees[0] - 1) * dataPerProcess;
                    startIdx = endIdx-(nData-1);
                } else {
                    nData = dataPerProcess;
                    startIdx += i * dataPerProcess;
                    endIdx = startIdx + (dataPerProcess-1);
                }
        
                char strId[LineBufferSize], strStartIdx[LineBufferSize];
                char strEndIdx[LineBufferSize], strNData[LineBufferSize];

                sprintf(strId, "%d", id);
                sprintf(strStartIdx, "%d", startIdx);
                sprintf(strEndIdx, "%d", endIdx);
                sprintf(strNData, "%d", nData);

                if (execl(programPath, program, strDepth, strId, strStartIdx, strEndIdx, strNData, inputFileName , NULL) == -1) {
                    fprintf(stderr, "ERROR: Failed to exec child program.");
                    free(degrees);
                    fclose(fp);
                    exit(EXIT_FAILURE);
                }
            }
        }


        // Wait all child processes to terminate if necessary
        for (int i = 0; i < degrees[0]; i++) {
            wait(NULL);
        }
    } else {
        // Read input data from original input file
        input = getFileInput(fp, startIdx, endIdx);
        quickSort(input, 0, nData-1);
    }
    // TODO: Merge sort or Quick sort (or other leaf node sorting algorithm)
    
    fclose(fp);
    free(input);
    free(degrees);

    return EXIT_SUCCESS;
}
