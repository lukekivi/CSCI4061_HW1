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
 * @param str the string of n integers seperated by spaces and ending with a new line 
 * @param arr the array to be filled with integers from str
 * @param n   Number of integers within the string
 * @return    Array of n integers that must be freed
 * @return        success (1) or error value (-1)
 */
int stringToIntArray(char* str, int arr[], int n);


/**
 * Get and parse first line of an input file.
 * 
 * @param fp      the file to parse
 * @param nData   address of variable to be assigned number of data
 * @param depth   address of variable to be assigned depth
 * @return        success (1) or error value (-1)
 */
int getFileAttributes(FILE* fp, int* nData, int* depth);


/**
 * Get and parse second line of an input file.
 * 
 * @param fp      the file to parse
 * @param degrees array to be filled with integers
 * @param depth   depth of the search
 * @return        success (1) or error value (-1)
 */
int getDegreesFromFile(FILE *fp, int degrees[], int depth);


/**
 * Get the data to be sorted from an input file. Assumes the fp is already in the right place.
 * 
 * @param fp the file to parse, already in position
 * @param startIdx where to start recording values
 * @param endIdx   where to end recording values
 */
int* getFileInput(FILE* fp, int startIdx, int endIdx);

void quickSort(int arr[], int low, int high);


void merge(char* myID, char** childIds, int depth, int nChild);


/**
 * Make a sub array from a parent array.
 * @param  nums       Parent array
 * @param  startIndex Index to start division of array
 * @param  endIndex   Index to end division of array
 * @return A sub array
 */
int* makeSubArray(int nums[], int startIndex, int endIndex);

#endif //MYUTILS_H
