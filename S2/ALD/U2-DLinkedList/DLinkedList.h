//
// Created by arman on 29.03.2022.
//

#ifndef U2_DLINKEDLIST_H
#define U2_DLINKEDLIST_H

#include "DLinkedValue.h"
#include <iostream>

using namespace std;

template<typename T> class DLinkedList{

private:
    DLinkedValue<T> values[10];
    int valuesSize = 10;
    int nextIndex = 0;

public:
    void add(T value);
    void print();
};

template<typename T>
void DLinkedList<T>::add(T value) {
    DLinkedValue<T> valueContainer;
    valueContainer.value = value;
    if(nextIndex == valuesSize){ // Increase arr size
        DLinkedValue<T> temp[(valuesSize*2)];
        for (int i = 0; i < valuesSize; ++i) {
            temp[i] = values[i];
        }
        valuesSize = valuesSize * 2;
        values = *temp;
    }
    values[nextIndex] = *valueContainer;
    valueContainer.prevValue = *values[nextIndex-1];
    valueContainer.nextValue = *values[nextIndex+1];
    nextIndex++;
}

template<typename T>
void DLinkedList<T>::print() {
    cout << "Printing " << valuesSize << " values:" << endl;
    for (int i = 0; i < valuesSize; ++i) {
        if(values[i] == NULL)
            cout << "null" << endl;
        else
            cout << "val:" << values[i].value << " prev:" << values[i].prevValue << " next: " << values[i].nextValue << endl;
    }
}


#endif //U2_DLINKEDLIST_H
