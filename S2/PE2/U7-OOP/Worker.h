//
// Created by arman on 24.05.2022.
//

#ifndef LIST_WORKER_H
#define LIST_WORKER_H

#include "string"

using namespace std;

class Worker{
public:
    int id;
    string name;
    int age;
    int standardIncome;
    int bonusIncome;
    int actualIncome(){
        return this->standardIncome;
    }
};

#endif //LIST_WORKER_H
