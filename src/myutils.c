#include "myutils.h"

int NUM_CHILD_PRGRM_ARGS = 8;
int MAX_FILENAME_SIZE = 50;

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

int *stringToIntArray(char* str, int n) {
    int *integers = (int*) malloc(sizeof(int) * n);
    int curInt = 0;
    int intsCompleted = 0;
    size_t strLen = strlen(str);

    for (int i = 0; i < strLen; i++) {
        char curChar = str[i];
        if (curChar == ' ' || i == strLen - 1) {
            integers[intsCompleted] = curInt;

            intsCompleted += 1;
            curInt = 0;
        } else {
            curInt *= 10;
            curInt += atoi(&curChar);
        }
    }

    if (intsCompleted != n) {
        fprintf(stderr, "ERROR: There were supposed to be %d ints in str but %d were found.\n", n, intsCompleted);   
        integers = NULL;
    } else if (intsCompleted == 0) {
        fprintf(stderr, "ERROR: The string provided was of an invalid format or did not contain any integers.\n");
        integers = NULL;
    }
    return integers;
}

void getFileAttributes(FILE* fp, int* nData, int* depth, int** degrees) {
    char *line = (char *)malloc(sizeof(char) * LineBufferSize);     // Line buffer where a new line is stored
    size_t len = LineBufferSize;                                    // The size of the line buffer
    ssize_t nread;                                                  // Length of a new line read

    // Read nData and depth    
    if((nread = getLineFromFile(fp, line, len)) != -1) {            // Read next line and write it to line buffer
        sscanf(line, "%d %d\n", nData, depth);
    }

    if (*depth > 9 || *depth < 0) {
        fprintf(stderr, "ERROR: Invalid depth. (min = 0 | max = 9)\n");
        exit(EXIT_FAILURE);
    }

    // Read degrees of each level
    if (*depth > 0) {
        if((nread = getLineFromFile(fp, line, len)) != -1) {            // Read next line and write it to line buffer
           *degrees = stringToIntArray(line, *depth);
            if (degrees == NULL) {
                fclose(fp);
                free(line);
                free(degrees);
                exit(EXIT_FAILURE);
            }
        } else {
            fprintf(stderr, "ERROR: Failed to read degrees from file.\n");
            fclose(fp);
            free(line);
            free(degrees);
            exit(EXIT_FAILURE);
        }
    } else {
        // if depth is 0 there will be an empty line that needs to be consumed.
        getLineFromFile(fp, line, len);
    }
    free(line);
}

int* getFileInput(FILE* fp, int startIdx, int endIdx) {
    char *line = (char *)malloc(sizeof(char) * LineBufferSize);     // Line buffer where a new line is stored
    size_t len = LineBufferSize;                                    // The size of the line buffer
    ssize_t nread;                                                  // Length of a new line read
    int size = endIdx - startIdx + 1;
    int * input = (int *)malloc(sizeof(int) * size);

    int aNumber;
    int idxInput = 0;
    int linesRead = 0;
    while((nread = getLineFromFile(fp, line, len)) != -1) {
        if (linesRead >= startIdx && linesRead <= endIdx) {
            sscanf(line, "%d\n", &aNumber);
            input[idxInput++] = aNumber;
        }
        linesRead += 1;
        if (linesRead > endIdx) {
            break;
        }
    }

    if (idxInput != size) {
        fprintf(stderr, "ERROR: File did not contain the requested amount of data");
        free(input);
        fclose(fp);
        exit(EXIT_FAILURE);
    }
    return input;
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


// TODO: Multiway Merge Sort with multiple data streams from intermediate files
void merge(char* myID, int depth, int nChild) {
    
    // translates myID to an integer so we can use math to find childrens name
    int intmyId = atoi(myID);

    // figure out how long the array needs to be
    int arr[1000];
    int childFileName;
    char *line = (char *)malloc(sizeof(char) * LineBufferSize); 
    int len = 0;
    printf("Got here\n");
    // for loop reading for nChild, myID * 10  + current nChild
    for (int i=1; i <= nChild; i++) {
        // find the id of the child we want to read from
        childFileName = intmyId * 10 + i;
        // read from childFileName
        FILE* fpt_read = fopen("<childFileName>.out", "r");
        if (fpt_read == NULL) {
            printf("Press any key to exit...\n");
            exit(0);
        }

        // Get the first line into childLen.
        char *line = (char *)malloc(sizeof(char) * LineBufferSize);         // Line buffer where a new line is stored
        size_t len = LineBufferSize;
        ssize_t  nread;
        int childLen;
        if((nread = getLineFromFile(fpt_read, line, len)) != -1) {            // Read next line and write it to line buffer
            sscanf(line, "%d \n", &childLen);
        }

        // Read the rest.
        int integer;
        int ch; //placeholder, fgetc reads in char
        while ((ch = fgetc(fpt_read)) != EOF) {
            integer = ch;

            // Finds the correct spot for the integer (j).
            int j = 0;
            while (integer > arr[j] && j < len) {
            j+= 1;
            }

            // Moves everything to the right one spot.
            for (int k = len-1; k >= j; k--) {
            arr[k+1] = arr[k];
            }

            // Assigns the new integer to its correct spot.
            arr[j] = integer;
            len += 1;

        }


        fclose(fpt_read);
    }

  free(line);

  // writeSortedResultToFile with <myID>
  writeSortedResultToFile(myID, arr, len);
}

int generateOutputFile(char* myId, int nums[], int n) {    
    char path[MAX_FILENAME_SIZE];
    strcpy(path, "output/");
    strcat(path, myId);
    strcat(path, ".out");

    FILE* fp = fopen(path, "w");

    for (int i = 0; i < n; i++) {
        if (fprintf(fp, "%d\n", nums[i]) < 0) {
            return -1;
        }
    }
    fclose(fp);
   return 1;
}
