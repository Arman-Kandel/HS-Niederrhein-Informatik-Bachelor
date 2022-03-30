//
// Created by arman on 28.03.2022.
//

#ifndef HS_NIEDERRHEIN_INFORMATIK_BACHELOR_CONVERTER_H
#define HS_NIEDERRHEIN_INFORMATIK_BACHELOR_CONVERTER_H


class Converter {
        public :
        Converter(double[] miles, double[] km);
        ~ Converter();
        void printTable();
        int getValueAt (int pos );
        void erase (int val );
        void toScreen ();
        char getError ();
};




#endif //HS_NIEDERRHEIN_INFORMATIK_BACHELOR_CONVERTER_H
