#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> /*POSIX thread Library*/
#include <semaphore.h>
#include <time.h>
#include <string.h>
#define SIZE 4
#define QUEUE_SIZE 256

typedef struct task
{
    int a,b;
    char fname[20];
    void * (*taskFunction)(void *,void *);
}Task_t;

typedef struct Queuetask
{
    Task_t TaskQueue[QUEUE_SIZE];
    int task_count;
}TaskQueue_t;

TaskQueue_t GlobalTaskQueue;

pthread_mutex_t taskMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t taskCond;

void * sum(void *a,void *b)
{
    int *res = (int *)malloc(sizeof(int));

    *res = *(int *)a + *(int *)b;
    return res;
}

void *product(void *a,void * b)
{
    int *res = (int *)malloc(sizeof(int));
    *res = (*(int *)a) * (*(int *)b);
    return res;

}
void * divide(void * a,void * b)
{
    int *res = (int *)malloc(sizeof(int));

    *res = *(int *)a / *(int *)b;
    return res;

}
void * sub(void *a,void * b)
{
    int *res = (int *)malloc(sizeof(int));

    *res = *(int *)a - *(int *)b;
    return res;
}

void submitTask(Task_t Task)
{
    pthread_mutex_lock(&taskMutex);
    GlobalTaskQueue.TaskQueue[GlobalTaskQueue.task_count] = Task;
    GlobalTaskQueue.task_count++;
    pthread_mutex_unlock(&taskMutex);
    pthread_cond_signal(&taskCond);
}

void ExecuteTask(Task_t *p_task,int index)
{
    usleep(50000);
    int *arg1   = (int *)malloc(sizeof(int));
    int *arg2   = (int *)malloc(sizeof(int));
    *arg1 = p_task->a;
    *arg2 = p_task->b;

    int *result = p_task->taskFunction(arg1,arg2);

    printf("(%2d) Function [%7s] arg1 = %2d arg2 = %2d :: result = %d\n",
            index,p_task->fname,*arg1,*arg2,*result);


    free(arg1);
    free(arg2);
    free(result);

}

void *startThread(void *args)
{

    Task_t Task;
    while(1)
    {
        pthread_mutex_lock(&taskMutex);
        while(GlobalTaskQueue.task_count == 0)
        {
            pthread_cond_wait(&taskCond,&taskMutex);
        }

        Task = GlobalTaskQueue.TaskQueue[0];

        for(int i = 0;i < GlobalTaskQueue.task_count;i++)
        {
            GlobalTaskQueue.TaskQueue[i] = GlobalTaskQueue.TaskQueue[i+1];
        }
        GlobalTaskQueue.task_count--;
        pthread_mutex_unlock(&taskMutex);

        ExecuteTask(&Task,*(int *)args);
    }
}

int main (int argc, char *argv[])
{

    srand(time(NULL));
    int count = 0;


    pthread_cond_init(&taskCond,NULL);
    //creating 100 task in the task queue

    for(int i = 0;i <10; i++)
    {
        Task_t t1;
        char *ptr = NULL;
        t1.a = rand()%100;
        t1.b = rand()%100;
        if (count == 4)
        {
            count = 0;
        }

        if (count  == 0)
        {
            ptr = "sum";
            strcpy(t1.fname,ptr);
            t1.taskFunction = &sum;
            submitTask(t1);
        }
        else if (count  == 1)
        {
            ptr = "product";
            strcpy(t1.fname,ptr);
            t1.taskFunction = &product;
            submitTask(t1);
        }
        else if (count == 2)
        {
            ptr = "sub";
            strcpy(t1.fname,ptr);
            t1.taskFunction = &sub;
            submitTask(t1);
        }
        else if (count == 3)
        {
            ptr = "divide";
            strcpy(t1.fname,ptr);
            t1.taskFunction = &divide;
            submitTask(t1);
        }
        ++count;
    }

    printf("Task Queue count = %d\n",GlobalTaskQueue.task_count);
    pthread_t tid[SIZE];

    for (int i = 0 ;i < SIZE; i++ )
    {
        int *index = (int *)malloc(sizeof(int));
        *index = i;
        if (0 != pthread_create(&tid[i],NULL,&startThread,index))
        {
            printf("Thread creation failed\n");
        }
    }

    for (int i = 0 ;i < SIZE; i++ )
    {
        pthread_join(tid[i],NULL);
    }

    pthread_mutex_destroy(&taskMutex);
    pthread_cond_destroy(&taskCond);
    return 0;
}



