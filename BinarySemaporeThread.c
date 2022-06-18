#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> /*POSIX thread Library*/
#include <semaphore.h>
#define SIZE 4

int mails = 0;

pthread_mutex_t mutex;
sem_t sem_fuel;

void *Mailbox()
{
    for(int i =0 ;i < 1000000;i++)
    {
        //pthread_mutex_lock(&mutex)
        sem_wait(&sem_fuel);
        mails++;
        sem_post(&sem_fuel);
        //pthread_mutex_unlock(&mutex)
    }
}

int main (int argc, char *argv[])
{
    pthread_t tid[SIZE];
    sem_init(&sem_fuel,0,1);

    if (0 != pthread_mutex_init(&mutex,NULL))
    {
        printf("Mutex Initialization failed ...!!!\n");
    }

    for (int i = 0 ;i < SIZE; i++ )
    {
        if (0 != pthread_create(&tid[i],NULL,Mailbox,NULL))
        {
            printf("Thread creation failed\n");
        }
    }

    for (int i = 0 ;i < SIZE; i++ )
    {
        pthread_join(tid[i],NULL);
    }


    if (0 != pthread_mutex_destroy(&mutex))
    {
        printf("Mutex Initialization failed ...!!!\n");
    }
    printf("No of mails = %d\n",mails);

    sem_destroy(&sem_fuel);
    return 0;
}



