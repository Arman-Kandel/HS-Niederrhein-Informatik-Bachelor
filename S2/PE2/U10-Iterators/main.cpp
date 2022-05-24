#include <iostream>
#include <string>
#include "coollist.h"


int main(int argc, char *argv[]) {
    auto cl = coollist<std::string>();
    cl[0] = "John";
    cl[1] = "Cena";
    auto map = cl.getAsMap();
    for (const auto &item : map){
        std::cout << item.first << " " << item.second << "\n";
    }
    return 0;
}