// C program for insertion sort
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "InsertionSort.h"
#include "Constants.h"
 
// Function to sort an array using insertion sort
DWORD WINAPI insertionSort(void* data)
{

    InsertionArgs* args = (InsertionArgs*) data;

    int* arr = args->arr;
    int n = args->n;

    int i, key, j;
    for (i = 1; i < n; i++) 
    {
        key = arr[i];
        j = i - 1;
 
        // Move elements of arr[0..i-1], that are greater than key, to one position ahead
        // of their current position.
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        
        arr[j + 1] = key;
    }

    return 0;
}
 
// Function to print an array of size n.
void printInsertionArray(int arr[], int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d", arr[i]);
        if (i < n - 1 && arr[i] > arr[i + 1])
            printf(" !!!");
        printf("\n");
    }
}
 