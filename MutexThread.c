#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> /*POSIX thread Library*/
#define SIZE 3

int mails = 0;

/*
typedef union
{
  struct __pthread_mutex_s
  {
    int __lock;
    unsigned int __count;
    int __owner;
    int __kind;
    unsigned int __nusers;
    __extension__ union
    {
      int __spins;
      __pthread_slist_t __list;
    };
  } __data;
  char __size[__SIZEOF_PTHREAD_MUTEX_T];
  long int __align;
} pthread_mutex_t;

*/
pthread_mutex_t mutex;

void *Mailbox()
{
/*
    int pthread_mutex_lock(pthread_mutex_t *mutex);
    
    The mutex object referenced by mutex shall be locked by calling pthread_mutex_lock().
    If the mutex is already locked, the calling thread shall block until the mutex becomes available. 
    This operation shall return with the mutex object referenced by mutex in the
    locked state with the calling thread as its owner.
   
     ###########################

    int pthread_mutex_unlock(pthread_mutex_t *mutex);
    The pthread_mutex_unlock() function shall release the mutex object referenced by mutex.
    The manner in which a mutex is released is dependent upon the mutex ka attribute type.
    If there are threads blocked on the mutex object referenced by mutex when pthread_mutex_unlock()
    is called, resulting in the mutex becoming available,
    the scheduling policy shall determine which thread shall acquire the mutex.

    on success ==> 0
    failure   ==>ERROR
*/
    for(int i =0 ;i < 1000000;i++)
    {
        if (0 != pthread_mutex_lock(&mutex))
        {
            printf("pthread_mutex_lock failed ...!!!\n");
        }
        mails++;
        if (0 != pthread_mutex_unlock(&mutex))
        {
            printf("pthread_mutex_unlock failed ...!!!\n");
        }
    }
}

int main (int argc, char *argv[])
{
    pthread_t tid[SIZE];
/*
    int pthread_mutex_init(pthread_mutex_t *restrict mutex,const pthread_mutexattr_t *restrict attr);
    mutex ==> address to the mutex
    attr ==> some attributes   NULL = Default; 
    Data Fields
    int 	pshared
    Whether to share the mutex with child processes.

    int 	kind
 	Type of the mutex.

    int 	protocol
 	Priority inheritance of the mutex.

    int 	robustness
 	What to do if a thread terminates while it holds a mutex.


    on success ==> 0
    failure   ==>ERROR
    */

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

    pthread_join(tid[0],NULL);
    pthread_join(tid[1],NULL);
    pthread_join(tid[2],NULL);


    /*
    int pthread_mutex_destroy(pthread_mutex_t *mutex);
    The pthread_mutex_destroy() function shall destroy the mutex object referenced by mutex;
    the mutex object becomes, in effect, uninitialized.
    An implementation may cause pthread_mutex_destroy() to set the object referenced by
    mutex to an invalid value. A destroyed mutex object can be reinitialized using pthread_mutex_init(); the results of otherwise referencing the object after it has been destroyed are undefined.

    on success ==> 0
    failure   ==>ERROR
    */
    if (0 != pthread_mutex_destroy(&mutex))
    {
        printf("Mutex Initialization failed ...!!!\n");
    }
    printf("No of mails = %d\n",mails);

    return 0;
}



