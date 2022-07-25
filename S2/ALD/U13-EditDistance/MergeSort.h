//
// Created by arman on 25.04.2022.
//

#ifndef U6_GENERICS_QUICKSORT_H
#define U6_GENERICS_QUICKSORT_H
#include <cmath>
using namespace std;

template <typename T> T min(T a, T b, T c){
    return min(min(a, b), c);
}

int editDistance(string s1, string s2){
    string smallerString, biggerString;
    if (s1.length() < s2.length()) {
        smallerString = s1;
        biggerString = s2;
    } else {
        smallerString = s2;
        biggerString = s1;
    }

    int smallerMax = smallerString.length() + 1;
    int biggerMax = biggerString.length() + 1;

    int colBefore[biggerMax];
    int colNow[biggerMax];

    // Fill values with 0 to max for colBefore
    for (int i = 0; i < biggerMax; i++) {
        colBefore[i] = i;
    }

    // Do algorithm with masks

    for (int iSmall = 1; iSmall < smallerMax; iSmall++) { // i=1 to skip 0 index
        colNow[0] = iSmall;
        for (int iBig = 1; iBig < biggerMax; iBig++) { // i=1 to skip 0 index

            if (smallerString[iSmall -1] == biggerString[iBig - 1])
                colNow[iBig] = min(colNow[iBig-1] + 1, colBefore[iBig] + 1, colBefore[iBig-1]);
            else
                colNow[iBig] = 1 + min(colNow[iBig-1], colBefore[iBig], colBefore[iBig-1]);
        }
        // Copy contents of current array to before array
        for (int i = 0; i < biggerMax; i++) {
            colBefore[i] = colNow[i];
        }
    }

    return colNow[biggerMax - 1];
}


#endif //U6_GENERICS_QUICKSORT_H
