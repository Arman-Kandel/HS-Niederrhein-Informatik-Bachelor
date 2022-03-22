#include <stdio.h>
#include <stdint.h>
#include "maths.h"
#include <stdlib.h>

int findComma(char string[], int length){
    for (int i = 0; i < length; ++i) {
        char c = string[i];
        if(c == '.' || c == ',')
            return i;
    }
    return -1;
}

void printDoubleAsBruch(double number) {
    int numberLength = sizeof(number);
    printf("%d",numberLength);
    char string[numberLength];
    snprintf(string, numberLength, "%f", number);
    int i = findComma(string, numberLength);
    for (int j = 0; j < i; ++j) {
        printf("LOL");
    }

    int multiplicator = 10;
    int currentMultiplicator = 1;
    int sign = 0;
    int exponent = 0;
    int exponentZeroOffset = 1023;
}

/**
 * Prints the bits of the provided number out, ordered from least significant
 * to most significant within bytes and reading the bytes from first to last.
 * Depending on your machine architecture that means the bytes may or may not be in order of significance.
 * Intel is strictly little endian so you should get all bits from least significant to most;
 * most CPUs use the same endianness for floating point numbers as for integers but even that's not guaranteed.
 * @author https://stackoverflow.com/questions/32259970/get-exact-bit-representation-of-a-double-in-c
 */
void printDoubleAsBits(double number) {
    uint8_t *bytePointer = (uint8_t * ) & number;
    for (size_t index = 0; index < sizeof(double); index++) {
        uint8_t byte = bytePointer[index];

        for (int bit = 0; bit < 8; bit++) {
            printf("%d", byte & 1);
            byte >>= 1;
        }
    }
}


