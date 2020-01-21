//  Morrison, Benjamin - Assignment 3
//  Created by Benjamin Morrison on 9/25/19.
//
//  assign3.c
//  Assignment 3
//
//  Copyright © 2019 Benjamin Morrison. All rights reserved.
//

#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<unistd.h>
#define SHMKEY ((key_t) 7890)

typedef struct
{
    int value;
}

shared_mem;
shared_mem *counter;

int child1()
{
    int line = 0;
    
    while (line < 20000)
    {
        line++;
        counter->value = counter->value + 1;
    }
    
    printf ("from child1 counter = %d\n", counter->value);
    exit (0);
}

int child2()
{
    int line = 0;
    
    while (line < 10000)
    {
        line++;
        counter->value = counter->value + 1;
    }
    
    printf ("from child2 counter = %d\n", counter->value);
    exit (0);
}

int grandchild1()
{
    int line = 0;
    
    while (line < 40000)
    {
        line++;
        counter->value = counter->value + 1;
    }
    
    printf ("from grandchild1 counter = %d\n", counter->value);
    exit (0);
}

int grandchild2()
{
    int line = 0;
    
    while (line < 30000)
    {
        line++;
        counter->value = counter->value + 1;
    }
    
    printf ("from grandchild2 counter = %d\n", counter->value);
    exit (0);
}

int main(int argc, char *argv[])
{
    //key_t key = 123; /* shared memory key */
    int shmid;
    int pid1, pid2 = 0, pid3 = 0, pid4;
    int status;
    char *shmadd; shmadd = (char *) 0;
    
    if (argc < 2)
    {
        printf("Please enter \'Y\' or \'N\' as an argument to show the corrent or incorrent amount\n");
        exit(0);
    }
    
    char arg = *(argv[1]);
    
    if (arg != 'Y' && arg != 'N')
    {
        printf("Please enter a valid argument. (Y or N)\n");
        exit(0);
    }
    
    if ((shmid = shmget (SHMKEY, sizeof(int), IPC_CREAT | 0666)) < 0)
    {
        perror ("shmget");
        exit (1);
    }
    
    if ((counter = (shared_mem *) shmat (shmid, shmadd, 0)) == (shared_mem *) -1)
    {
        perror ("shmat");
        exit (0);
    }
    
    else
        printf("Shared memory creation is successful\n");
    
    counter->value = 0;
    fflush (stdout);
    
    if ((pid1 = fork()) == 0)
    {
        printf("Parent process created\n");
        child1();
    }
    
    if (arg == 'Y')
        sleep(2);
    else if (arg == 'N') {}

    if ((pid1 != 0) && (pid2 = fork()) == 0)
        child2();
    
    if (arg == 'Y')
        sleep(3);
    else if (arg == 'N') {}
    
    if ((pid1 != 0) && (pid2 != 0) && (pid3 = fork()) == 0)
        grandchild1();

    if (arg == 'Y')
        sleep(4);
    else if (arg == 'N') {}

    if ((pid1 != 0) && (pid2 != 0) && (pid3 != 0) && (pid4 = fork()) == 0)
        grandchild2();
    
    if ((pid1 != 0) && (pid2 != 0)&& (pid3 != 0))
    {
        pid4 = wait(&status); // wait for child1 to end
        pid4 = wait(&status); // wait for child2 to end
        pid4 = wait(&status); // wait for grandchild1 to end
        pid4 = wait(&status); // wait for grandchild2 to end
        
        printf("The child processes terminated normally\n");
        printf ("from parent counter = %d\n", counter->value);
        
        /* the next set of statements releases the shared memory */
        if ((shmctl (shmid, IPC_RMID, (struct shmid_ds *) 0)) == -1)
        {
            perror ("shmctl"); exit (-1);
        }
        
        else
            printf("Shared memory has been released\n");

        exit (0);
    }
}


