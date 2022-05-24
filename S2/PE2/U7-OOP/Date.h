//
// Created by arman on 24.05.2022.
//

#ifndef LIST_DATE_H
#define LIST_DATE_H

#include <time.h>
#include "string"

using namespace std;

class Date{
public:
    time_t rawtime;
    const tm *timeinfo;
    static Date now(){
        Date d{};

        // Fetch current local time data
        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime );
        timeinfo = localtime(&rawtime);
        printf ( "Current local time and date: %s\n", asctime (timeinfo) );

        d.rawtime = rawtime;
        d.timeinfo = timeinfo;
    }

};

#endif //LIST_DATE_H
