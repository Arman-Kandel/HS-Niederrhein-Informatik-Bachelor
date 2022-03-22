//
// Created by arman on 21.03.2022.
//

#include <stdlib.h>
#include <stdio.h>
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
    printf("Creating array with %d random elements... Adding: \n", arrSize);
    long* arr = calloc(arrSize, sizeof(long));
    for (long* i = arr; i < arr+arrSize; ++i) {
        *i = rand() << 16+rand();
        printf("%d ",*i);
    }
    printf("Done!\n");
    return arr;
}

// TODO make this work
void sortSmallToHigh(long* arrStart, int length){
    long* max = arrStart+length;
    for (long* i = arrStart; i < max; ++i) {
        long foundSmaller = 0;
        long smallerValue = *i;
        for (long* j = i+1; j < max; ++j) {
            if(foundSmaller==1 && *j < smallerValue){
                smallerValue = *j;
            }
            else if(*j < *i){
                foundSmaller = 1;
                smallerValue = *j;
            }
        }

        if(foundSmaller==1) *i = smallerValue;
    }
}

void printArray(long* arrStart, int length){
    for (long* i = arrStart; i < arrStart+length; ++i) {
        printf("%d ",*i);
    }
    printf("\n");
}

