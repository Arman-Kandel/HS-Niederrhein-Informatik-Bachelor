//
// Created by arman on 23.05.2022.
//

#include "coollist.h"

template<class type>
std::map<int, type> coollist<type>::getAsMap() {
    auto myMap = std::map<int, type>();
    int a = 0;
    // Using a for loop with iterator
    for(auto it = std::begin(this); it != std::end(this); ++it) {
        myMap.insert(std::pair<int, type>(a, *it));
        a++;
    }
    return myMap;
}