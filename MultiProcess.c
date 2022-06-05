#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int a = 2;

int main (int argc, char *argv[])
{

    int pid = fork();

    if (pid == -1)
    {
        printf("Process creation failed...!!!");
    }
    if (pid == 0)
    {
        a++;
    }
    else 
    {
        a += 100;
    }

    sleep(2);

    printf("Process Id = %d\n",getpid());
    printf(" a = %d\n",a);
    if (pid != 0)
    {
        wait(NULL);
    }

    while (1);

    return 0;
}



