//#define TEST // Uncomment to run in test mode
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "my-utils.h"
#ifndef _WIN32
#include "wiringPi.h"
#endif

// Constant globals
const int maxLoopCount = 2;
const int maxDistance = 4;
const int minDistance = 1;
// Note that the globals below should
// only be read from other threads:
double latestDistance = 0;

// Pins for stepper motor:
const int pinMotor1AD4 = 6;
const int pinMotor1BD5 = 10;
const int pinMotor2AD6 = 11;
const int pinMotor2BD7 = 31;
// Pins for super sonic sensor:
const int pinSuperSonicEcho = 21;
const int pinSuperSonicTrigger = 22;
// Pins for LED matrix:
const int pinMatrixDIN = 27;
const int pinMatrixLOAD = 28;
const int pinMatrixCLK = 29;


void setMotorBits(bool b0, bool b1, bool b2, bool b3){
    //printf("Motor state: %d%d%d%d\n", b0, b1, b2, b3);
#ifndef TEST
    digitalWrite(pinMotor1AD4, b0);
    digitalWrite(pinMotor1BD5, b1);
    digitalWrite(pinMotor2AD6, b2);
    digitalWrite(pinMotor2BD7, b3);
#endif
}
void setMotorState(int state){
    switch (state) {
        case 0:
            setMotorBits(1, 0, 0, 0); return;
        case 1:
            setMotorBits(1, 1, 0, 0); return;
        case 2:
            setMotorBits(0, 1, 0, 0); return;
        case 3:
            setMotorBits(0, 1, 1, 0); return;
        case 4:
            setMotorBits(0, 0, 1, 0); return;
        case 5:
            setMotorBits(0, 0, 1, 1); return;
        case 6:
            setMotorBits(0, 0, 0, 1); return;
        case 7:
            setMotorBits(1, 0, 0, 1); return;
    }
}

/**
 * @return distance in meters, determined by the ultrasonic sensor.
 */
double getDistance(){
#ifdef TEST
    int min = 0, max = 5;
    return rand() % (max-min + 1) + min;
#else
    digitalWrite(pinSuperSonicTrigger, 1);
    sleep_ms(10);
    digitalWrite(pinSuperSonicTrigger, 0);
    while(digitalRead(pinSuperSonicEcho) == 0);
    long long time1 = getCurrentTime();
    while(digitalRead(pinSuperSonicEcho) == 1);
    long long time2 = getCurrentTime();
    // Die Schallgeschwindigkeit in trockener Luft von 20 °C beträgt 343,2 m/s (1236 km/h)
    // also 0,3432 m/ms
    long long timeTook = time2 - time1;
    return (timeTook * 0.3432) / 2;
#endif
}

void ledSetBit(bool bit){
    digitalWrite(pinMatrixDIN, bit);
    digitalWrite(pinMatrixCLK, 1);
    sleep_ms(1);
    digitalWrite(pinMatrixCLK, 0);
    sleep_ms(1);
}

/**
 * Send 16 bit to LEDs MAX7219,
 * where the first 8 are the address and the last 8 the data. <br>
 * Actually you only need 4 bits for the address (params a1-a4)
 * and the data has still 8 bits (params d1-d8).
 */
