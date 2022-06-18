#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h> /*POSIX thread Library*/
#include <sys/syscall.h>
#include <semaphore.h>
#define SIZE 8

sem_t sem;

void *routine(void * arg)
{

    sem_wait(&sem);
    sleep(1);
    printf("worker Thread num = %d with TID = %d\n",*(int *)arg,(pid_t) syscall(SYS_gettid));
    sem_post(&sem);


    free(arg);
}

int main (int argc, char *argv[])
{
    pthread_t tid[SIZE];

    /*
       int sem_init(sem_t *sem, int pshared, unsigned int value);
     
     
       sem_init() initializes the unnamed semaphore at the address
       pointed to by sem.  The value argument specifies the initial
       value for the semaphore.

       The pshared argument indicates whether this semaphore is to be
       shared between the threads of a process, or between processes.

       If pshared has the value 0, then the semaphore is shared between
       the threads of a process, and should be located at some address
       that is visible to all threads (e.g., a global variable, or a
       variable allocated dynamically on the heap).

       If pshared is nonzero, then the semaphore is shared between
       processes, and should be located in a region of shared memory
       (see shm_open(3), mmap(2), and shmget(2)).  (Since a child
       created by fork(2) inherits its parent's memory mappings, it can
       also access the semaphore.)  Any process that can access the
       shared memory region can operate on the semaphore using
       sem_post(3), sem_wait(3), and so on.

        sem_init() returns 0 on success; on error, -1 is returned, and
        errno is set to indicate the error.
     */
    sem_init(&sem,0,7);

    printf("Main Thread ID TID  = %d\n",(pid_t) syscall(SYS_gettid));
    for (int i = 0 ;i < SIZE; i++ )
    {
        int *a = (int *)malloc(sizeof(int));
        *a = i;
        if (0 != pthread_create(&tid[i],NULL,&routine,a))
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
