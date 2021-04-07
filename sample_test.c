#include "types.h"
#include "stat.h"
#include "user.h"

void test1(void* arg){
    printf(1,"IN THREAD\n");
      
    exit();

}

    


int main(int argc, char *argv[])
{
    printf(1,"In main\n");
    void * stack;
    stack = malloc(4096);

    int ret = clone(test1, 0, stack);
    printf(1, "RETURN VALUE FOR CLONE : %d\n", ret);
    exit();
}