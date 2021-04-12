#include "types.h"
#include "stat.h"
#include "user.h"

typedef struct THREAD{
    int pid;
    void* stack;
}THREAD;

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


void test1(void* arg){
    printf(1,"IN THREAD\n");
      
    exit();

}

    


int main(int argc, char *argv[])
{
    printf(1,"In main\n");
    THREAD th = create_thread(test1, 0);
    printf(1,"Thread Created\n");
    //join_threads(th);
    join(th.pid);
    printf(1,"Threads Joined\n");

    
    printf(1, "RETURN VALUE FOR CLONE : %d\n", th.pid);
    exit();
}