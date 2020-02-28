
int executeCommand(const char *pathName, char *const argv[], int notbgprocess);

int checkIfDirectory(const char *directoryName);

int readDirectoryContent(const char *directoryName);

void pauseShell(void);

void clearScreen(void);

void readMan(char *path);

void echo(int argc, char *args[]);

void printEnv(char **environ);
