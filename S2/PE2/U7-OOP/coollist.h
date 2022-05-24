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

#endif //U10_ITERATORS_COOLLIST_H
