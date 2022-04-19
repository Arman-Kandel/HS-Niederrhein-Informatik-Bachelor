//
// Created by arman on 18.04.2022.
//

#ifndef LIST_GROWSPEED_H
#define LIST_GROWSPEED_H


#include <string>
#include <utility>

class GrowSpeed {
public:
    std::string name;
    std::string symbol;
    GrowSpeed(std::string name, std::string symbol) : name(std::move(name)), symbol(std::move(symbol)) {}
};


#endif //LIST_GROWSPEED_H
