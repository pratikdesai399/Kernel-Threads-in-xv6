#include "types.h"
#include "stat.h"
#include "user.h"
#include "threadlib.h"
#include "fcntl.h"

int A[4][4] = {{1,0,1,0},{0,1,1,0},{1,0,1,0},{0,1,1,0}};
int B[4][4] = {{0,2,2,0},{2,0,2,0},{0,2,2,0},{2,0,2,0}};
int C[4][4] = {{0}};

// COUNTER TESTING
volatile int globalCounter = 0;

void addToCounter(void* arg){
  globalCounter++;
  printf(1, "COUNTER IS: %x\n", globalCounter);
  exit();
}

void Counter(){
    printf(1,"\n\nCOUNTER TESTING\n");
    THREAD t2[10];
    printf(1, "RUNNING TESTS\n");
    globalCounter++;
    printf(1, "COUNTER BEFORE CLONE: %x\n", globalCounter);

    int i;
    for(i=0; i<10; i++){

        t2[i] = create_thread(addToCounter, 0, CLONE_VM);
        join_threads(t2[i]);
        printf(1, "PID: %d\n", t2[i].pid);
    }
    globalCounter++;
    printf(1, "JOINED\n");

    printf(1, "COUNTER SHOULD BE 12 AND IT IS = %d\n", globalCounter);
}
// void test2(void* arg){
//     char *str = (char*)arg;
//     printf(1,"IN SECOND THREAD\n");
//     printf(1,"SECOND STRING : %s\n", str);
//     exit();
// }

// void test1(void* arg){
//     int * no = (int*)arg;
//     //printf(1,"IN THREAD\n");
//     printf(1,"%d\n", no);
//     // THREAD t = create_thread(test2, "HELLO THREAD IN THREAD");
//     // printf(1,"SECOND THREAD CREATED : %d\n", t.pid);
//     // int ret = join_threads(t);
//     // printf(1,"Threads Joined\n");
//     // printf(1, "RETURN VALUE FOR TEST1 : %d\n", ret);
      
//     exit();

// }

void matrix_add(void* arg){
    int id = (int)arg;
    for(int i = id; i < 4; i+=2){
        for(int j = 0; j < 4; j++){
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    exit();

}

void Matrix(){
    THREAD t[2];
    for(int i = 0; i < 2; i++){
        t[i] = create_thread(matrix_add, (void*)i, CLONE_VM);
        join_threads(t[i]);
    }

    printf(1,"MATRIX PROGRAM\n");
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf(1, (j < 4-1) ? "%d " : "%d\n", C[i][j]);
        }
    }
    
}

// GET TID, TGID, AND PARENT ID
void id(void* arg){
    int tid = gettid();
    int parent_id = getppid();
    int tgid = gettgid();
    printf(1, "TID : %d\nParent_ID : %d\nTGID : %d\n", tid,parent_id, tgid);
    exit();
}

void Get_ids(){
    printf(1,"\n\nTHREAD ID FUNCTIONS\n");
    THREAD t1;
    t1 = create_thread(id, 0, CLONE_VM);
    join_threads(t1);
    
} 

//CLONE_VM TESTING
void test_files(void * arg){
    char buf[100];
    printf(1,"CLONE\n");
    int fd = (int)arg;
    read(fd, &buf, 100);
    write(1, &buf, 100);
    printf(1,"\nTHREAD COMPLETED\n");
    close(fd);
    exit();
}

void Clone_Vm(){
    printf(1,"\n\nCLONE VM TESTING\n");
    char buff[100];
    int fd = open("README", O_RDWR);
    // read(fd, &buff, 10);
    // write(1, &buff, 10);
    THREAD t = create_thread(test_files, (void*)fd, CLONE_VM || CLONE_FILES);
    join_threads(t);
    printf(1,"\nAFTER JOIN\n");
    read(fd, &buff, 100);
    write(1, &buff, 100);
}

//JOIN ORDER TEST
void jointestthread(void *a)
{
  int x = (int)a;
  sleep(x);
  exit();
}

void joinordertest(){
    THREAD tid1, tid2;
    int ret1, ret2;
    int one, two;
    one = 3;
    two = 1;
    tid1 = create_thread(jointestthread, (void*)one, CLONE_VM);
    tid2 = create_thread(jointestthread, (void*)two, CLONE_VM);
    ret1 = join_threads(tid1);
    ret2 = join_threads(tid2);
    if (ret1 == tid1.pid && ret2 == tid2.pid)
        printf(1, "\n\nJOIN ORDER TEST PASSED\n");
    else
        printf(1, "\n\nJOIN ORDER TEST FAILED\n");
    
}

void say_hello(void* arg){
    sleep(10);
    printf(1,"TEST FAILED\n");
    exit();
}

void childkilltest(){
    printf(1, "CHILD KILL TEST\n\n");
    THREAD t = create_thread(say_hello, 0, CLONE_VM);
    threadkill(t.pid);
    join_threads(t);


}


int main(int argc, char *argv[])
{
    
    Matrix();
    Get_ids();
    Clone_Vm();
    Counter();
    joinordertest();
    childkilltest();
    
    

    // THREAD threads[100];
    // for(int i = 0; i < 100; i++){
    //     threads[i] = create_thread(test1, (void*)i, CLONE_VM);
    //     join_threads(threads[i]);
    //     printf(1,"PID: %d\n", threads[i].pid);

    // }
    
    exit();
}

