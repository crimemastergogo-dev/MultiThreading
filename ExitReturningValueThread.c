#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h> /*POSIX thread Library*/
#define SIZE 2

void *roll_dice(void* tid)
{
    int value = (rand() % 6) +1;
    int *temp = malloc(sizeof(int));

    *temp = value;
    printf("Value = %d \n",value);

  //  return (void *)temp;
  while(1);
    pthread_exit((void *)temp);
}

int main (int argc, char *argv[])
{
    srand(time(NULL));
    pthread_t tid[SIZE];

    for (int i = 0 ;i < SIZE; i++ )
    {
        if (0 != pthread_create(tid +i,NULL,roll_dice,NULL))
        {
            printf("Thread creation failed\n");
        }

    }

//    return 0;
    pthread_exit(0);
    for (int i = 0 ;i < SIZE; i++ )
    {
        int *result = NULL;
        if (0 != pthread_join(tid[i],(void **)&result))
        {
            printf("Thread joinng failed\n");
        }
        printf("result = %d \n",*result);

        if (result != NULL)
            free(result);
    }

    //return 0;
}
