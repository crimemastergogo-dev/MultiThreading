#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> /*POSIX thread Library*/
#define SIZE 2


int fuel = 0;
pthread_mutex_t mutex;

/*
 * struct pthread_queue 	queue

   int 	 flags

   int 	 waiters

   pthread_mutex_t * 	mutex

   Condiftion variable has following 3 main tasks -
   1.condition wait
   2.condition broadcast
   3.condition signal
 */

pthread_cond_t condition;

void *Fuel_filling(void * arg)
{
    printf("Filling Fuel ...!!!\n");
    for (int i = 0;i  < 5;i++)
    {
        pthread_mutex_lock(&mutex);
        fuel += 15;
        printf("Filling Fuel %d\n",fuel);
        pthread_mutex_unlock(&mutex);
        /*
         int pthread_cond_signal(pthread_cond_t *cond);

         The pthread_cond_signal() function shall unblock at least one of the threads that are 
         blocked on the specified condition variable cond (if any threads are blocked on cond).

         If successful, pthread_cond_signal() functions shall return zero;
         otherwise, an error number shall be returned to indicate the error.
         */
        pthread_cond_signal(&condition);
        sleep(1);
    }
}

void *car(void *arg)
{
    printf("car = %d Here to get Fuel...!!!\n",*(int *)arg);
    pthread_mutex_lock(&mutex);
    while (fuel < 40)
    {
        printf("No Fuel for CarId = %d...  waiting!!! \n",*(int *)arg);
        sleep(1);

        /*
         int pthread_cond_wait(pthread_cond_t *restrict cond,pthread_mutex_t *restrict mutex);
         The pthread_cond_wait() functions shall block on a condition variable.
         They shall be called with mutex locked by the calling thread or undefined behavior results.

         Upon successful completion, a value of zero shall be returned; otherwise, 
         an error number shall be returned to indicate the error.
         */
        pthread_cond_wait(&condition,&mutex);
        //pthread_mutex_unlock
        //wait for the signal
        //pthread_mutex_lock
    }

    fuel -= 40;
    printf("For car ID = %d value after consumption =  %d\n",*(int *)arg,fuel);
    pthread_mutex_unlock(&mutex);

    if (arg != NULL)
    {
        free(arg);
        arg = NULL;
    }
}

int main (int argc, char *argv[])
{
    pthread_t tid[SIZE];

    /*
     int pthread_cond_init(pthread_cond_t *restrict cond,const pthread_condattr_t *restrict attr);

     The pthread_cond_init() function shall initialize the condition variable referenced by cond 
     with attributes referenced by attr. 
     If attr is NULL, the default condition variable attributes shall be used; 
     the effect is the same as passing the address of a default condition variable attributes object. 
     Upon successful initialization, the state of the condition variable shall become initialized.

     If successful, pthread_cond_init() functions shall return zero; 
     otherwise, an error number shall be returned to indicate the error.
    */

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
        if (i == 0)
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

    /*
     The pthread_cond_destroy() function shall destroy the given condition variable specified by cond;
     the object becomes, in effect, uninitialized.
     An implementation may cause pthread_cond_destroy() to set the object referenced by cond to an
     invalid value. A destroyed condition variable object can be reinitialized using 
     pthread_cond_init(); the results of otherwise referencing the object after it has been 
     destroyed are undefined.
     
     
     If successful, the pthread_cond_destroy() function shall return zero; 
     otherwise, an error number shall be returned to indicate the error.
     */
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



