#define MEMORY 1024

//resources structure
typedef struct {
    int printers, scanners, modems, cds;
} resources;

int set_mem(int *avail_mem, int memory, int p);

void free_mem(int *avail_mem, int address, int memory, int val);

void pushByQueue(node_t **rt, node_t **p1, node_t **p2, node_t **p3, proc process, int level);

int realtimeIterate(node_t **realtime, int * avail_mem, resources res_avail);

int priorityIterate(node_t **priority1, node_t **priority2, node_t **priority3, int * avail_mem, resources res_avail, int level);
