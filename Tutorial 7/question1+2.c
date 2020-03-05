#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

struct queue *headPtr, head;

struct proc {
    char name[256];
    int priority;
    int pid;
    int runtime;
};

struct queue {
    struct proc process;
    struct queue *linkedList;
};

//Adds a node to the tail
void push(struct proc process){
    
}

//pops a node from the front of the head and reads it
struct proc pop() {

}

int delete_pid(int pid){


    return NULL;
}

int main(void)
{
    //file setup
    struct proc *headProcPtr, headProc;

    char path[64];

    printf("Please enter the path of the process.txt...\n");
    scanf("%[^\n]s", path);

    FILE *processFile = fopen(path, "r");

    if (processFile == NULL){
        printf("No file in that location...\n");
        return 1;
    }

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    struct proc procs[10];

    //j is the currecnt procs number listed
    int j = 0;

    //line contains the text and read contains the line length
    while ((read = getline(&line, &len, processFile)) != -1) {
        int i = 0;
        int iterationID = 0;
        char buffer[128] = "";

        for (i = 0; i < (unsigned int)read; i++){
            if ((!line[iterationID]) == ','){
                buffer[i] = line[iterationID];
            }
            else {
                break;
            }
            iterationID++;
        }

        *procs[j].name = buffer;
        buffer[128] = "";

        for (; i < (unsigned int)read; i++){
            if ((!line[iterationID]) == ','){
                buffer[i] = line[iterationID];
            }
            else {
                break;
            }
            iterationID++;
        }

        //probably will throw an error, 
        //will need to find a way to allow nubmers to be taken into account
        procs[j].priority = buffer;
        buffer[128] = "";

        for (; i < (unsigned int)read; i++){
            if ((!line[iterationID]) == ','){
                buffer[i] = line[iterationID];
            }
            else {
                break;
            }
            iterationID++;
        }

        procs[j].pid = buffer;
        buffer[128] = "";

        for (; i < (unsigned int)read; i++){
            if ((!line[iterationID]) == ','){
                buffer[i] = line[iterationID];
            }
            else { 
                break;
            }
            iterationID++;
        }

        procs[j].runtime = buffer;
        buffer[128] = "";

        j++;
    }

    printf(procs[0].name);


}

