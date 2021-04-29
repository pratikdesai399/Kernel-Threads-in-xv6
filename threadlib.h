#ifndef THREADLIB
#include "types.h"

typedef struct THREAD{
    int pid;
    void* stack;
}THREAD;

THREAD create_thread(void (*start)(void *), void *arg, int flags);
int join_threads(THREAD t);


#endif