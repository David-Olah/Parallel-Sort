#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "MergeSort.h"
#include "Profiler.h"

#define USAGE_STR "Usage: ParallelSort [options] array_size thread_count\n  options:\n    -s     Run in serial mode\n"
#define INVALID_STR "Invalid syntax.\n"
#define MALFORMED_STR "Malformed number.\n"
#define ARR_SMALL_STR "Array size must be greater than 0.\n"
#define THC_SMALL_STR "Thread count must be greater than 0.\n"
#define THC_NOT_POW_2 "Thread count will be rounded to the next power of 2.\n"

// Range of randomly generated numbers.
#define RANGE 200000000

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        printf("Not enough arguments\n" USAGE_STR);
        return 2;
    }
    else if (argc > 4)
    {
        printf("Too many arguments\n" USAGE_STR);
        return 2;
    }

    char run_serial = 0;

    if (argc == 4)
    {
        if (strncmp(argv[1], "-s", 3) == 0)
            run_serial = 1;
        else
        {
            printf(INVALID_STR USAGE_STR);
            return 2;
        }
    }

    char* str_end;
    int array_size = strtol(argv[1 + run_serial], &str_end, 10);
    if (*str_end != '\0')
    {
        printf(MALFORMED_STR USAGE_STR);
        return 2;
    }
    else if (array_size <= 0)
    {
        printf(ARR_SMALL_STR);
        return 2;
    }

    int thread_count = strtol(argv[2 + run_serial], &str_end, 10);
    if (*str_end != '\0')
    {
        printf(MALFORMED_STR USAGE_STR);
        return 2;
    }
    else if (thread_count <= 0)
    {
        printf(THC_SMALL_STR);
        return 2;
    }
    else if ((thread_count & (thread_count - 1)) != 0)
        printf(THC_NOT_POW_2);

    int group_size = array_size / thread_count;

    srand((unsigned int) time(NULL));
    clock_t start;
    clock_t end;

    // Allocate space for arrays.
    int* array = (int*) malloc (array_size * sizeof(int)); 
    for(int i = 0; i < array_size; i++)
    {
        array[i] = rand() % RANGE;
    }

    init_profiler();

    // Start timer.    
    start = clock();

    // Sorts array. mergeSort will call insertionSort.
    mergeSort(array, group_size, 0, array_size - 1, run_serial);

    // Stops timer.
    end = clock();

    free (array);

    // Prints processing time.
    printf("\n\nTime in milliseconds: %ld\n", (end-start));

    // If there is any profiling data recorded, print it out here.
    // This is only for debugging.
    ProfileData* profiles;
    int profile_count;
    get_profiles(&profiles, &profile_count);

    clock_t read_args_time = 0;
    clock_t do_sort_time = 0;

    for (int i = 0; i < profile_count; i++)
    {
        ProfileData profile = profiles[i];
        printf("%-16s Total: %lums Min: %lums Max: %lums Avg: %lums\n", profile.name, profile.total, profile.min, profile.max, profile.avg);
    }

    return 0;
}