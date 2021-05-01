#include "types.h"
#include "stat.h"
#include "user.h"
#include "threadlib.h"
#include "fcntl.h"

void fn(void* arg){
  printf(1,"HELLO\n");
  exit();
}

void test1(void* arg){
  int fd = open("README", O_RDWR);
  printf(1,"\n%d\n", fd);
  exit();
}

int main(int argc, char *argv[])
{
    
    THREAD t[478];
    //char buff[100];
    
    for(int i = 0; i < 478; i++){
      t[i] = create_thread(fn,0, CLONE_VM);
      printf(1,"%d\n", t[i].pid);
      join_threads(t[i]);
    }
    // THREAD t;
    // t = create_thread(test1, 0, CLONE_VM || CLONE_FILES);
    // join_threads(t);
    // int ret = read(3,&buff, 10);
    // printf(1,"%d\n",ret);
    // write(1, &buff, 10);
    // exit();
}

