// https://stackoverflow.com/a/28827188/13600212
#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#ifdef _WIN32 // do Windows-specific stuff
#include <windows.h>
#include <process.h>
#define MUTEX HANDLE
#else // do Unix-specific stuff
#include <unistd.h>
#define MUTEX pthread_mutex_t
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

/**
 * Runs the provided functions
 * in new cross-platform threads (windows threads and POSIX thread supported),
 * does NOT wait for threads to finish.
 * One thread per function.
 * @return != 0 value if error.
 */
int runAsync(int count, void* functions[]){
    for (int i = 0; i < count; ++i) {
#ifdef _WIN32 // do Windows-specific stuff
        HANDLE t1 = CreateThread(NULL, 0, functions[i], NULL, 0, NULL);
        if (!t1) {printf("Failed to start t1!\n");return -1;}
#else // do Unix-specific stuff
        pthread_t t1;
        int result = pthread_create(&t1, NULL, functions[i], NULL);
        if(result != 0) return result;
#endif
    }
    return 0;
}

/**
 * Runs the provided functions
 * in new cross-platform threads (windows threads and POSIX thread supported),
 * does wait for all threads to finish.
 * One thread per function.
 * @return != 0 value if error.
 */
int runAsyncJoin(int count, void* functions[]){

    // INIT THREADS ARRAY
#ifdef _WIN32 // do Windows-specific stuff
    HANDLE array[count];
#else // do Unix-specific stuff
    pthread_t array[count];
#endif

    // CREATE THREADS FOR EACH FUNCTION
    for (int i = 0; i < count; ++i) {
#ifdef _WIN32 // do Windows-specific stuff
        HANDLE t1 = CreateThread(NULL, 0, functions[i], NULL, 0, NULL);
        if (!t1) {printf("Failed to start t1!\n");return -1;}
        array[i] = t1;
#else // do Unix-specific stuff
        pthread_t t1;
        int result = pthread_create(&t1, NULL, functions[i], NULL);
        if(result != 0) return result;
        array[i] = t1;
#endif
    }

    // JOIN EACH THREAD
    for (int i = 0; i < count; ++i) {
#ifdef _WIN32 // do Windows-specific stuff
        WaitForSingleObject(array[i],INFINITE);
#else // do Unix-specific stuff
        pthread_join(array[i], NULL);
#endif
    }

}

int initLock(MUTEX *mutex){
    if (mutex == NULL) return 1;
#ifdef _WIN32 // do Windows-specific stuff
    InitializeCriticalSection(mutex);
#else // do Unix-specific stuff
    return pthread_mutex_init(mutex, NULL);
#endif
    return 0;
}

/**
 * @return an cross-platform, initialized mutex/lock.
 */
MUTEX newLock(){
#ifdef _WIN32 // do Windows-specific stuff
    return CreateMutex(0, FALSE, 0);
#else // do Unix-specific stuff
    MUTEX mutex = PTHREAD_MUTEX_INITIALIZER;
    initLock(&mutex);
    return mutex;
#endif
}

int lockDestroy(MUTEX *mutex){
    if (mutex == NULL)return 1;
#ifdef _WIN32 // do Windows-specific stuff
    DeleteCriticalSection(mutex);
    return 0;
#else // do Unix-specific stuff
    return pthread_mutex_destroy(mutex);
#endif
}

int lock(MUTEX *mutex)
{
    if (mutex == NULL)return 1;
#ifdef _WIN32 // do Windows-specific stuff
    EnterCriticalSection(mutex);
    return 0;
#else // do Unix-specific stuff
    return pthread_mutex_lock(mutex);
#endif
}

int unlock(MUTEX *mutex)
{
    if (mutex == NULL)return 1;
#ifdef _WIN32 // do Windows-specific stuff
    LeaveCriticalSection(mutex);
    return 0;
#else // do Unix-specific stuff
    return pthread_mutex_unlock(mutex);
#endif
}

