//
// Created by arman on 24.05.2022.
//

#ifndef LIST_UTISSTRING_H
#define LIST_UTISSTRING_H

#include <stdio.h>
#include <string.h>
#include "string"
#include "vector"

using namespace std;

class UtilsString{
public:
    vector<string> split(string target, string delimiter) {
        vector<string> vec{};
        char* targetData = target.data();
        char* delimiterData = delimiter.data();
        char * pch = strtok(targetData, delimiterData);
        while (pch != NULL)
        {
            auto s = pch;
            vec.push_back(s);
            pch = strtok (NULL, delimiterData);
        }
        return vec;
    }
};

#endif //LIST_UTISSTRING_H
