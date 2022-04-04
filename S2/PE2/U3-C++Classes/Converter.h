//
// Created by arman on 28.03.2022.
//

#ifndef HS_NIEDERRHEIN_INFORMATIK_BACHELOR_CONVERTER_H
#define HS_NIEDERRHEIN_INFORMATIK_BACHELOR_CONVERTER_H


class Converter {
public :
    double toMiles(double kilometers);

    double toKilometers(double miles);

    void printTable(double kilometers[], int length);
};

#endif //HS_NIEDERRHEIN_INFORMATIK_BACHELOR_CONVERTER_H
