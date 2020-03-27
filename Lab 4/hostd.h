#define MEMORY 1024

typedef struct {
    int printers, scanners, modems, cds;
} resources;

int set_mem(int *avail_mem, int memory, int p);

void free_mem(int *avail_mem, int address, int memory, int val);
