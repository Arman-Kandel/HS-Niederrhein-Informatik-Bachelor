//
// Created by arman on 24.05.2022.
//

#ifndef LIST_WORKER2_H
#define LIST_WORKER2_H


#include "Worker.h"

class Worker2: public Worker {
public:
    int actualIncome(){
        return this->standardIncome + bonusIncome;
    }
};


#endif //LIST_WORKER2_H
