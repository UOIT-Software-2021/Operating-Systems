

typedef struct {
    int arrivalTime, priority, processorTime, memory, printers, scanners, modems, cds, pid;
} proc;

//queue structure
typedef struct queue
{
    proc data;
    struct queue *next;
} node_t;

void push(node_t **head, proc process);

proc pop(node_t **head);
