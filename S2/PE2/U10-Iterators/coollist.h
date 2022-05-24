//
// Created by arman on 23.05.2022.
//

#ifndef U10_ITERATORS_COOLLIST_H
#define U10_ITERATORS_COOLLIST_H
#include <vector>
#include <map>

template <class _type>
class coollist: public std::vector<_type> {
public:
    std::map<int, _type> getAsMap();
};

template<class _type>
std::map<int, _type> coollist<_type>::getAsMap() {
    auto myMap = std::map<int, _type>();
    int a = 0;
    // Using a for loop with iterator
    for(auto it = std::begin(this); it != std::end(this); ++it) {
        myMap.insert(std::pair<int, _type>(a, *it));
        a++;
    }
    return myMap;
}

#endif //U10_ITERATORS_COOLLIST_H
