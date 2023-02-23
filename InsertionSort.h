#ifndef INSERTIONSORT_H_
#define INSERTIONSORT_H_

#include <Windows.h>

typedef struct InsertionArgs {
	int* arr;
	int n;
} InsertionArgs;

DWORD WINAPI insertionSort(void* data);
void printInsertionArray(int arr[], int n);

#endif