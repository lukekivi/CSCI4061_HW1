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
        // track all child ids to avoid generating them again later
        char* childIds[degrees[0]];

        pid_t pid;
        int id = 0;
        int dataPerProcess = nData/degrees[0];

        // common data for execution of childProgram
        char program[] = "childProgram";
        char programPath[] = "childProgram";
        char strDepth[] = "1";

        for (int i = 0; i < degrees[0]; i++) {
            id += 1;
            
            // generate childId and store it in array
            
            childIds[i] = (char*) malloc(sizeof(char) * LineBufferSize);
            sprintf(childIds[i], "%d", id);

            pid = fork();

            if (pid == 0) {
                // child process sets up for executing childProgram
                if (i == degrees[0] - 1) {
                    // the final node in the child set gets all of the remaining data
                    nData = nData - (degrees[0] - 1) * dataPerProcess;
                    startIdx = endIdx-(nData-1);
                } else {
                    nData = dataPerProcess;
                    startIdx += i * dataPerProcess;
                    endIdx = startIdx + (dataPerProcess-1);
                }
        
                // get string versions of data for childProgram arguments
                char strStartIdx[LineBufferSize], strEndIdx[LineBufferSize], strNData[LineBufferSize];

                sprintf(strStartIdx, "%d", startIdx);
                sprintf(strEndIdx, "%d", endIdx);
                sprintf(strNData, "%d", nData);
                printf("Parent [%s] - Spawn Child [%s, %s, %s, %s, %s]\n", "master", "1", childIds[i], strStartIdx, strEndIdx, strNData);

                if (execl(programPath, program, strDepth, childIds[i], strStartIdx, strEndIdx, strNData, inputFileName , NULL) == -1) {
                    fprintf(stderr, "ERROR: Failed to exec child program.");
                    
                    for (int index = 0; index < degrees[0]; index++) {
                        freeStringArray(childIds, degrees[0]);
                    }

                    fclose(fp);
                    exit(EXIT_FAILURE);
                }
            }
        }

        // Wait all child processes to terminate 
        for (int i = 0; i < degrees[0]; i++) {
            wait(NULL);
        }
        
        // merge data from child processes
        if (merge("master", childIds, 0, degrees[0]) == 1) {
            freeStringArray(childIds, degrees[0]);
        }
    } else {

        // Read input data from original input file
        int input[nData];
        if ((getFileInput(fp, input, startIdx, endIdx)) == -1) {
            exit(EXIT_FAILURE);
        }

        // sort and write output file
        quickSort(input, 0, nData-1);
        writeSortedResultToFile("master", input, nData);
        printf("Process [master] - Quick Sort - Done\n");
    }

    fclose(fp);
    return EXIT_SUCCESS;
}
