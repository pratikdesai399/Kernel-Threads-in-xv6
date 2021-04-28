#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"

volatile int globalCounter = 0;

void addToCounter(){
  globalCounter++;
  printf(1, "COUNTER IS: %x\n", globalCounter);
  exit();
}

int
main(int argc, char *argv[])
{
 int pid;
 void * stack[10];

 printf(1, "RUNNING TESTS\n");
 globalCounter++;
 printf(1, "COUNTER BEFORE CLONE: %x\n", globalCounter);

 int x;
 for(x=0; x<10; x++){
   stack[x] = malloc(4096);
   pid = clone((void *) &addToCounter, (void *) &globalCounter, (void *) stack[x], CLONE_VM);
   join(pid);
   printf(1, "PID: %d\n", pid);
 }



 globalCounter++;
 printf(1, "JOINED\n");

 printf(1, "COUNTER SHOULD BE 12 AND IT IS = %d\n", globalCounter);
 exit();
 return 0;
}