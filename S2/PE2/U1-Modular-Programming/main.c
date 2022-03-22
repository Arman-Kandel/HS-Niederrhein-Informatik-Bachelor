#include <stdio.h>
#include "liste.h"
#include "maths.h"
#include "sort.h"

void testListe() {
    int i;
    list_t *l = create();
    for (i = 1; i < 30; i++)
        append(l, i);
    toScreen(l);
    i = getValueAt(l, 30);
    if (getError(l) == 0) printf(" value [%2 d] = %2 d\n ", 30, i);
    destroy(l);
}

void testMaths() {
    printDoubleAsBruch(2.0815);
}

void testSort(){
    int arrSize = 7000;
    long* arrStart = createRandomArray(arrSize);
    sortSmallToHigh(arrStart, arrSize);
    //printArray(arrStart, arrSize);
}

int main() {
    //testListe();
    //testMaths();
    testSort();
    return 0;
}
