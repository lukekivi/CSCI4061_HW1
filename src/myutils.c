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

    printf("String: %s\n", str);
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
    for (int i = 0; i < n; i++) {
        printf("%d ",integers[i]);    
    }
    printf("\n");
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

  quickSort(arr, low, i);
  quickSort(arr, (i+2), high);
}


// TODO: Multiway Merge Sort with multiple data streams from intermediate files
void merge(char* myID, int depth, int nChild) {


}
