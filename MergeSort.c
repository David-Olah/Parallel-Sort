#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <Windows.h>

#include "MergeSort.h"
#include "InsertionSort.h"

// Merges two subarrays of arr[].
// First subarray is arr[l..m].
// Second subarray is arr[m+1..r].
void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays.
    int* L = (int*)malloc(n1 * sizeof(int));
    int* R = (int*)malloc(n2 * sizeof(int));

    // Copy data to temp arrays L[] and R[].
    for (i = 0; i < n1; i++)
    {
        L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++)
    {
        R[j] = arr[m + 1 + j];
    }

    // Merge the temp arrays back into arr[l..r].
    i = 0; // Initial index of first subarray.
    j = 0; // Initial index of second subarray.
    k = l; // Initial index of merged subarray.
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any.
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any.
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

// l is for left index and r is right index of the sub-array of arr to be sorted.
void recursiveMergeSort(int arr[], int group_size, int l, int r, int *threadCount, HANDLE *threads, InsertionArgs *globalArgs, char run_serial)
{
    if (l < r)
    {
        // If size of the array is larger than S, use merge sort, otherwise use insertion sort.
        if (r - l + 1 > group_size)
        {
            // Same as (l+r)/2, but avoids overflow for large l and h.
            int m = l + (r - l) / 2;

            // Sort first and second halves.
            recursiveMergeSort(arr, group_size, l, m, threadCount, threads, globalArgs, run_serial);
            recursiveMergeSort(arr, group_size, m + 1, r, threadCount, threads, globalArgs, run_serial);

            // Merges arrays back together.
            merge(arr, l, m, r);
        }
        else
        {
            InsertionArgs *args = globalArgs + *threadCount;
            args->arr = arr + l;
            args->n = r - l;

            if (run_serial)
                insertionSort(args);
            else
            {
                HANDLE handle;
                do {
                    handle = CreateThread(NULL, 0, insertionSort, args, 0, 0);
                } while (handle == INVALID_HANDLE_VALUE);
                threads[(*threadCount)++] = handle;
            }
        }
    }
}

void mergeSort(int arr[], int group_size, int l, int r, char run_serial)
{
    int size = r - l + 1;

    int maxThreads = (int)ceil(((double)size) / ((double)group_size)) * 2;
    int threadCount = 0;

    HANDLE* threads = NULL;
    InsertionArgs* globalArgs = malloc(maxThreads * sizeof(InsertionArgs));

    if (!run_serial)
        threads = malloc(maxThreads * sizeof(HANDLE));

    recursiveMergeSort(arr, group_size, l, r, &threadCount, threads, globalArgs, run_serial);

    if (!run_serial)
    {
        WaitForMultipleObjects(threadCount, threads, TRUE, INFINITE);
        for (int i = 0; i < threadCount; i++)
            CloseHandle(threads[i]);
    }

    threadCount += run_serial;
    printf("Used %d threads\n", threadCount);

    if (!run_serial)
        free(threads);
    free(globalArgs);
}

// Function to print an array.
void printMergeArray(int A[], int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        printf("%d ", A[i]);
    }
    printf("\n");
}
