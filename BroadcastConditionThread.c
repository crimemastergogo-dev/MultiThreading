#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> /*POSIX thread Library*/
#define SIZE 5


int fuel = 0;
pthread_mutex_t mutex;
pthread_cond_t condition;

void *Fuel_filling(void * arg)
{
    /*
     int pthread_cond_broadcast(pthread_cond_t *cond);

     The pthread_cond_broadcast() function shall unblock all threads currently blocked on
     the specified condition variable cond.

     If successful, the pthread_cond_broadcast() function shall return zero;
     otherwise, an error number shall be returned to indicate the error.
     
     */
    printf("Filling Fuel ...!!!\n");
    for (int i = 0;i  < 5;i++)
    {
        pthread_mutex_lock(&mutex);
        fuel += 60;
        printf("Filling Fuel %d\n",fuel);
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&condition);
        sleep(1);
    }
}

void *car(void *arg)
{
    int *carId = arg;
    printf("car = %d Here to get Fuel...!!!\n",*carId);
    pthread_mutex_lock(&mutex);
    while (fuel < 40)
    {
        printf("No Fuel for CarId = %d...  waiting!!! \n",*carId);
        sleep(1);

        pthread_cond_wait(&condition,&mutex);
    }

    fuel -= 40;
    printf("For car ID = %d value after consumption =  %d\n",*carId,fuel);
    pthread_mutex_unlock(&mutex);

    free(carId);
    
}

int main (int argc, char *argv[])
{
    pthread_t tid[SIZE];

    if ( 0 != pthread_cond_init(&condition,NULL))
    {
        printf("condiftion init failed ...!!!\n");
    }
    if (0 != pthread_mutex_init(&mutex,NULL))
    {
        printf("Mutex Initialization failed ...!!!\n");
    }

    for (int i = 0 ;i < SIZE; i++ )
    {
        if (i != 4)
        {
            int *a = malloc(sizeof(int));
            *a = i;
            if (0 != pthread_create(&tid[i],NULL,&car,a))
            {
                printf("Thread creation failed\n");
            }
        }
        else
        {
            if (0 != pthread_create(&tid[i],NULL,&Fuel_filling,NULL))
            {
                printf("Thread creation failed\n");
            }

        }

    }

    for (int i = 0 ;i < SIZE; i++ )
    {
        pthread_join(tid[i],NULL);
    }

    if ( 0 != pthread_cond_destroy(&condition))
    {
        printf("condiftion init failed ...!!!\n");
    }

    if (0 != pthread_mutex_destroy(&mutex))
    {
        printf("Mutex Initialization failed ...!!!\n");
    }

    return 0;
}



