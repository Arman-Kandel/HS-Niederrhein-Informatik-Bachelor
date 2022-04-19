//
// Created by arman on 18.04.2022.
//

#ifndef LIST_COMPLEXITY_H
#define LIST_COMPLEXITY_H

#include <ostream>
#include "GrowSpeed.h"

class Complexity {
private:
    GrowSpeed slower("SLOWER", "");
    GrowSpeed same("SAME", "");
    GrowSpeed faster("FASTER", "");
    GrowSpeed aLotFaster("A_LOT_FASTER", "");
    GrowSpeed notAlwaysSlower("NOT_ALWAYS_SLOWER", "");
    GrowSpeed notReallySlower("NOT_REALLY_SLOWER", "");
public:
    GrowSpeed growSpeed;
    friend std::ostream &operator<<(std::ostream &os, const Complexity &complexity);

};


#endif //LIST_COMPLEXITY_H
