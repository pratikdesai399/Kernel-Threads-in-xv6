#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


int sys_clone(void){
  char* f; 
  char* arg; 
  char* stack;
  int flags;
  if( argptr(0,&f, sizeof(char*)) < 0 || argptr(1, &arg,sizeof(char*)) < 0 || argptr(2, &stack,sizeof(char*)) < 0 || argint(3, &flags) < 0){
    return -1;
  }
  return clone((void*)f, (void*)arg, (void*)stack, flags);

}

int sys_join(void){
  int pid;
  if(argint(0, &pid) < 0){
    return -1;
  }

  return join(pid);
}

int sys_gettgid(void){
  return myproc()->tgid;
}

int sys_getppid(void){
  return myproc()->parent->pid;
}

int sys_gettid(void){
  return myproc()->pid;
}

int sys_threadkill(void){
  int pid;
  if(argint(0,&pid) < 0){
    return -1;
  }
  return threadkill((int)pid);
}
