/**
 * This program will have 3 running threads. <p>
 * First thread uses the stepper motor, to rotate. <p>
 * Second thread updates the latestDistance variable (distance from
 * sensor to the stepper motor), by
 * periodically calculating the distance via the supersonic sensor. <p>
 * Third thread reads the latestDistance variable and controls the LED-Matrix.
 * It displays either a neutral face, when latestDistance is
 * within min and max distance, or a sad face when smaller
 * than min distance, or a happy face when bigger than max distance.
 * @author Arman Kandel
 */
//#define TEST // Uncomment to run in test mode
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "my-utils.h"
#ifndef _WIN32
#include "wiringPi.h"
#endif

/**
 * A number from 0 to 3 that represents a “key”.
 * When another process/thread tries to lock the same key, it will be stalled
 * until the first process/thread has unlocked the same key.
 */
const int keyNum = 0;
void lock(){
    piLock(keyNum);
}
void unlock(){
    piUnlock(keyNum);
}

const int maxLoopCount = 10;
const int maxDistanceCm = 10;
const int minDistanceCm = 5;
/**
 * This is a global variable used across multiple threads,
 * but it should only be modified by one (can be read by
 * all threads without problems).
 * If you wanted to modify this on multiple threads, make sure
 * to acquire locks first like shown below:
 * @code
 #include <pthread.h>
 pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
 void *exampleFunc(void* param){
   pthread_mutex_lock(&mutex); // lock
   // modify variable
   pthread_mutex_unlock(&mutex); // unlock
 }
 */
int latestDistanceCm = 0;

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
    lock();
    digitalWrite(pinMotor1AD4, b0);
    digitalWrite(pinMotor1BD5, b1);
    digitalWrite(pinMotor2AD6, b2);
    digitalWrite(pinMotor2BD7, b3);
    unlock();
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
void stopMotor(){
    setMotorBits(0, 0, 0, 0);
    sleep_ms(1000);
}

/**
 * @return distance in centimeters, determined by the ultrasonic sensor.
 */
int getDistance(){
#ifdef TEST
    int min = 0, max = 5;
    return rand() % (max-min + 1) + min;
#else
    lock();
    digitalWrite(pinSuperSonicTrigger, 0);
    sleep_ms(5);
    digitalWrite(pinSuperSonicTrigger, 1);
    sleep_ms(10);
    digitalWrite(pinSuperSonicTrigger, 0);
    while(digitalRead(pinSuperSonicEcho) == 0);
    long long time1 = getNowMicros();
    while(digitalRead(pinSuperSonicEcho) == 1);
    long long time2 = getNowMicros();
    unlock();
    // Die Schallgeschwindigkeit in trockener Luft von 20 °C beträgt 343,2 m/s (1236 km/h)
    // also 0,3432 m/ms
    long long nanoSecondsTook = time2 - time1;
    return (int)(((nanoSecondsTook * (343.2 / 1000000)) / 2) * 100);
    // divide by 1000000 because second to microsecond
    // divide by 2 because sound bounces back
    // multiply with 100, m to cm
#endif
}

/**
 * Do not call directly, since this method does not lock the pi! <br>
 * This should only be called inside setLedBits().
 */
