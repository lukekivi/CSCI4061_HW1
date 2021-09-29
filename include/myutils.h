#ifndef MYUTILS_H
#define MYUTILS_H

#include "utils.h"
#define debug 0

// You can modify these functions as you need

/**
 * Print all elements in the array (space separation)
 *
 * @param arr   integer array
 * @param line  size of elements in the array
 * @return None
 */
void printArray(int arr[], int size);

/**
 * Write sorted numbers to a file
 *
 * @param myID  process's ID which is a part of output filename
 * @param arr   integer array storing sorted numbers
 * @param size  size of element in the array
 * @return None
 */
void writeSortedResultToFile(char* myID, int arr[], int size);

/**
 * Split a string into an array of n integers.
 * 
 * @param str the string of n integers seperated by spaces
 * and ending with a new line 
 * @param n   number of integers within the string
 * @return Array of n integers that must be freed
 */
int *stringToIntArray(char* str, int n);


void quickSort(int arr[], int low, int high);
void merge(char* myID, int depth, int nChild);

#endif //MYUTILS_H
