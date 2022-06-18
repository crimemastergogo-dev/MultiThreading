/*
 * consider there are multiple chefs using a stove and if the fuel in the stove finished then  chefs loose there job
  chefs = thread 
  stove = shared data (+mutex)

 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h> /*POSIX thread Library*/
#define SIZE 10


#if 0
pthread_mutex_t stovemutex;
int stoveFuel = 100;

void *routine(void *args)
{
    pthread_mutex_lock(&stovemutex);
    printf("stove is being used by chef = %d\n",*(int *)args);
    int fuelNeeded = (rand() % 30);
    if ((stoveFuel - fuelNeeded) < 0)
    {
        printf("No fule left...!!!\n");
    }
    else
    {
        usleep(500000);
        stoveFuel -= fuelNeeded;
        printf("Fuel left after consumtion by chef %d is %d\n",*(int *)args,stoveFuel);
    }
    pthread_mutex_unlock(&stovemutex);
}

int main (int argc, char *argv[])
{
    srand(time(NULL));
    pthread_t tid[SIZE];
    if (0 != pthread_mutex_init(&stovemutex,NULL))
    {
        printf("Mutex Initialization failed ...!!!\n");
    }

    for (int i = 0 ;i < SIZE; i++ )
    {
        int *chefID = (int *)malloc(sizeof(int));
        *chefID = i;
        if (0 != pthread_create(&tid[i],NULL,routine,chefID))
        {
            printf("Thread creation failed\n");
        }
    }

    for (int i = 0 ;i < SIZE; i++ )
        pthread_join(tid[i],NULL);

    if (0 != pthread_mutex_destroy(&stovemutex))
    {
        printf("Mutex Initialization failed ...!!!\n");
    }

    return 0;
}
#else
#define NUM_STOVE 4
#define MAX_FUEL 100
pthread_mutex_t stovemutex[NUM_STOVE];

int stoveFuel[NUM_STOVE] = {MAX_FUEL,MAX_FUEL,MAX_FUEL,MAX_FUEL};

void *routine(void *args)
{
    for (int i = 0 ;i < NUM_STOVE ; i++)
    {
        if (pthread_mutex_trylock(&stovemutex[i]) == 0)
        {
            printf("stove is being used by chef = %d\n",*(int *)args);
            int fuelNeeded = (rand() % 30);
            if ((stoveFuel[i] - fuelNeeded) < 0)
            {
                printf("No fule left...!!!\n");
            }
            else
            {
                usleep(500000);
                stoveFuel[i] -= fuelNeeded;
                printf("chef No = %d stovefuel[%d] = %d\n",*(int *)args,i,stoveFuel[i]);
            }
            pthread_mutex_unlock(&stovemutex[i]);
            break;
        }
        else
        {
            if (i == 3)
            {
                printf("Chef %d is waiting as no stove is available...!!!\n",*(int *)args);
                usleep(300000);
                i =  0;
            }
        }
    }
}

int main (int argc, char *argv[])
{
    srand(time(NULL));
    pthread_t tid[SIZE];

    for (int i = 0 ; i < NUM_STOVE ; i ++)
    {
        if (0 != pthread_mutex_init(&stovemutex[i],NULL))
        {
            printf("Mutex Initialization failed ...!!!\n");
        }
    }
    

    for (int i = 0 ;i < SIZE; i++ )
    {
        int *chefID = (int *)malloc(sizeof(int));
        *chefID = i;
        if (0 != pthread_create(&tid[i],NULL,routine,chefID))
        {
            printf("Thread creation failed\n");
        }
    }

    for (int i = 0 ;i < SIZE; i++ )
        pthread_join(tid[i],NULL);

    for (int i = 0 ; i < NUM_STOVE ; i ++)
    {
        if (0 != pthread_mutex_destroy(&stovemutex[i]))
        {
            printf("Mutex Initialization failed ...!!!\n");
        }
    }

    return 0;
}
#endif



