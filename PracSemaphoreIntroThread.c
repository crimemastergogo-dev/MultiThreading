/*
 *logging queue
 *
 *12 people login at the same time
 *more than 12 try to log in
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h> /*POSIX thread Library*/
#include <sys/syscall.h>
#include <semaphore.h>
#define SIZE 16

sem_t sem;

void *routine(void * arg)
{

    printf("Thraed num = %d with TID = %d is waiting in the logging queue...!!!!\n",*(int *)arg,(pid_t) syscall(SYS_gettid));
    sem_wait(&sem);
    sleep(1);
    printf("Thraed num = %d with TID = %d is waiting in the logged in ....!!!!\n",*(int *)arg,(pid_t) syscall(SYS_gettid));
    sleep(rand() %5 +1);
    printf("Thraed num = %d with TID = %d is waiting in the logged out ....!!!!\n",*(int *)arg,(pid_t) syscall(SYS_gettid));
    sem_post(&sem);


    free(arg);
}

int main (int argc, char *argv[])
{
    pthread_t tid[SIZE];
    srand(time(NULL));

    sem_init(&sem,0,20);

    printf("Main Thread ID TID  = %d\n",(pid_t) syscall(SYS_gettid));
    for (int i = 0 ;i < SIZE; i++ )
    {
        int *a = (int *)malloc(sizeof(int));
        *a = i;
        if (0 != pthread_create(&tid[i],NULL,&routine,a))
        {
            printf("Thread creation failed\n");
        }

    }

    for (int i = 0 ;i < SIZE; i++ )
    {
        pthread_join(tid[i],NULL);
    }

    sem_destroy(&sem);

    return 0;
}
