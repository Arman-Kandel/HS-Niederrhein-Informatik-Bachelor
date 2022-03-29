#include <iostream>
#include <list>
#include "DLinkedList.h"

int main(int argc, char *argv[])
{
    std::cout << "Args count: " << argc << std::endl;
    for (char** i = argv; i < argv+argc; ++i) {
        std::cout << &i << std::endl;
    }
    std::list<int> mylist;
    mylist.push_back(10);
    mylist.push_back(13);
    mylist.push_back(12);

    std::list<int>::iterator it;
    for (it = mylist.begin(); it != mylist.end(); ++it)
        std::cout << '\t' << *it;
    std::cout << '\n';

    DLinkedList<int> anotherList;
    anotherList.add(10);
    return 0;
}