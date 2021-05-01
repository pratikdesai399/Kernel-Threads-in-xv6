#include "threadlib.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "x86.h"


THREAD create_thread(void(*start)(void *), void *arg, int flags){
    int stack_address;
    thread_lock lock;
    lock_init(&lock);
    
    void* stack = malloc(4096);
    lock_acquire(&lock);
    if((uint)stack % 4096 != 0){
        stack = malloc(4096);
    }
    stack_address = (uint)stack;
    lock_release(&lock);
    int ret = clone(start, arg, stack,flags);

    THREAD thread;
    thread.pid = ret;
    thread.stack = (void*)stack_address;
    return thread;

}

int join_threads(THREAD t){
    if(join(t.pid) != t.pid){
        free(t.stack);
        return -1;
    }
    free(t.stack);
    return t.pid;
}

void lock_init(thread_lock *l){
    l->isHeld = 0;
}

void lock_acquire(thread_lock *l){
    //Reference spinlock.c
    while (xchg(&l->isHeld, 1) != 0)
        ;
    __sync_synchronize();
}

void lock_release(thread_lock *l){
    //Reference spinlock.c
    __sync_synchronize();
    xchg(&l->isHeld, 0);
}
