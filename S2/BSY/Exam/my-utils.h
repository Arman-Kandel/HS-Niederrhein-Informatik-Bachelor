// https://stackoverflow.com/a/28827188/13600212
#include <sys/time.h>
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif

void sleep_ms(int milliseconds){ // cross-platform sleep function
#ifdef WIN32
    Sleep(milliseconds);
#else
    struct timeval tv;
    tv.tv_sec = milliseconds / 1000;
    tv.tv_usec = milliseconds % 1000 * 1000;
    select(0, NULL, NULL, NULL, &tv);
#endif
}

/**
 * Time passed in milliseconds since 00:00 Coordinated Universal Time (UTC), January 1, 1970.
 */
long long getNowMillis() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec * 1000LL + t.tv_usec / 1000; // calc milliseconds
}

/**
 * Time passed in microseconds since 00:00 Coordinated Universal Time (UTC), January 1, 1970.
 */
long long getNowMicros(){
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec * 1000000LL + t.tv_usec; // calc micros
}



