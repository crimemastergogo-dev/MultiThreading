/*
 * create 2 threads, and give sum of first 10 prime number 
   {2,3,5,7,11,13,17,19,23,29};
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> /*POSIX thread Library*/
#define SIZE 2 

int primes[10] = {2,3,5,7,11,13,17,19,23,29};

void *routine(void* arg)
{
    int index = *(int *)arg;
    int sum = 0;

    for (int i = 0 ;i < 5;i++)
    {
        sum += primes[i +index ];
    }

    *(int *)arg = sum;
    return arg;
}

int main (int argc, char *argv[])
{
    srand(time(NULL));
    pthread_t tid[SIZE];

    for (int i = 0 ;i < SIZE; i++ )
    {
        int *a = malloc(sizeof(int));
        *a = i * 5;
        if (0 != pthread_create(tid +i,NULL,routine,a))
        {
            printf("Thread creation failed\n");
        }

    }

    int primeSum = 0;
    for (int i = 0 ;i < SIZE; i++ )
    {
        int *res = NULL;
        if (0 != pthread_join(tid[i],(void **)&res))
        {
            printf("Thread joinng failed\n");
        }
        primeSum += *res;
        if (res != NULL)
        {
            free(res);
            res = NULL;
        }
    }
    printf("Prime sum = %d\n",primeSum);

    return 0;
}



