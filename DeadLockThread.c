#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h> /*POSIX thread Library*/
#include <sys/syscall.h>
#define SIZE 8


int fuel = 50;
pthread_mutex_t FuelMutex = PTHREAD_MUTEX_INITIALIZER;

int water = 50;
pthread_mutex_t Fuelwater = PTHREAD_MUTEX_INITIALIZER;

void *routine(void * arg)
{
    printf("worker Thread ID TID  = %d\n",(pid_t) syscall(SYS_gettid));
    if ( rand() %2 == 0 )
    {
        pthread_mutex_lock(&FuelMutex);
        sleep(1);
        pthread_mutex_lock(&Fuelwater);
    }
    else
    {
        pthread_mutex_lock(&Fuelwater);
        sleep(1);
        pthread_mutex_lock(&FuelMutex);
    }
//    pthread_mutex_lock(&FuelMutex);    ---------> In case we forgot to unlock the Mutex we can get a deadlock
    fuel +=50;
    water = fuel;
    printf("Fuel incremented to %d\n",fuel);
    printf("water incremented to %d\n",water);
    pthread_mutex_unlock(&FuelMutex);
    pthread_mutex_unlock(&Fuelwater);
}

int main (int argc, char *argv[])
{
    srand(time(NULL));
    pthread_t tid[SIZE];

    printf("Main Thread ID TID  = %d\n",(pid_t) syscall(SYS_gettid));
    for (int i = 0 ;i < SIZE; i++ )
    {
        if (0 != pthread_create(&tid[i],NULL,&routine,NULL))
        {
            printf("Thread creation failed\n");
        }

    }


    printf("final Fuel  = %d\n",fuel);
    printf("final water = %d\n",water);
    for (int i = 0 ;i < SIZE; i++ )
    {
        pthread_join(tid[i],NULL);
    }
    pthread_mutex_destroy(&FuelMutex);
    pthread_mutex_destroy(&Fuelwater);

    return 0;
}



/*
            | fuel Mutex    | water Mutex
   -------------------------------------------        
   Thread 1 |    X          |  wait
   --------------------------------------------
   Thread 2 |    wait       |   X


 */
