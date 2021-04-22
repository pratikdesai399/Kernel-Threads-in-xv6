#include "types.h"
#include "stat.h"
#include "user.h"
#include "threadlib.h"


void test1(void* arg){
    char *str = (char*)arg;
    printf(1,"IN THREAD\n");
    printf(1,"STRING : %s\n", str);
      
    exit();

}

    


int main(int argc, char *argv[])
{
    printf(1,"In main\n");
    THREAD th = create_thread(test1, "PRATIK");
    printf(1,"Thread Created\n");
    //join_threads(th);
    join_threads(th);
    printf(1,"Threads Joined\n");
    printf(1, "RETURN VALUE FOR CLONE : %d\n", th.pid);
    exit();
}