
int executeCommand(char *pathName, int argCount,char *const argv[], int notbgprocess);

int checkIfDirectory(const char *directoryName);

int readDirectoryContent(const char *directoryName);

void pauseShell(void);

void clearScreen(void);

void help(char *path);

void echo(int argc, char *args[]);

void printEnv(char **environ);