void ledSetBits(bool a1, bool a2, bool a3, bool a4, bool d1, bool d2, bool d3, bool d4, bool d5, bool d6, bool d7, bool d8){
#ifdef TEST
    printf("address: %c data: %c\n",address, data);
#else
    digitalWrite(pinMatrixLOAD, 0);
    for (int i = 0; i < 4; ++i) {
        ledSetBit(0);
    }
    ledSetBit(a1);
    ledSetBit(a2);
    ledSetBit(a3);
    ledSetBit(a4);
    ledSetBit(d1);
    ledSetBit(d2);
    ledSetBit(d3);
    ledSetBit(d4);
    ledSetBit(d5);
    ledSetBit(d6);
    ledSetBit(d7);
    ledSetBit(d8);
    digitalWrite(pinMatrixLOAD, 1);
    printf("[LEDS] Set LEDs at %d%d%d%d to %d%d%d%d%d%d%d%d Status: %s\n", a1,a2,a3,a4,  d1,d2,d3,d4,d5,d6,d7,d8, strerror(errno));
#endif
}
void disableLEDMatrix(){
    printf("[LED_MATRIX] Disable LEDs.\n");
#ifndef TEST
    ledSetBits(0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0); // Digit 0 address: 0001
    ledSetBits(0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0); // Digit 1 address: 0010
    ledSetBits(0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0); // Digit 2 address: 0011
    ledSetBits(0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // Digit 3 address: 0100
    ledSetBits(0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0); // Digit 4 address: 0101
    ledSetBits(0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0); // Digit 5 address: 0110
    ledSetBits(0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0); // Digit 6 address: 0111
    ledSetBits(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // Digit 7 address: 1000
#endif
}
void showMediumFace(){
    printf("[LED_MATRIX] Medium face.\n");
#ifndef TEST
    ledSetBits(0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1); // Digit 0 address: 0001
    ledSetBits(0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1); // Digit 1 address: 0010
    ledSetBits(0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1); // Digit 2 address: 0011
    ledSetBits(0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1); // Digit 3 address: 0100
    ledSetBits(0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1); // Digit 4 address: 0101
    ledSetBits(0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1); // Digit 5 address: 0110
    ledSetBits(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1); // Digit 6 address: 0111
    ledSetBits(1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1); // Digit 7 address: 1000
#endif
}
void showHappyFace(){
    printf("[LED_MATRIX] Happy face.\n");
#ifndef TEST
    ledSetBits(0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1); // Digit 0 address: 0001
    ledSetBits(0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1); // Digit 1 address: 0010
    ledSetBits(0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1); // Digit 2 address: 0011
    ledSetBits(0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1); // Digit 3 address: 0100
    ledSetBits(0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1); // Digit 4 address: 0101
    ledSetBits(0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1); // Digit 5 address: 0110
    ledSetBits(0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1); // Digit 6 address: 0111
    ledSetBits(1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1); // Digit 7 address: 1000
#endif
}
void showSadFace(){
    printf("[LED_MATRIX] Sad face.\n");
#ifndef TEST
    ledSetBits(0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0); // Digit 0 address: 0001
    ledSetBits(0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0); // Digit 1 address: 0010
    ledSetBits(0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0); // Digit 2 address: 0011
    ledSetBits(0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0); // Digit 3 address: 0100
    ledSetBits(0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0); // Digit 4 address: 0101
    ledSetBits(0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0); // Digit 5 address: 0110
    ledSetBits(0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0); // Digit 6 address: 0111
    ledSetBits(1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0); // Digit 7 address: 1000
#endif
}

void runT1(){ // Stepper motor

    for (int i = 0; i < maxLoopCount; ++i) {
        int state = 0;
        for (int j = 0; j < 3000; ++j) {
            setMotorState(state);
            state++;
            if(state >= 8) state = 0;
            sleep_ms(1);
        }

        sleep_ms(1000);
    }
}
void runT2(){ // Ultrasonic sensor
    for (int i = 0; i < maxLoopCount; ++i) {
        double distance = getDistance();
        latestDistance = distance;
        printf("[T2-%d/SUPER_SONIC_SENSOR] %f meters Status: %s\n", i, latestDistance, strerror(errno));
        sleep_ms(1000);
    }
}
void runT3(){ // LEDs matrix
    ledSetBits(1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // Set shutdown operation
    ledSetBits(1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1); // Set normal operation
    ledSetBits(1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1); // Set scan limit to all digits
    ledSetBits(1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0); // Set no decode mode, to set each segment
    ledSetBits(1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1); // Set max intensity
    for (int i = 0; i < maxLoopCount; ++i) {
        if(latestDistance <= maxDistance && latestDistance >= minDistance) showMediumFace();
        else if(latestDistance > maxDistance) showHappyFace();
        else showSadFace();
        sleep_ms(1000);
    }
    ledSetBits(1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1); // Set min intensity
    disableLEDMatrix();
    ledSetBits(1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // Set shutdown operation
}

#ifdef _WIN32 // do Windows-specific stuff
#include <windows.h>

DWORD WINAPI winT1Run(void* data) {
    runT1();
    return 0;
}
DWORD WINAPI winT2Run(void* data) {
    runT2();
    return 0;
}
DWORD WINAPI winT3Run(void* data) {
    runT3();
    return 0;
}

int initWinThreads() {
    HANDLE t1 = CreateThread(NULL, 0, winT1Run, NULL, 0, NULL);
    if (!t1) {printf("Failed to start t1!\n");return -1;}
    HANDLE t2 = CreateThread(NULL, 0, winT2Run, NULL, 0, NULL);
    if (!t2) {printf("Failed to start t2!\n");return -1;}
    HANDLE t3 = CreateThread(NULL, 0, winT3Run, NULL, 0, NULL);
    if (!t3) {printf("Failed to start t3!\n");return -1;}

    WaitForSingleObject(t1,INFINITE);
    WaitForSingleObject(t2,INFINITE);
    WaitForSingleObject(t3,INFINITE);
}

#else // do Unix-specific stuff
#include <pthread.h>
#include <unistd.h>

void *unixT1Run(void*args) {
    runT1();
}
void *unixT2Run(void*args) {
    runT2();
}
void *unixT3Run(void*args) {
    runT3();
}

int initUnixThreads() {
    pthread_t t1;
    pthread_create(&t1, NULL, unixT1Run, NULL);
    pthread_t t2;
    pthread_create(&t2, NULL, unixT2Run, NULL);
    pthread_t t3;
    pthread_create(&t3, NULL, unixT3Run, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    return 0;
}
#endif

int main(){
#ifdef TEST
    printf("Note that you are currently running in test mode!\n");
#else
    wiringPiSetup();
    // Set pin mode for motor pins:
    pinMode(pinMotor1AD4, OUTPUT);
    pinMode(pinMotor1BD5, OUTPUT);
    pinMode(pinMotor2AD6, OUTPUT);
    pinMode(pinMotor2BD7, OUTPUT);
    // Set pin mode for super sonic sensor pins:
    pinMode(pinSuperSonicEcho, INPUT);
    pinMode(pinSuperSonicTrigger, OUTPUT);
    // Set pin mode for LED matrix:
    pinMode(pinMatrixDIN, OUTPUT);
    pinMode(pinMatrixLOAD, OUTPUT);
    pinMode(pinMatrixCLK, OUTPUT);
    // Small LEDs for status
    pinMode(0, OUTPUT);
    digitalWrite(0, 1);
#endif
#ifdef _WIN32
    initWinThreads();
#else
    initUnixThreads();
#endif
    digitalWrite(0, 0);
}


