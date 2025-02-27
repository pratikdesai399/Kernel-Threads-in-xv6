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
    int ret1 = read(3, &buff, 100);
    if(ret1 == -1){
        printf(1,"TEST FAILED\n\n");
    }else{
        printf(1,"TEST PASSED\n\n");
    }
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

//THREAD IN THREAD
void thread1(void* arg){
    printf(1,"HELLO\n\n");
    exit();
}

void thread(void* arg){
    THREAD t1;
    t1 = create_thread(thread1, 0, CLONE_VM);
    join_threads(t1);
    exit();

}

void threadinthread(){
    THREAD t;
    printf(1,"THREAD IN THREAD TEST\n");
    t = create_thread(thread, 0, CLONE_VM);
    join_threads(t);
    
}


//JOIN WAIT TEST
void jwt(void *arg){
    int x = (int)arg;
    //printf(1,"%d\n", x);
    sleep(x);
    exit();
}
void joinwaittest(){
    
    THREAD t[2];
    int one = 5, two = 2;
    int tgid = fork();
    if(!tgid){
        //CHILD
        t[0] = create_thread(jwt, (void*)one, CLONE_VM);
        t[1] = create_thread(jwt, (void*)two, CLONE_VM);
        int ret1 = join_threads(t[0]);
        int ret2 = join_threads(t[1]);

        if(ret1 != t[0].pid || ret2 != t[1].pid){
            printf(1,"Wait reaped before join. TEST FAILED\n");
        }else{
            printf(1,"JOIN WAIT TEST PASSED\n\n");
        }
        exit();
        
    }
    int ret;
    while((ret = wait()) != -1);
    
}

//WAIT CHILD TEST
// void wct(void*arg){
    
//     int ret = wait();
//     printf(1,"%d\n", ret);
//     exit();
// }

// void waitchildtest(){
//     int ret = fork();
//     if(!ret){
//         //CHILD
//         sleep(30);
//         exit();
//     }
//     THREAD t = create_thread(wct, 0, CLONE_THREAD);
//     ret = join_threads(t);
//     ret = wait();
//     if(ret != -1){
//         printf(1,"TEST FAILED\n");
//     }else{
//         printf(1,"TEST PASSED\n");
//     }
    
// }


//RACE TEST
// Threads are racing to increment a single integer
void race(void*arg){
    int *x = (int*)arg;
    for(int i = 0; i < 1000;i++){
        *x += 1;
    }
    exit();
}

void racing(){
    THREAD t[50];
    int v = 0;
    for(int i = 0; i < 50; i++){
        t[i] = create_thread(race,(void*)&v, CLONE_VM );
        join_threads(t[i]);
    }
    if(v != 50*1000){
        printf(1,"RACING FAILED\n");
    }else{
        printf(1,"RACING TEST PASSED\n\n");
    }
}

// //CHILD EXEC TEST
// char *threadargs[] = {"ls", 0};
// void threadchild(void*arg){
//     int ret = exec("ls", threadargs);
//     if(ret == -1){
//         printf(1,"\nEXEC FAILED\n");
//     }
//     exit();
// }


// void childexectest(){
//     THREAD t[2];
//     int x = 10;
//     int ret = fork();
//     if(!ret){
//         //CHILD
//         t[0] = create_thread(threadchild, 0,CLONE_VM);
//         t[1] = create_thread(jointestthread, (void*)&x,CLONE_VM);
//         join_threads(t[1]);
//         printf(1,"EXEC TEST FAILED/n");
//         join_threads(t[0]);
//         exit();

//     }else{
//         int ret2 = wait();
//         if(ret == ret2){
//             printf(1,"EXEC TEST PASSED\n");
//         }else{
//             printf(1,"EXEC TEST FAILED ret : %d , ret2 : %d\n", ret,ret2);
//         }
//     }
// }

//CLONE FILES TESTING
void testf(void *arg){
    int fd = open("README", O_RDWR);
    printf(1,"%d\n", fd);
    exit();
}

void Clone_Files(){
    THREAD t;
    char buff[100];
    t = create_thread(testf, 0, CLONE_FILES);
    join_threads(t);
    int ret = read(3,&buff, 20);
    if(ret > 0){
        printf(1,"TEST CASE PASSED FOR CLONE_FILES\n");
        printf(1,"RET : %d\n\n", ret);
    }
}

//MATRIX MULTIPLICATION
int no_of_threads = 3;
int r1,c1,r2,c2;
int mat1[100][100];
int mat2[100][100];
int res[100][100];

void multiplication(void *arg){
    int n = (int)arg;
    //printf("%d\n",r1);
    int start = (n*r1)/no_of_threads;
    int end = ((n+1)*r1)/no_of_threads;

    for(int i = start ; i < end; i++){
        for(int j = 0; j < c2; j++){
            res[i][j] = 0;
            for(int k = 0 ; k < c1; k++){
                res[i][j] += mat1[i][k]*mat2[k][j];
            }
        }
    }
    //Printing matrix
    printf(1,"\n");
    //printf("Thread no %ld\n",n);
    if(n == 0){
        printf(1,"%d %d\n",r1,c2);
        for(int r=0; r < r1; r++){
        for(int c = 0; c < c2; c++){
            printf(1,"%d",res[r][c]);
        }
        printf(1,"\n");
    }
    }
    exit();
}

