#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int main(void)
{
    pid_t parent = getpid();
    pid_t pid = fork();

    if (pid == -1)
    {
        printf("Fork failed.");
    } 
    else if (pid > 0)
    {
        int status;
        waitpid(pid, &status, 0);
    }
    else 
    {
        pid_t pid_process = getpid();
        char * argv_list[] = {"process",NULL}; 
        execv("process", argv_list);
        sleep(5);
        kill(pid_process, SIGINT);
    }
}


