//
// Created by arman on 29.03.2022.
//

#ifndef U2_DLINKEDVALUE_H
#define U2_DLINKEDVALUE_H


template<typename T> class DLinkedValue {
public:
    T value;
    DLinkedValue* prevValue;
    DLinkedValue* nextValue;
};


#endif //U2_DLINKEDVALUE_H
