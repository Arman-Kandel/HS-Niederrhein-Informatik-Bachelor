#include <iostream>
#include "MergeSort.h"
#include <cmath>

int main(int argc, char *argv[])
{
    MergeSort<int> mergeSort{};
    mergeSort.printSteps = true;
    int arr[8] = {10, 2, 11, 3, 877, 198, 10, 28};
    std::cout << "Before: " << mergeSort.toString(arr, 8) << "\n";
    mergeSort.sort(arr, 8);
    std::cout << "After: " << mergeSort.toString(arr, 8) << "\n";
    // TODO fix segfault
    return 0;
}