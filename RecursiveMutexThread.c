#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h> /*POSIX thread Library*/
#include <sys/syscall.h>
#define SIZE 8


int fuel = 50;
pthread_mutex_t FuelMutex ;

void *routine(void * arg)
{
    pthread_mutex_lock(&FuelMutex);
    pthread_mutex_lock(&FuelMutex);
    printf("worker Thread ID TID  = %d\n",(pid_t) syscall(SYS_gettid));
    fuel +=50;
    pthread_mutex_unlock(&FuelMutex);
    pthread_mutex_unlock(&FuelMutex);
}

int main (int argc, char *argv[])
{
    pthread_t tid[SIZE];
    pthread_mutexattr_t fuelMutexattr;

    pthread_mutexattr_init(&fuelMutexattr);

    pthread_mutexattr_settype(&fuelMutexattr,PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(&FuelMutex,&fuelMutexattr);
    printf("Main Thread ID TID  = %d\n",(pid_t) syscall(SYS_gettid));
    for (int i = 0 ;i < SIZE; i++ )
    {
        if (0 != pthread_create(&tid[i],NULL,&routine,NULL))
        {
            printf("Thread creation failed\n");
        }

    }


    printf("final Fuel  = %d\n",fuel);
    for (int i = 0 ;i < SIZE; i++ )
    {
        pthread_join(tid[i],NULL);
    }
    pthread_mutexattr_destroy(&fuelMutexattr);
    pthread_mutex_destroy(&FuelMutex);

    return 0;
}

