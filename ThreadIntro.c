#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> /*POSIX thread Library*/

void *WorkerThredFunc(void* tid)
{
    long *MyThreadID = (long *)tid;
    printf("My Worker Thread 1 id = %ld\n",*MyThreadID);
    while(1);
}
int main (int argc, char *argv[])
{
    /*Basicically pthread_t is a large interger 
     * which is used to uniquly identify the thread*/
    pthread_t tid1;

#if 0
    int pthread_create(pthread_t *restrict thread,
                          const pthread_attr_t *restrict attr,
                          void *(*start_routine)(void *),
                          void *restrict arg);

    start_routine ==> Function to be performed on thread creation
    arg ==> sole argument to be passed into the function.
    attr  ==> attributes of the thread
     if attr = NULL then default attributes it uses structure "pthread_attr_t"
    On success, returns 0;
    on error, it returns an error number
    and the contents of *thread are undefined.
#endif
    printf("Inside Main creating thread\n");
    if (0 != pthread_create(&tid1,NULL,WorkerThredFunc,(void *)&tid1))
    {
        printf("Thread creation failed\n");
    }
    printf("After Thread Creation\n");

    while (1);
#if 0
    void pthread_exit(void *retval);
    The pthread_exit() function terminates the calling thread and 
    returns a value via retval that (if the thread is joinable) is available to another
    thread in the same process that calls pthread_join(3).

    The use of this function is to wait the execution of WorkerThredFunc

    pthread_exit is called from the thread itself to terminate its execution (and return a result) early.

    int pthread_join(pthread_t thread, void **retval);
    pthread_join is called from another thread (usually the thread that created it) to wait for a thread
    to terminate and obtain its return value. It can be called before or after the thread you are waiting 
    for calls pthread_exit.
#endif
    pthread_exit(NULL);

    return 0;
}



