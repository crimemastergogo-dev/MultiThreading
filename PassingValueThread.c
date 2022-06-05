/*
 * create 10 threads, each taking a unique prime from the primes array
 * and print it on the screen
   {2,3,5,7,11,13,17,19,23,29};
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> /*POSIX thread Library*/
#define SIZE 10

int primes[10] = {2,3,5,7,11,13,17,19,23,29};

#if 0 
void *routine(void* arg)
{
    sleep(1);
    int index = *(int *)arg;
    printf("Prime[%d] = %d \n",index,primes[index]);
}
#else

void *routine(void* arg)
{
    int index = *(int *)arg;
    printf("Prime[%d] = %d \n",index,primes[index]);

    free(arg);
}

#endif

int main (int argc, char *argv[])
{
    pthread_t tid[SIZE];

#if 0
    for (int i = 0 ;i < SIZE; i++ )
    {
        if (0 != pthread_create(tid +i,NULL,routine,&i))
        {
            printf("Thread creation failed\n");
        }

    }

    for (int i = 0 ;i < SIZE; i++ )
    {
        if (0 != pthread_join(tid[i],NULL))
        {
            printf("Thread joinng failed\n");
        }
    }
#else
    for (int i = 0 ;i < SIZE; i++ )
    {
        int *a = malloc(sizeof(int));
        *a = i;
        if (0 != pthread_create(tid +i,NULL,routine,a))
        {
            printf("Thread creation failed\n");
        }

    }

    for (int i = 0 ;i < SIZE; i++ )
    {
        if (0 != pthread_join(tid[i],NULL))
        {
            printf("Thread joinng failed\n");
        }
    }

#endif

    return 0;
}



