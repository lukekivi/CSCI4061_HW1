/*test machine: csel-broccoli.cselabs.umn.edu
* group number: G[45]
* name: Lucas Kivi, Dallas Schauer, Viet Nguyen
* x500: kivix019, schau364, nguy4471 */
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
    if (getFileAttributes(fp, &nData, &depth) == -1) {
        exit(EXIT_FAILURE);
    }

    if (nData < 1) {
        fprintf(stderr, "ERROR: No input data.\n");
        exit(EXIT_FAILURE);
    } 
    
    if (depth > 9) {
        fprintf(stderr, "ERROR: Depth is greater than 9.\n");
        exit(EXIT_FAILURE);
    }

    int degrees[depth];
    if (getDegreesFromFile(fp, degrees, depth) == -1) {
        exit(EXIT_FAILURE);
    }

    // calculate number of leaf nodes
    int numLeafNodes = 1;
    for (int i = 0; i < depth; i++) {
        if (degrees[i] < 1 || degrees[i] > 9) {
            fprintf(stderr, "ERROR: Degree of a level is not between 1 and 9.\n");
            exit(EXIT_FAILURE);    
        }
        numLeafNodes *= degrees[i];
    }

    if (numLeafNodes > nData) {
        fprintf(stderr, "ERROR: Number of leaf nodes is greater than the size of input data.\n");
        exit(EXIT_FAILURE);
    }

    // Spawn child processes and launch childProgram if necessary
    int* input = NULL;
    int startIdx = 0;
    int endIdx = nData-1;

    if (depth > 0) {
        char* childIds[degrees[0]];

        pid_t pid;
        int id = 0;
        int dataPerProcess = nData/degrees[0];
        char program[] = "childProgram";
        char programPath[] = "childProgram";
        char strDepth[] = "1";

        for (int i = 0; i < degrees[0]; i++) {
            id += 1;
            childIds[i] = (char *) malloc(sizeof(char) * LineBufferSize);
            sprintf(childIds[i], "%d", id);

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
                printf("Parent [%s] - Spawn Child [%s, %s, %s, %s, %s]\n", "master", "1", strId, strStartIdx, strEndIdx, strNData);

                if (execl(programPath, program, strDepth, strId, strStartIdx, strEndIdx, strNData, inputFileName , NULL) == -1) {
                    fprintf(stderr, "ERROR: Failed to exec child program.");
                    fclose(fp);
                    exit(EXIT_FAILURE);
                }
            }
        }


        // Wait all child processes to terminate if necessary
        for (int i = 0; i < degrees[0]; i++) {
            wait(NULL);
        }

        merge("master", childIds, 0, degrees[0]);

        for (int i = 0; i < degrees[0]; i++) {
            free(childIds[i]);
        }
    } else {
        // Read input data from original input file
        if ((input = getFileInput(fp, startIdx, endIdx)) == NULL) {
            free(input);
            fprintf(stderr, "ERROR: File did not contain the requested amount of data");
            exit(EXIT_FAILURE);
        }
        quickSort(input, 0, nData-1);
        writeSortedResultToFile("master", input, nData);
        printf("Process [master] - Quick Sort - Done\n");
    }

    fclose(fp);
    free(input);
    return EXIT_SUCCESS;
}
