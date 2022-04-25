//
// Created by arman on 06.04.2022.
//

#ifndef U4_STREAMS_SALE_H
#define U4_STREAMS_SALE_H

#include "string"
#include "vector"

using namespace std;

class Sale {
public:
    /**
     * Integer in following format expected: YYYYMMDD
     */
    string* dateFormat;
    string* carName;
    string* countCarsSold;
    vector<Sale*> sales = vector<Sale *>();

    Sale();
    Sale(string *dateFormat, string *carName, string *countCarsSold);

    void loadFile(string filePath);
    void print();
};


#endif //U4_STREAMS_SALE_H
