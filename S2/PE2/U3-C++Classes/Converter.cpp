//
// Created by arman on 28.03.2022.
//

#include "iostream"
#include "Converter.h"

// 1km = 0.621371 miles
// 1 mile = 1 + (1 - 0.621371) km

double Converter::toMiles(double kilometers) {
    return kilometers / 0.621371;
}

double Converter::toKilometers(double miles) {
    double a = 1 - (1 - 0.621371); // 1mile = 1-(1-0.6miles)
    return miles * a;
}

void Converter::printTable(double *kilometers, int length) {
    for (double *i = kilometers; i < kilometers + length; ++i) {
        double a = *i;
        std::cout << a << "km = " << toMiles(a) << "miles\n";
    }
}
