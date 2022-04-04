//
// Created by arman on 04.04.2022.
//

#ifndef U3_C_CLASSES_STACK_H
#define U3_C_CLASSES_STACK_H

#include <iostream>

template<typename T>
class Stack {
private:
    int next = 0;
    int sizeValues = 10;
    T *values = new T[sizeValues];
    int countValues = 0;
    char error = 0;

    void stackFree() {
        free(values);
    }

    char stackIsEmpty() {
        return sizeValues == 0;
    }

    char isFull() {
        return sizeValues == next;
    }

    void increase() {
        sizeValues *= 4;
        values = (T *) realloc(values,
                               sizeValues * sizeof(T));
    }

    void decrease() {
        sizeValues /= 4;
        values = (T *) realloc(values,
                               sizeValues * sizeof(T));
    }

public:
    ~Stack() {
        stackFree();
    };

    /**
    * Adds the provided value to the provided stack.
    */
    void add(T value) {
        if (isFull()) increase();
        values[next] = value;
        next += 1;
        countValues++;
    }

    /**
     * Returns the last added value from the provided stack.
    */
    T getLast() {
        int i = next - 1;
        if (i < 0) {
            error = 1;
            return -1;
        }
        return values[i];
    }

    /**
    * Removes the last added value from the provided stack.
    */
    int freeLast() {
        int i = next - 1;
        if (i < 0) {
            error = 1;
            return -1;
        }
        free(&values[i]);
        countValues--;
        return 0;
    }

    char getError() {
        return error;
    }

    void print() {
        for (T *i = values; i < values + countValues; ++i) {
            T a = *i;
            std::cout << a << "\n";
        }
    }
};


#endif //U3_C_CLASSES_STACK_H
