#include "myutils.h"

int NUM_CHILD_PRGRM_ARGS = 8;

void printArray(int arr[], int size) {
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void writeSortedResultToFile(char* myID, int arr[], int size) {
    char line[WriteBufferSize];
    char filepath[MaxFileNameLength];
    memset(line, '\0', WriteBufferSize);
    memset(filepath, '\0', MaxFileNameLength);
    sprintf(filepath, "output/%s.out", myID);

    int i;
    int idx = 0;
    idx += sprintf(&line[idx], "%d\n", size);
    for (i = 0; i < size; i++) {
        idx += sprintf(&line[idx], "%d\n", arr[i]);
    }
    writeLineToFile(filepath, line);                // Write data in the write buffer to the file
}


// Converts a string to an array of ints given they are seperated by spaces
int stringToIntArray(char* str, int arr[], int n) {
    int curInt = 0;
    int intsCompleted = 0;
    size_t strLen = strlen(str);

    for (int i = 0; i < strLen; i++) {
        char curChar = str[i];
        if (curChar == ' ' || i == strLen - 1) {
            // an int is completed
            arr[intsCompleted] = curInt;

            // setup for next int
            intsCompleted += 1;
            curInt = 0;
        } else {
            // append curInt with the new integer
            curInt *= 10;
            curInt += atoi(&curChar);
        }
    }

    if (intsCompleted != n) {
        fprintf(stderr, "ERROR: There were supposed to be %d ints in str but %d were found.\n", n, intsCompleted);   
        return -1;
    } else if (intsCompleted == 0) {
        fprintf(stderr, "ERROR: The string provided was of an invalid format or did not contain any integers.\n");
        return -1;
    }
    return 1;
}


// Gets the number of data and depth from the file
// Assumes the fp is at the beginning of the file
int getFileAttributes(FILE* fp, int* nData, int* depth) {
    char *line = (char *)malloc(sizeof(char) * LineBufferSize);     // Line buffer where a new line is stored
    size_t len = LineBufferSize;                                    // The size of the line buffer
    ssize_t nread;                                                  // Length of a new line read

    if((nread = getLineFromFile(fp, line, len)) != -1) {            // Read line and write it to line buffer
        sscanf(line, "%d %d\n", nData, depth);
    } else {
        free(line);
        return -1;
    }

    free(line);
    return 1;
}


// Gets the degrees from the file. Assumes the fp is in the right position
int getDegreesFromFile(FILE *fp, int degrees[], int depth) {
    char *line = (char *)malloc(sizeof(char) * LineBufferSize);     // Line buffer where a new line is stored
    size_t len = LineBufferSize;                                    // The size of the line buffer
    ssize_t nread;                                                  // Length of a new line read

    // Read degrees of each level
    if (depth > 0) {
        if((nread = getLineFromFile(fp, line, len)) != -1) {            // Read line and write it to line buffer
            if (stringToIntArray(line, degrees, depth) == -1) {
                free(line);
                return -1;
            }
        } else {
            free(line);
            fprintf(stderr, "ERROR: Failed to read degrees from file.\n");
            return -1;
        }
    } else {
        // if depth is 0 there will be an empty line that needs to be consumed.
        getLineFromFile(fp, line, len);
    }
    free(line);
    return 1;
}


// Gets the integer values that need to be sorted from a file
// Assumes the fp is in the correct position already
int getFileInput(FILE* fp, int arr[], int startIdx, int endIdx) {
    char *line = (char *)malloc(sizeof(char) * LineBufferSize);     // Line buffer where a new line is stored
    size_t len = LineBufferSize;                                    // The size of the line buffer
    ssize_t nread;                                                  // Length of a new line read
    int size = endIdx - startIdx + 1;

    int aNumber;
    int idxInput = 0;                                               // tracks how many ints have been added 
    int linesRead = 0;
    while((nread = getLineFromFile(fp, line, len)) != -1) {
        if (linesRead >= startIdx && linesRead <= endIdx) {
            sscanf(line, "%d\n", &aNumber);
            arr[idxInput++] = aNumber;
        }
        linesRead += 1;
        if (linesRead > endIdx) {
            break;
        }
    }

    if (idxInput != size) {
        fprintf(stderr, "ERROR: File did not contain the requested amount of data");
        return -1;
    }
    return 1;
}


void quickSort(int arr[], int low, int high) {
  if (low >= high) {
    return;
  }
  int pivot = arr[high];
  int temp;
  int i = low-1;

  for (int j = low; j < high; j++) {
    if (arr[j] <= pivot)
    {
      i++;

      temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
    }
  }

  temp = arr[i+1];
  arr[i+1] = arr[high];
  arr[high] = temp;

  quickSort(arr, low, i);
  quickSort(arr, (i+2), high);
}


// Multiway Merge Sort with multiple data streams from intermediate files
int merge(char* myId, char** childIds, int depth, int nChild) {
    int arr[1000];
    int arrLen = 0;
    for (int i=0; i < nChild; i++) {
        char path[MaxFileNameLength];
        strcpy(path, "output/");
        strcat(path, childIds[i]);
        strcat(path, ".out");



        FILE* filePathPointer = getFilePointer(path);

        int integer;
        char *line = (char *)malloc(sizeof(char) * LineBufferSize);         // Line buffer where a new line is stored
        size_t len = LineBufferSize;

        ssize_t  nread;
        nread = getLineFromFile(filePathPointer, line, len);
        while ((nread = getLineFromFile(filePathPointer, line, len)) != -1) {
          sscanf(line, "%d \n", &integer);

          int j = 0;
          while (integer > arr[j] && j < arrLen) {
            j+= 1;
          }

          // Moves everything to the right one spot.
          for (int k = arrLen-1; k >= j; k--) {
            arr[k+1] = arr[k];
          }

          // Assigns the new integer to its correct spot.
          arr[j] = integer;
          arrLen += 1;
        }

        free(line);
    }

    printf("Process [%s] - Merge Sort  - Done\n", myId);

    // writeSortedResultToFile with <myID>
    writeSortedResultToFile(myId, arr, arrLen);
    memset(arr, 0, 1000);
    return 1;
}

// Function to free a malloc'd string array
void freeStringArray(char** strings, int n) {
    for (int index = 0; index < n; index++) {
        free(strings[index]);
    }
}
