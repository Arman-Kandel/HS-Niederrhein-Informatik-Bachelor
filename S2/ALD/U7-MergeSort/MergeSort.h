//
// Created by arman on 25.04.2022.
//

#ifndef U6_GENERICS_QUICKSORT_H
#define U6_GENERICS_QUICKSORT_H
#include <sys/time.h>

template<typename T>
class MergeSort {
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
    void merge(T array[], int const left, int const right);
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
    long countSplits = 0;
    bool printSteps = false;
};

template<typename T>
void MergeSort<T>::swap(T *pEl1, T *pEl2) {
    T temp = *pEl1;
    *pEl1 = *pEl2;
    *pEl2 = temp;
    countSwaps++;
    if(printSteps) std::cout << "swap(" << *pEl1 <<", "<< *pEl2 <<")" << std::endl;
}

template<typename T>
void MergeSort<T>::merge(T array[], int const left, int const right)
{
    if (left >= right)
        return; // Returns recursively

    int length = (right - left + 1 );
    int mid = length / 2;

    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;

    // Create temp arrays
    auto *leftArray = new int[subArrayOne],
            *rightArray = new int[subArrayTwo];

    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];

    auto indexOfSubArrayOne = 0, // Initial index of first sub-array
    indexOfSubArrayTwo = 0; // Initial index of second sub-array
    int indexOfMergedArray = left; // Initial index of merged array

    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne] <= rightArray[indexOfSubArrayTwo]) {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }


    merge(array, left, mid);
    merge(array, mid + 1, length-1);
    merge(array, left,length-1);
}

template<typename T>
void MergeSort<T>::sort(T *pFirstEl, int length) {
    countSwaps = 0;
    countComparisons = 0;
    countSplits = 0;
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    merge(pFirstEl, 0, length-1);
    gettimeofday(&stop, NULL);
    std::cout << "Finished in: " << (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec
              << "us Swaps: " << countSwaps << " Comparisons: " << countComparisons <<
              " Splits: " << countSplits << "\n";
}

template<typename T>
std::string MergeSort<T>::toString(T* pFirstEl, int length) {
    std::string s = "[ ";
    for (T* i = pFirstEl; i < pFirstEl + length; ++i) {
        s += std::to_string(*i);
        s+= " ";
    }
    s += "]";
    return s;
}


#endif //U6_GENERICS_QUICKSORT_H
