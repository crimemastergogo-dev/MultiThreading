/*
 *1.manage share memory access
 *2.checking for buffer is full
 *3. checking for buffer empty
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h> /*POSIX thread Library*/
#include <sys/syscall.h>
#include <semaphore.h>
#define PRODUCER_SIZE 2
#define CONSUMER_SIZE 4
#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE] = {0};
int count = 0;

sem_t sem_empty;
sem_t sem_full;

pthread_mutex_t buffermutex = PTHREAD_MUTEX_INITIALIZER;

void *producer(void * arg)
{
    while (1)
    {
        int x = rand() % 100;

        sem_wait(&sem_empty);
        pthread_mutex_lock(&buffermutex);
        buffer[count] = x;
        count++;
        pthread_mutex_unlock(&buffermutex);
        sem_post(&sem_full);

    }


    free(arg);
}
void *consumer(void * arg)
{
    while (1)
    {
        int y = -1;

        sem_wait(&sem_full);
        pthread_mutex_lock(&buffermutex);

        y = buffer[count -1] ;

        count--;

        printf("Got number y = %d\n",y);
        pthread_mutex_unlock(&buffermutex);
        sem_post(&sem_empty);
        sleep(1);
    }

    free(arg);
}
int main (int argc, char *argv[])
{
    srand(time(NULL));
    pthread_t tid_producer[PRODUCER_SIZE];
    pthread_t tid_consumer[CONSUMER_SIZE];
    sem_init(&sem_empty,0,10);
    sem_init(&sem_full,0,0);
    int i = 0;

    for (i = 0 ;i < PRODUCER_SIZE; i++ )
    {
        int *producerID = (int *)malloc(sizeof(int));
        *producerID = i;
        if (0 != pthread_create(&tid_producer[i],NULL,&producer,producerID))
        {
            printf("Thread creation failed\n");
        }
    }

    for (i = 0 ;i < CONSUMER_SIZE ; i++ )
    {
        int *consumerID = (int *)malloc(sizeof(int));
        *consumerID = i;
        if (0 != pthread_create(&tid_consumer[i],NULL,&consumer,consumerID))
        {
            printf("Thread creation failed\n");
        }
    }

    for (i = 0 ;i < PRODUCER_SIZE; i++ )
    {
        pthread_join(tid_producer[i],NULL);
    }

    for (i = 0 ;i < CONSUMER_SIZE; i++ )
    {
        pthread_join(tid_consumer[i],NULL);
    }
    pthread_mutex_destroy(&buffermutex);
    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);

    return 0;
}
