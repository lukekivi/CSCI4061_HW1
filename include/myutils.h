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
 * @param n   Number of integers within the string
 * @return    Array of n integers that must be freed
 */
int* stringToIntArray(char* str, int n);

/**
 * Get and parse first and second line of an input file.
 * 
 * @param fp      the file to parse
 * @param nData   address of variable to be assigned number of data
 * @param depth   address of variable to be assigned depth
 * @param degrees address of variable to be assigned array of degrees
 */
void getFileAttributes(FILE* fp, int* nData, int* depth, int** degrees);

/**
 * Get the data to be sorted from an input file. Assumes the fp is already in the right place.
 * 
 * @param fp the file to parse, already in position
 * @param startIdx where to start recording values
 * @param endIdx   where to end recording values
 */
int* getFileInput(FILE* fp, int startIdx, int endIdx);

void quickSort(int arr[], int low, int high);
void merge(char* myID, int depth, int nChild);

/**
 * Make a sub array from a parent array.
 * @param  nums       Parent array
 * @param  startIndex Index to start division of array
 * @param  endIndex   Index to end division of array
 * @return A sub array
 */
int* makeSubArray(int nums[], int startIndex, int endIndex);

/**
 * Generate an input file for childProgram.
 * 
 * @param myId id of the calling process
 * @param nums array of relevent integers
 * @param n    size of array nums
 * @return 1 for success, -1 for error
 */
int generateOutputFile(char* myId, int nums[], int n);

#endif //MYUTILS_H
