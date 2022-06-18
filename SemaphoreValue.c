#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> /*POSIX thread Library*/
#include <semaphore.h>
#define SIZE 4


sem_t sem;
void *routine(void *args)
{
    int index = *(int *)args;
    int semVal = 0;
    sem_wait(&sem);
    sem_getvalue(&sem,&semVal);
    printf("sem_wait Index = %d semVal = %d\n",index,semVal);
    sem_post(&sem);
    sem_getvalue(&sem,&semVal);
    printf("sem_post Index = %d semVal = %d\n",index,semVal);

    free(args);
}

int main (int argc, char *argv[])
{

    pthread_t tid[SIZE];
    sem_init(&sem,0,4);
    for (int i = 0 ;i < SIZE; i++ )
    {
        int *index = (int *)malloc(sizeof(int));
        *index = i;
        if (0 != pthread_create(&tid[i],NULL,routine,index))
        {
            printf("Thread creation failed\n");
        }
    }

    for (int i = 0 ;i < SIZE; i++ )
    {
        pthread_join(tid[i],NULL);
    }

    sem_destroy(&sem);
    return 0;
}



