//
// Created by arman on 25.04.2022.
//

#ifndef U6_GENERICS_QUICKSORT_H
#define U6_GENERICS_QUICKSORT_H
#include <sys/time.h>

template<typename T>
class QuickSort {
private:
    /**
     * Swaps the values of the provided elements.
     * @param pEl1 pointer to the first element to swap.
     * @param pEl2 pointer to the second element to swap.
     */
    void swap(T* pEl1, T* pEl2);
    /**
     * Splits the array into two sections, around
     * the pivot element (which is pEndEl).
     * On the left section the elements are smaller than the pivot,
     * and on the right section they are bigger.
     * @param pStartEl
     * @param pEndEl
     * @return
     */
    T partition(T* pStartEl, T* pEndEl);
public:
    /**
     * Sorts the provided array (or part of array) using the QuickSort algorithm.
     * @param pFirstEl pointer to the first element in the array.
     * @param length the arrays' length, aka count of elements.
     */
    void sort(T* pFirstEl, int length);
    std::string toString(T* pFirstEl, int length);
    long countSwaps = 0;
    long countComparisons = 0;
    long countPartitions = 0;
    bool printSteps = false;
};

template<typename T>
void QuickSort<T>::swap(T *pEl1, T *pEl2) {
    T temp = *pEl1;
    *pEl1 = *pEl2;
    *pEl2 = temp;
    countSwaps++;
    if(printSteps) std::cout << "swap(" << *pEl1 <<", "<< *pEl2 <<")" << std::endl;
}

template<typename T>
T QuickSort<T>::partition(T *pStartEl, T *pEndEl) {
    T pivot = *pEndEl;
    T* i = pStartEl; // Index of smaller element and indicates the right position of pivot found so far
    countPartitions++;
    countComparisons++;
    if(printSteps) std::cout << "partition(" << *pStartEl <<", "<< *pEndEl <<")\n";
    if(pStartEl < pEndEl){
        for (T* j = pStartEl; j <= pEndEl; j++)
        {
            // If current element is smaller than the pivot
            countComparisons++;
            if(printSteps) std::cout << *j << " < " << pivot << std::endl;
            if (*j < pivot)
            {
                swap(i, j);
                i++; // increment index of smaller element
            }
        }
        swap(i, pEndEl);
        // Recursion:
        partition(pStartEl, i-1);
        partition(i+1, pEndEl);
    }
    return *i;
}

template<typename T>
void QuickSort<T>::sort(T *pFirstEl, int length) {
    countSwaps = 0;
    countComparisons = 0;
    countPartitions = 0;
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    partition(pFirstEl, pFirstEl + length-1);
    gettimeofday(&stop, NULL);
    std::cout << "Finished in: " << (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec
    << "us Swaps: " << countSwaps << " Comparisons: " << countComparisons <<
    " Partitions: " << countPartitions << "\n";
}

template<typename T>
std::string QuickSort<T>::toString(T* pFirstEl, int length) {
    std::string s = "[ ";
    for (T* i = pFirstEl; i < pFirstEl + length; ++i) {
        s += std::to_string(*i);
        s+= " ";
    }
    s += "]";
    return s;
}


#endif //U6_GENERICS_QUICKSORT_H
