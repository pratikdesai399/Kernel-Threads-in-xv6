#include "types.h"
#include "stat.h"
#include "user.h"
#include "threadlib.h"

// void test2(void* arg){
//     char *str = (char*)arg;
//     printf(1,"IN SECOND THREAD\n");
//     printf(1,"SECOND STRING : %s\n", str);
//     exit();
// }

void test1(void* arg){
    int * no = (int*)arg;
    //printf(1,"IN THREAD\n");
    printf(1,"%d\n", no);
    // THREAD t = create_thread(test2, "HELLO THREAD IN THREAD");
    // printf(1,"SECOND THREAD CREATED : %d\n", t.pid);
    // int ret = join_threads(t);
    // printf(1,"Threads Joined\n");
    // printf(1, "RETURN VALUE FOR TEST1 : %d\n", ret);
      
    exit();

}

    


int main(int argc, char *argv[])
{
    // //printf(1,"In main\n");
    // THREAD th = create_thread(test1, "PRATIK");
    // printf(1,"Thread Created\n");
    // //join_threads(th);
    // int ret = join_threads(th);
    // printf(1,"Threads Joined\n");
    // printf(1, "RET: %d\n", ret);

    THREAD threads[100];
    for(int i = 0; i < 100; i++){
        threads[i] = create_thread(test1, (void*)i);
        join_threads(threads[i]);
        printf(1,"PID: %d\n", threads[i].pid);

    }
    
    exit();
}

