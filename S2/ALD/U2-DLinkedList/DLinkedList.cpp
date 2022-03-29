//
// Created by arman on 29.03.2022.
//

#include "DLinkedList.h"
#include "DLinkedValue.h"

template<typename T>
void DLinkedList<T>::add(T value) {
    DLinkedValue<T> valueContainer();
    valueContainer.value = value;
}
