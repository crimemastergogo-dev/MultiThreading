#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h> /*POSIX thread Library*/
#include <sys/syscall.h>
#define SIZE 8


void *routine(void * arg)
{
    sleep(1);
    printf("worker Thread ID TID  = %d\n",(pid_t) syscall(SYS_gettid));
}

int main (int argc, char *argv[])
{
    pthread_t tid[SIZE];

    printf("Main Thread ID TID  = %d\n",(pid_t) syscall(SYS_gettid));
    for (int i = 0 ;i < SIZE; i++ )
    {
        if (0 != pthread_create(&tid[i],NULL,&routine,NULL))
        {
            printf("Thread creation failed\n");
        }

        /*
          int pthread_detach(pthread_t thread); 

          The pthread_detach() function marks the thread identified by
          thread as detached.  When a detached thread terminates, its
          resources are automatically released back to the system without
          the need for another thread to join with the terminated thread.

          On success, pthread_detach() returns 0; on error, it returns an
          error number.
         */
        pthread_detach(tid[i]);
    }


    /*
    for (int i = 0 ;i < SIZE; i++ )
    {
        pthread_join(tid[i],NULL);
    }*/


    pthread_exit(0);
    return 0;
}



