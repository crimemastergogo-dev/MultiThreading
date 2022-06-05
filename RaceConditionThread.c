#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> /*POSIX thread Library*/
#define SIZE 3

int mails = 0;

void *Mailbox()
{
    for(int i =0 ;i < 10000;i++)
    {
        mails++;
        flag =1;
    }
    //read mail
    //increment
    //wrie mail
}

int main (int argc, char *argv[])
{
    pthread_t tid[SIZE];

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

    printf("No of mails = %d\n",mails);

    return 0;
}



