#include "threadlib.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"


THREAD create_thread(void(*start)(void *), void *arg, int flags){
    int stack_addr;
    
    void* stack = malloc(4096);
    stack_addr = (uint)stack;
    int ret = clone(start, arg, stack,flags);

    THREAD thread;
    thread.pid = ret;
    thread.stack = (void*)stack_addr;
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

