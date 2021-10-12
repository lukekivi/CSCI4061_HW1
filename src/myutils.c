#include "myutils.h"

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
        printf("An error has occurred: there were supposed to be %d ints in str but %d were found.\n", n, intsCompleted);   
        integers = NULL;
    } else if (intsCompleted == 0) {
        printf("The string provided was of an invalid format or did not contain any integers.\n");
        integers = NULL;
    }

    return integers;
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

  quicksort(arr, low, i);
  quicksort(arr, (i+2), high);
}


// TODO: Multiway Merge Sort with multiple data streams from intermediate files
void merge(char* myID, int depth, int nChild) {
  // translates myID to an integer so we can use math to find childrens name
  int intmyId = atoi(myID);

  // figure out how long the array needs to be
  int arr[1000];
  int childFileName;
  int len = 0;

  // for loop reading for nChild, myID * 10  + current nChild
  for (int i=1; i <= nChild; i++) {
      // find the id of the child we want to read from
      childFileName = intmyid * 10 + i;
      // read from childFileName
      fpt_read = fopen("<childFileName>.out", "r");
      if (fpt_read == NULL) {
        printf("Press any key to exit...\n");
        exit(0);
      }

      // Get the first line into childLen.
      char *line = (char *)malloc(sizeof(char) * LineBufferSize);         // Line buffer where a new line is stored
      size_t len = LineBufferSize;
      ssize_t  nread;
      int childLen;
      if((nread = getLineFromFile(fp, line, len)) != -1) {            // Read next line and write it to line buffer
          sscanf(line, "%d \n", &childLen);
      }

      // Read the rest.
      int integer;
      char ch; //placeholder, fgetc reads in char
      while ((ch = fgetc(fpt_read)) != EOF) {
        integer = atoi(ch);

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
