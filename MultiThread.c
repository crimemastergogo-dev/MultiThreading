#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> /*POSIX thread Library*/
#define SIZE 3

int x = 0;

void *WorkerThreadFunc(void* tid)
{
    printf("x = %d\n",x);
    x++;
    long *MyThreadID = (long *)tid;
    printf("My Worker Thread  id = %ld\n",*MyThreadID);
    printf("Process id = %d\n",getpid());
    sleep (3);
    printf("x = %d\n",x);
    printf("Ending Thread\n");
    //while(1);
}

int main (int argc, char *argv[])
{
    pthread_t tid[SIZE];

#if 1
    for (int i = 0 ;i < SIZE; i++ )
    {
        if (0 != pthread_create(tid +i,NULL,WorkerThreadFunc,(void *)&tid[i]))
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
        if (0 != pthread_create(tid +i,NULL,WorkerThreadFunc,(void *)&tid[i]))
        {
            printf("Thread creation failed\n");
        }

        if (0 != pthread_join(tid[i],NULL))
        {
            printf("Thread joinng failed\n");
        }
    }

#endif

    //while (1);

    return 0;
}



