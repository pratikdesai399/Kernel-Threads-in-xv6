#ifndef THREADLIB
#include "types.h"

typedef struct THREAD{
    int pid;
    void* stack;
}THREAD;

typedef struct thread_lock{
    uint isHeld;
}thread_lock;

THREAD create_thread(void (*start)(void *), void *arg, int flags);
int join_threads(THREAD t);
void lock_init(thread_lock* l);
void lock_acquire(thread_lock *l);
void lock_release(thread_lock *l);

#endif