#define TEST // Uncomment to run in test mode
#include <stdio.h>
#include "wiringPi.h"
#include "my-utils.h"

// Constant globals
const int maxLoopCount = 10;
const int maxDistance = 1000;
const int minDistance = 9;
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
    long long time1 = getCurrentTime();
    while(digitalRead(pinSuperSonicEcho) == 1);
    long long time2 = getCurrentTime();
    // Die Schallgeschwindigkeit in trockener Luft von 20 °C beträgt 343,2 m/s (1236 km/h)
    // also 0,3432 m/ms
    long long timeTook = time2 - time1;
    return (timeTook * 0.3432) / 2;
#endif
}

/**
 * Send 16 bit to LEDs MAX7219,
 * where the first 8 are the address and the last 8 the data.
 */
void showData(unsigned char address, unsigned char data){
    digitalWrite(pinMatrixLOAD, 0);
    for (int i = 0; i < 8; ++i) {
        if (address & 0x01) digitalWrite(pinMatrixDIN, 1);
        else digitalWrite(pinMatrixDIN, 0);
        digitalWrite(pinMatrixCLK, 1);
        sleep_ms(1);
        digitalWrite(pinMatrixCLK, 0);
        address = address >> 1;
    }
    for (int i = 0; i < 8; ++i) {
        if (data & 0x01) digitalWrite(pinMatrixDIN, 1);
        else digitalWrite(pinMatrixDIN, 0);
        digitalWrite(pinMatrixCLK, 1);
        sleep_ms(1);
        digitalWrite(pinMatrixCLK, 0);
        data = data >> 1;
    }
    digitalWrite(pinMatrixLOAD, 1);
}

void showMediumFace(){
#ifdef TEST
    printf("[LED_MATRIX] Medium face.");
#else
    showData(0b00000000, 0b11111111);
#endif

}
void showHappyFace(){
#ifdef TEST
    printf("[LED_MATRIX] Happy face.");
#else
    showData(0b00000000, 0b11111111);
#endif
}
void showSadFace(){
#ifdef TEST
    printf("[LED_MATRIX] Sad face.");
#else
    showData(0b00000000, 0b11111111);
#endif
}

void runT1(){ // Schrittmotor

}
void runT2(){ // Ultraschall
    for (int i = 0; i < maxLoopCount; ++i) {
        double distance = getDistance();
        latestDistance = distance;
        printf("[T2/SUPER_SONIC_SENSOR] %f meters", latestDistance);
        sleep_ms(1000);
    }
}
void runT3(){ // LEDs
    for (int i = 0; i < maxLoopCount; ++i) {
        if(latestDistance <= maxDistance && latestDistance >= minDistance) showMediumFace();
        else if(latestDistance > maxDistance) showHappyFace();
        else showSadFace();
        sleep_ms(1000);
    }
}

#ifdef _WIN32 // do Windows-specific stuff
#include <windows.h>
#include <unistd.h>

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
    if (!t1) {printf("Failed to start t1!");return -1;}
    HANDLE t2 = CreateThread(NULL, 0, winT2Run, NULL, 0, NULL);
    if (!t2) {printf("Failed to start t2!");return -1;}
    HANDLE t3 = CreateThread(NULL, 0, winT3Run, NULL, 0, NULL);
    if (!t3) {printf("Failed to start t3!");return -1;}

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
    pinMode(GPIO-Nr., OUTPUT);
#endif
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



    // write only on output pins, and read only on input pins
    digitalWrite(GPIO-Nr., 0);

#ifdef _WIN32
    initWinThreads();
#else
    initUnixThreads();
#endif
}


