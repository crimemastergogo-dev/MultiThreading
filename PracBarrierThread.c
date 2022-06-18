/*
 * Every thread rolls a dice, saved its value in an array.
 * The main thread calculates the winner then each thread
 * prints a message with whether or not they lost or won
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h> /*POSIX thread Library*/
#define SIZE 8

int diceValue[SIZE];
int status[SIZE] = {0};


pthread_barrier_t barrier;
pthread_barrier_t StatusCheckBarrier;


void *rollDice(void * arg)
{
    int index = *(int *)arg;
    while (1)
    {
        diceValue[index] = rand()%6 +1;
        printf("Player %d rolled value =%d\n",index,diceValue[index]);
        pthread_barrier_wait(&barrier);
        pthread_barrier_wait(&StatusCheckBarrier);
        if (status[index] == 1)
            printf("player %d won ...!!!\n",index);
    }

    free(arg);
}

int main (int argc, char *argv[])
{
    srand(time(NULL));
    pthread_t tid[SIZE];

    pthread_barrier_init(&barrier,NULL,SIZE +1);
    pthread_barrier_init(&StatusCheckBarrier,NULL,SIZE +1);

    for (int i = 0 ;i < SIZE; i++ )
    {
        int *idx = (int *)malloc(sizeof(int));
        *idx = i;
        if (0 != pthread_create(&tid[i],NULL,&rollDice,idx))
        {
            printf("Thread creation failed\n");
        }
    }

    int n = 0;
    while (1)
    {
        printf("=========Round [%d] started=============\n",n++);
        pthread_barrier_wait(&barrier);
        int MaxRolledValue = diceValue[0]; 
        for(int i =0; i < SIZE; i++)
        {
            if (diceValue[i] > MaxRolledValue)
            {
                MaxRolledValue = diceValue[i];
            }
        }
        for(int i = 0; i < SIZE; i++)
        {
            if(MaxRolledValue == diceValue[i])
                status[i] = 1;
            else
                status[i] = 0;
        }


        pthread_barrier_wait(&StatusCheckBarrier);
        sleep(1);
    }

    for (int i = 0 ;i < SIZE; i++ )
    {
        pthread_join(tid[i],NULL);
    }
    pthread_barrier_destroy(&barrier);
    pthread_barrier_destroy(&StatusCheckBarrier);


    return 0;
}



