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
    pthread_attr_t thread_attributes;
    pthread_attr_init(&thread_attributes);
    pthread_attr_setdetachstate(&thread_attributes,PTHREAD_CREATE_DETACHED);

    printf("Main Thread ID TID  = %d\n",(pid_t) syscall(SYS_gettid));
    for (int i = 0 ;i < SIZE; i++ )
    {
        if (0 != pthread_create(&tid[i],NULL,&routine,NULL))
        {
            printf("Thread creation failed\n");
        }
        pthread_detach(tid[i]);
    }
   // pthread_attr_destroy(&thread_attributes);


    pthread_exit(0);
    return 0;
}