void matrixMulti(){
    //Declare number of threads
    THREAD tid[no_of_threads];
    r1 = 1; 
    c1 = 3;
    for(int row = 0; row<r1; row++){
        for(int col = 0; col < c1; col++){
            mat1[row][col] = 2;
        }
    }

    r2 = 3; c2 = 1;
    
    for(int row = 0; row<r2; row++){
        for(int col = 0; col < c2; col++){
            mat2[row][col] = 3;
        }
    }

    //Creating threads
    for(int i = 0; i < no_of_threads; i++){
        tid[i] = create_thread(multiplication, (void*)i, CLONE_VM);
        
    }
    for(int i = 0; i < no_of_threads; i++){
        join_threads(tid[i]);
    }
    if(res[0][0] == 18){
        printf(1,"TEST CASE PASSED\n\n");
    }

}

//FLAGS TESTING
int VM_FLAG_TEST = 0;

void testFlags(void* arg){
    char buf[10];
    int fd = (int)arg;
    //int fd = open("README", O_RDWR);
    int ret = read(fd, &buf, 10);
    if(fd){
        printf(1,"BEFORE CALLING THREAD\n");
        if(ret > 0){
            printf(1,"READ SUCCESSFUL\n");
            printf(1,"%d READ BYTES\n",ret);
        }else{
            printf(1,"READ UNSUCCESSFULL\n");
        }
        int pid = getpid();
        int tgid = gettgid();
        int ppid = getppid();
        VM_FLAG_TEST++;
        printf(1,"GLOBAL INT VM_FLAG_TEST : %d\nFD : %d\nPID : %d\nTGID : %d\nPPID : %d\n\n",VM_FLAG_TEST,fd,pid,tgid,ppid);
        if(VM_FLAG_TEST){
            printf(1,"CLONE VM TEST PASSED\n\n");
        }else{
            printf(1,"CLONE VM TEST FAILED\n\n");
        }

    }else{
        printf(1,"FILE OPEN FAILED\n");
    }
    exit();

}

void testallflags(){
    THREAD t1;
    char buf[10];
    int fd = open("README", O_RDWR);
    int ret = read(fd, &buf, 10);
    if(fd){
        printf(1,"BEFORE CALLING THREAD\n");
        if(ret > 0){
            printf(1,"READ SUCCESSFUL\n");
            printf(1,"%d READ BYTES\n",ret);
        }else{
            printf(1,"READ UNSUCCESSFULL\n");
        }
        int pid = getpid();
        int tgid = gettgid();
        int ppid = getppid();
        printf(1,"GLOBAL INT VM_FLAG_TEST : %d\nFD : %d\nPID : %d\nTGID : %d\nPPID : %d\n\n",VM_FLAG_TEST,fd,pid,tgid,ppid);

        t1 = create_thread(testFlags, (void*)fd, CLONE_VM);
        join_threads(t1);

    }else{
        printf(1,"FILE OPEN FAILED\n");
    }
}

//STRESS TEST
#define MAX 100
void stressTest1(){
    THREAD thread[100];
    
    int f = 0, i;
    int x = 100;
    for(i = 0; i < MAX; i++){
        thread[i] = create_thread(jointestthread,(void*)x, 0 );
        if(thread[i].pid == -1){
            break;
        }
    }

    if(i < 61){
        //NPROC value is  64
        printf(1,"STRESS TEST FAILED\n\n");
        f = 1;
    }
    for(int j = 0; j< i; j++){
        join_threads(thread[j]);
    }

    if(!f){
        printf(1,"STRESS TEST PASSED\n\n");
    }

    
}

//FORK IN THREAD
void forkthreadchild(void* arg){
    int ret = fork();
    if(!ret){
        //CHILD
        printf(1,"CHILD IN THREAD\n");
        exit();
    }
    int ret1 = wait();
    printf(1,"ret1 : %d\n", ret1);
    printf(1,"PARENT IN THREAD\nFORK IN THREAD TEST PASSED\n\n");
    
    exit();
}

void forkinThread(){
    THREAD t;
    t = create_thread(forkthreadchild, 0, CLONE_VM);
    join_threads(t);
}


//TEST PROGRAM
void testThread(void* arg){
    char* m = (char*)arg;
    //printf(1,"%d\n", m[4]);
    for(int i = 0; i < 100;i++){
        if(m[i] != 0){
            printf(1,"%d\n", m[i]);
            exit();
        }
    }
    printf(1,"BUFFER TEST PASSED\n\n");
    exit();
}
void SimpleTest(){
    char *buff;
    buff = malloc(100);
    memset(buff, 0, 100);
    //printf(1,"%d\n",buff[4]);

    THREAD t = create_thread(testThread, (void*)buff, CLONE_VM);
    join_threads(t);
}

//FORK IN THREAD TEST 2
int var = 0;
void fork_fun(void *args) {
  int ret = fork();
  if(ret == 0) {
    var+= 1;
  } 
  else{
    int ret1 = wait();
    printf(1,"%d\n", ret1);
    var+= 2;
  }
  exit();
}

void fork_in_thread() {
  THREAD t;
  int v = 9;
  // printf()
  t = create_thread(fork_fun, (void*)&v, CLONE_VM || CLONE_THREAD);
  join_threads(t);
  if(var == 2) {
    printf(1, "FORK IN THREAD TEST PASSED\n\n");
  } else {
    printf(1, "FORK IN THREAD TEST FAILED\n\n");
  }
}


int main(int argc, char *argv[])
{
    stressTest1();
    threadinthread();
    racing();
    Clone_Files();
    Matrix();
    Get_ids();
    Clone_Vm();
    Counter();
    joinordertest();
    childkilltest();
    joinwaittest();
    matrixMulti();
    testallflags();
    forkinThread();
    SimpleTest();
    fork_in_thread();
    
    
    exit();
}

