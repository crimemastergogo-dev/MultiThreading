#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> /*POSIX thread Library*/
#define SIZE 4


pthread_mutex_t mutex;

void *routine()
{
    /*
     * int pthread_mutex_trylock(pthread_mutex_t *mutex);
     * The pthread_mutex_trylock() function shall be equivalent to pthread_mutex_lock(), 
     * except that if the mutex object referenced by mutex is currently locked
     * (by any thread, including the current thread), the call shall return immediately. 
     * If the mutex type is PTHREAD_MUTEX_RECURSIVE and the mutex is currently owned by
     * the calling thread, the mutex lock count shall be incremented by one and the 
     * pthread_mutex_trylock() function shall immediately return success.
     *
     * The pthread_mutex_trylock() function shall return zero if a lock on the mutex object 
     * referenced by mutex is acquired. Otherwise, an error number is returned to indicate the error.
     */
        if (0 != pthread_mutex_trylock(&mutex))
        {
            printf("pthread_mutex_lock failed ...!!!\n");
        }
        printf("Locked...!!!\n");
        sleep(1);
        if (0 != pthread_mutex_unlock(&mutex))
        {
            printf("pthread_mutex_unlock failed ...!!!\n");
        }
}

int main (int argc, char *argv[])
{
    pthread_t tid[SIZE];
    if (0 != pthread_mutex_init(&mutex,NULL))
    {
        printf("Mutex Initialization failed ...!!!\n");
    }

    for (int i = 0 ;i < SIZE; i++ )
    {
        if (0 != pthread_create(&tid[i],NULL,routine,NULL))
        {
            printf("Thread creation failed\n");
        }
    }

    for (int i = 0 ;i < SIZE; i++ )
        pthread_join(tid[i],NULL);

    if (0 != pthread_mutex_destroy(&mutex))
    {
        printf("Mutex Initialization failed ...!!!\n");
    }

    return 0;
}



