
//process structure
typedef struct {
    int arrivalTime, priority, processorTime, memory, printers, scanners, modems, cds, pid, suspended, address, started;
} proc;

//queue structure
typedef struct queue
{
    proc *data;
    struct queue *next;
} node_t;

void push(node_t **head, proc *process);

proc *pop(node_t **head);

proc *peek(node_t **head);

int queueSize(node_t **head);
