#define TEST // Uncomment to run in test mode
#include <stdio.h>

#ifdef _WIN32 // do Windows-specific stuff
#include <windows.h>
#include <unistd.h>

const int maxLoopCount = 10;

DWORD WINAPI t1Run(void* data) {
    // START SCHRITTMOTOR
    for (int i = 0; i < maxLoopCount; ++i) {
    }
    #if defined TEST

    #else

    #endif
    return 0;
}
DWORD WINAPI t2Run(void* data) {
    // START ULTRASCHALL
    for (int i = 0; i < 10; ++i) {
        printf("Hi t2!\n");
        sleep(1);
    }
    return 0;
}
DWORD WINAPI t3Run(void* data) {
    // START LEDs
    for (int i = 0; i < 10; ++i) {
        printf("Hi t3!\n");
        sleep(1);
    }
    return 0;
}

int main() {
    HANDLE t1 = CreateThread(NULL, 0, t1Run, NULL, 0, NULL);
    if (!t1) {printf("Failed to start t1!");return -1;}
    HANDLE t2 = CreateThread(NULL, 0, t2Run, NULL, 0, NULL);
    if (!t2) {printf("Failed to start t2!");return -1;}
    HANDLE t3 = CreateThread(NULL, 0, t3Run, NULL, 0, NULL);
    if (!t3) {printf("Failed to start t3!");return -1;}

    WaitForSingleObject(t1,INFINITE);
    WaitForSingleObject(t2,INFINITE);
    WaitForSingleObject(t3,INFINITE);
}

#else // do Unix-specific stuff
#include <pthread.h>
#include <unistd.h>

void *thread1Run(void*args) {
    for (int i = 0; i < 10; ++i) {
        printf("Hi!\n");
        sleep(1);
    }
}

void *thread2Run(void*args) {
}

void *thread3Run(void*args) {
}

int main() {
    pthread_t t1;
    pthread_create(&t1, NULL, thread1Run, NULL);
    pthread_t t2;
    pthread_create(&t2, NULL, thread1Run, NULL);
    pthread_t t3;
    pthread_create(&t3, NULL, thread1Run, NULL);
    sleep(10);
    return 0;
}
#endif


