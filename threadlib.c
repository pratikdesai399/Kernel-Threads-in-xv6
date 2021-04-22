#include "threadlib.h"
#include "types.h"
#include "stat.h"
#include "user.h"


THREAD create_thread(void (*start)(void *), void *arg){
    int stack_addr;
    void* stack = malloc(4096);
    stack_addr = (uint)stack;
    int ret = clone(start, arg, stack);

    THREAD thread;
    thread.pid = ret;
    thread.stack = (void*)stack_addr;
    return thread;

}

int join_threads(THREAD t){
    int ret = join(t.pid);
    return ret;
}

