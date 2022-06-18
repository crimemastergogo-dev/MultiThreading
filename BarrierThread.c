#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> /*POSIX thread Library*/
#define SIZE 9

/*
  int pthread_barrier_init(pthread_barrier_t * restrict barrier, const pthread_barrierattr_t * restrict attr, unsigned int count);

  int pthread_barrier_destroy(pthread_barrier_t *barrier);

  int pthread_barrier_wait(pthread_barrier_t *barrier);

DESCRIPTION

     The pthread_barrier_init() function creates a new barrier with attributes attr and count.
     The count parameter indicates the number of threads which will participate in the barrier.
     The pthread_barrierattr_init(3) function may be used to specify the attributes supplied in
     attr.  If attr is NULL, the default attributes are used.  Barriers are most commonly used 
     in the decomposition of parallel loops.

     The pthread_barrier_destroy() function causes the resources allocated to barrier to be released.
     No threads should be blocked on barrier.

     The pthread_barrier_wait() function causes the current thread to wait on the barrier specified.
     Once as many threads as specified by the count parameter to the corresponding pthread_barrier_init() call
     have called pthread_barrier_wait(), all threads will wake up, return from
     their respective pthread_barrier_wait() calls and continue execution.

RETURN VALUES

     If successful, pthread_barrier_init() will return zero and put the new barrier id into barrier, otherwise an error number will be returned to
     indicate the error.

     If successful, pthread_barrier_destroy() will return zero.  Otherwise an error value will be returned.

     If successful, pthread_barrier_wait() will return zero for all waiting threads except for one. 
     One thread will receive status PTHREAD_BARRIER_SERIAL_THREAD, which is intended to indicate that
     this thread may be used to update shared data.  It is the responsibility of
     this thread to insure the visibility and atomicity of any updates to shared data with respect
     to the other threads participating in the barrier.  In the case of failure, an error value will be returned.
 
 */

pthread_barrier_t barrier;


void *routine(void * arg)
{
    printf("waiting for the barrier ..!!!\n");
    pthread_barrier_wait(&barrier);
    printf("Thread passed  ..!!!\n");
}

int main (int argc, char *argv[])
{
    pthread_t tid[SIZE];

    pthread_barrier_init(&barrier,NULL,3);

    for (int i = 0 ;i < SIZE; i++ )
    {
        if (0 != pthread_create(&tid[i],NULL,&routine,NULL))
        {
            printf("Thread creation failed\n");
        }
    }

    for (int i = 0 ;i < SIZE; i++ )
    {
        pthread_join(tid[i],NULL);
    }
    pthread_barrier_destroy(&barrier);


    return 0;
}



