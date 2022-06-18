#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> /*POSIX thread Library*/
#include <semaphore.h>
#include <time.h>
#define SIZE 4
#define QUEUE_SIZE 256

typedef struct task
{
    int a,b;
}Task_t;

typedef struct Queuetask
{
    Task_t TaskQueue[QUEUE_SIZE];
    int task_count;
}TaskQueue_t;

TaskQueue_t GlobalTaskQueue;

pthread_mutex_t taskMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t taskCond;

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
    int result = p_task->a + p_task->b;
    printf("[%d]Sum of a = %d and b =%d ===> %d\n", index,p_task->a, p_task->b,result);
}

void *startThread(void *args)
{

    printf("thread num = %d\n",*(int *)args);
    Task_t Task;
    while(1)
    {
        pthread_mutex_lock(&taskMutex);
        while(GlobalTaskQueue.task_count == 0)
        {
            pthread_cond_wait(&taskCond,&taskMutex);
        }

        Task = GlobalTaskQueue.TaskQueue[0];
        int i = 0;
        for(i = 0;i < GlobalTaskQueue.task_count;i++)
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


    pthread_cond_init(&taskCond,NULL);
    //creating 100 task in the task queue

    for(int i = 0;i <10; i++)
    {
        Task_t t1;
        t1.a = rand()%100;
        t1.b = rand()%100;
        submitTask(t1);
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



