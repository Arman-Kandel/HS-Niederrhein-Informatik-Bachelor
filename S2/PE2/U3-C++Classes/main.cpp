#include <iostream>
#include <list>
#include "Converter.h"
#include "Stack.h"

int main(int argc, char *argv[]) {
    // Converter:
    Converter c;
    // TODO outputs for 1 are switched, however outputs for over 1 are correct.
    std::cout << 1 << "km = " << c.toMiles(1) << "miles\n";
    std::cout << 1 << "mile = " << c.toKilometers(1) << "km\n\n";

    double a = 20;
    int length = 11;
    double arr[length];
    for (int i = 0; i < 11; ++i) {
        arr[i] = a;
        a += 5;
    }
    c.printTable(arr, length);

    // Stack:
    Stack<int> stack;
    stack.add(10);
    stack.add(25);
    stack.print();

    Stack<std::string> stackStrings;
    stackStrings.add("Hello!");
    stackStrings.add("AWESOME!");
    stackStrings.print();
    return 0;
}