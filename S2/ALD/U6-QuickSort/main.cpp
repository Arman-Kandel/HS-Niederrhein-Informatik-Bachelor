#include <iostream>
#include "QuickSort.h"

int main(int argc, char *argv[])
{
    QuickSort<int> quickSort{};
    quickSort.printSteps = true;
    int arr[8] = {10, 2, 11, 3, 877, 198, 10, 28};
    std::cout << "Before: " << quickSort.toString(arr, 8) << "\n";
    quickSort.sort(arr, 8);
    std::cout << "After: " << quickSort.toString(arr, 8) << "\n";
    return 0;
}