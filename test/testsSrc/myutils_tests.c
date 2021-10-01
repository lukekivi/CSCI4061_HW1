#include <stdio.h>
#include <stdlib.h>
#include "myutils.h"

// the functions to test
int *stringToIntArray(char* str, int n);

// print array to stdouts
void printAnArray(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int numElements;
    scanf("%d\n", &numElements); 
    int *arr;

    char *line = (char *) malloc(sizeof(char) * numElements * 10);
    size_t bufferSize = LineBufferSize;

    getline(&line, &bufferSize, stdin);

    arr = stringToIntArray(line, numElements);
    if (arr != NULL) {
        printAnArray(arr, numElements);
    }

    free(arr);
    free(line);
    return 0;
}