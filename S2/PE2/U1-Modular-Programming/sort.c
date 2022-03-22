//
// Created by arman on 21.03.2022.
//

#include <stdlib.h>
#include <stdio.h>
#include <pthread_time.h>
#include <math.h>
#include "sort.h"

/**
 * Creates a new random array and returns the pointer to the first element. <br>
 * Loop through the array like this:
 * @code
 * for (long* i = arr; i < arr+arrSize; ++i)
 * ...
 * @endcode
 */
long* createRandomArray(int arrSize){
    printf("Creating array with %d random elements...\n", arrSize);
    long* arr = calloc(arrSize, sizeof(long));
    for (long* i = arr; i < arr+arrSize; ++i) {
        *i = rand() << 16+rand();
        //printf("%d ",*i);
    }
    printf("Done!\n");
    return arr;
}


void sortSmallToHigh(long* arrStart, int length){
    struct timespec timeStart, timeEnd;
    clock_gettime(CLOCK_REALTIME, &timeStart);
    long countChecks, countSwitches;
    long* max = arrStart+length;
    for (long* i = arrStart; i < max; ++i) {
        long* smallerIndex = NULL;
        long smallerValue = *i;
        for (long* j = i+1; j < max; ++j) {
            long newVal = *j;
            countChecks++;
            if(newVal < smallerValue){
                smallerIndex = j;
                smallerValue = newVal;
            }
        }
        if(smallerIndex != NULL) {
            countSwitches++;
            *smallerIndex = *i;
            *i = smallerValue;
        }
    }
    clock_gettime(CLOCK_REALTIME, &timeEnd);
    long ns = timeEnd.tv_nsec - timeStart.tv_nsec;
    if(ns < 1.0e6)
        printf("Sorted array! Took %d nanoseconds or %d milliseconds.\n", ns, round(ns / 1.0e6));
    else
        printf("Sorted array! Took %d milliseconds.\n", round(ns / 1.0e6));
    printf("Checks: %d\n", countChecks);
    printf("Switches: %d\n", countSwitches);
}

void printArray(long* arrStart, int length){
    for (long* i = arrStart; i < arrStart+length; ++i) {
        printf("%d ",*i);
    }
    printf("\n");
}