void setLEDBit(bool bit){
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
void setLEDBits(bool a1, bool a2, bool a3, bool a4, bool d1, bool d2, bool d3, bool d4, bool d5, bool d6, bool d7, bool d8){
#ifndef TEST
    lock();
    digitalWrite(pinMatrixLOAD, 0);
    for (int i = 0; i < 4; ++i) {
        setLEDBit(0);
    }
    setLEDBit(a1);
    setLEDBit(a2);
    setLEDBit(a3);
    setLEDBit(a4);
    setLEDBit(d1);
    setLEDBit(d2);
    setLEDBit(d3);
    setLEDBit(d4);
    setLEDBit(d5);
    setLEDBit(d6);
    setLEDBit(d7);
    setLEDBit(d8);
    digitalWrite(pinMatrixLOAD, 1);
    unlock();
    //printf("[LEDS] Set LEDs at %d%d%d%d to %d%d%d%d%d%d%d%d Status: %s\n", a1,a2,a3,a4,  d1,d2,d3,d4,d5,d6,d7,d8, strerror(errno));
#endif
}
void clearLEDMatrix(){
    setLEDBits(0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0); // Digit 0 address: 0001
    setLEDBits(0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0); // Digit 1 address: 0010
    setLEDBits(0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0); // Digit 2 address: 0011
    setLEDBits(0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // Digit 3 address: 0100
    setLEDBits(0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0); // Digit 4 address: 0101
    setLEDBits(0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0); // Digit 5 address: 0110
    setLEDBits(0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0); // Digit 6 address: 0111
    setLEDBits(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // Digit 7 address: 1000
}
void startLEDMatrix(){
    setLEDBits(1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1); // Set display-test to get out of shutdown mode
    sleep_ms(250);
    setLEDBits(1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0); // Set display-test to normal operation
    setLEDBits(1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1); // Set normal operation
    setLEDBits(1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1); // Set scan limit to all digits
    setLEDBits(1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0); // Set no decode mode, to set each segment
    setLEDBits(1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1); // Set max intensity
}
void stopLEDMatrix(){
    printf("[LED_MATRIX] Disable LEDs.\n");
#ifndef TEST
    clearLEDMatrix();
    setLEDBits(1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1); // Set min intensity
    setLEDBits(1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // Set shutdown operation
#endif
}
void showNeutralFace(){
    printf("[LED_MATRIX] Neutral face (now=%dcm min=%dcm max=%dcm).\n", latestDistanceCm, minDistanceCm, maxDistanceCm);
#ifndef TEST
    clearLEDMatrix();
    setLEDBits(0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0); // Digit 0 address: 0001
    setLEDBits(0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0); // Digit 1 address: 0010
    setLEDBits(0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0); // Digit 2 address: 0011
    setLEDBits(0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0); // Digit 3 address: 0100
    setLEDBits(0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0); // Digit 4 address: 0101
    setLEDBits(0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0); // Digit 5 address: 0110
    setLEDBits(0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0); // Digit 6 address: 0111
    setLEDBits(1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0); // Digit 7 address: 1000
#endif
}
void showHappyFace(){
    printf("[LED_MATRIX] Happy face (now=%dcm min=%dcm max=%dcm).\n", latestDistanceCm, minDistanceCm, maxDistanceCm);
#ifndef TEST
    clearLEDMatrix();
    setLEDBits(0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0); // Digit 0 address: 0001
    setLEDBits(0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0); // Digit 1 address: 0010
    setLEDBits(0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0); // Digit 2 address: 0011
    setLEDBits(0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0); // Digit 3 address: 0100
    setLEDBits(0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0); // Digit 4 address: 0101
    setLEDBits(0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0); // Digit 5 address: 0110
    setLEDBits(0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0); // Digit 6 address: 0111
    setLEDBits(1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0); // Digit 7 address: 1000
#endif
}
void showSadFace(){
    printf("[LED_MATRIX] Sad face (now=%dcm min=%dcm max=%dcm).\n", latestDistanceCm, minDistanceCm, maxDistanceCm);
#ifndef TEST
    clearLEDMatrix();
    setLEDBits(0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0); // Digit 0 address: 0001
    setLEDBits(0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0); // Digit 1 address: 0010
    setLEDBits(0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0); // Digit 2 address: 0011
    setLEDBits(0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0); // Digit 3 address: 0100
    setLEDBits(0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0); // Digit 4 address: 0101
    setLEDBits(0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0); // Digit 5 address: 0110
    setLEDBits(0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0); // Digit 6 address: 0111
    setLEDBits(1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0); // Digit 7 address: 1000
#endif
}

void runT1(){ // Stepper motor
    stopMotor();
    for (int i = 0; i < maxLoopCount; ++i) {
        int state = 0;
        for (int j = 0; j < 64; ++j) { // 360 degrees spin in 1 second
            setMotorState(state); // This performs 1/64 of the rotation
            state++;
            if(state >= 8) state = 0;
            sleep_ms(16); // 16 * 64 = 1000 ms = 1s for one turn
        }
        printf("[T2-%d/MOTOR] 360 degrees spin complete. Status: %s\n", i, strerror(errno));
    }
    stopMotor();
}
void runT2(){ // Ultrasonic sensor
    for (int i = 0; i < maxLoopCount * 4; ++i) {
        int distance = getDistance();
        latestDistanceCm = distance;
        //printf("[T2-%d/SUPER_SONIC_SENSOR] %d centimeters Status(%d): %s\n", i, latestDistance, errno, strerror(errno));
        sleep_ms(1000 / 4);
    }
}
void runT3(){ // LEDs matrix
    //setLEDBits(1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0); // Set shutdown operation
    startLEDMatrix();
    for (int i = 0; i < maxLoopCount; ++i) {
        if(latestDistanceCm <= maxDistanceCm && latestDistanceCm >= minDistanceCm) showNeutralFace();
        else if(latestDistanceCm > maxDistanceCm) showHappyFace();
        else showSadFace();
        sleep_ms(1000);
    }
    stopLEDMatrix();
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
    printf("[SETUP] Starting... Status(%d): %s\n", errno, strerror(errno));
    wiringPiSetup();
    printf("[SETUP] WiringPi setup complete. Status(%d): %s\n", errno, strerror(errno));
    errno = 0;
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
    printf("[SETUP] PIN modes set. Status(%d): %s\n", errno, strerror(errno));
    digitalWrite(0, 1);
#endif
#ifdef _WIN32
    initWinThreads();
#else
    initUnixThreads();
#endif
    digitalWrite(0, 0);
}


