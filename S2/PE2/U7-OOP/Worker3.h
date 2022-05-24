//
// Created by arman on 24.05.2022.
//

#ifndef LIST_WORKER3_H
#define LIST_WORKER3_H

#include "Worker.h"

class Worker3: public Worker{
public:
    int actualIncome(){
        return this->standardIncome + (this->age - 23) * 50;
    }
};

#endif //LIST_WORKER3_H
