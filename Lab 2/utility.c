#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


//fork and execute the specified command using the child process
//wait for command to complete before proceeding
int executeCommand(const char *pathName, char *const argv[], int notbgprocess){
	pid_t pid = fork();

	//parent: wait
	int wstatus;
	if (pid != 0){
		if (notbgprocess){
			//return child exit status
			char es = waitpid(pid, &wstatus, 0);			
			if( WIFEXITED(wstatus) ) {
				es = WEXITSTATUS(wstatus);
			}
			return es;
		}
	} 
	//child: execute function	
	else {
		
		int status = execv(pathName, argv);
				
		//should only be reached if an error occurs		
		if (status == -1){
			exit(-1);//release child process
		}		
	}
	return 0;
	
}

void help(){
	int c;
	FILE *file;
	file = fopen("readme", "r");
	if (file) {
    	while ((c = getc(file)) != EOF)
        putchar(c);
    fclose(file);
	}
}

/* 
void pauseShell(){
    char temp;
    printf("myshell: paused: press enter to resume ");
    
    //scan for single characters until user presses enter            
    do {
        scanf("%c",&temp);
    } while(temp!='\n');
} */

int checkIfDirectory(const char *directoryName){
	struct stat directoryStat;
	stat(directoryName, &directoryStat);
	return S_ISREG(directoryStat.st_mode);
}

//read the contents of the current directory
int readDirectoryContent(const char *directoryName){
	//pointer for 	dirent structure
	struct dirent *dp;
	//directory pointer
	DIR *dir = opendir(directoryName);

	// Unable to open directory
	if (dir==0) {
		return 1; 
	}
	
	//print the contents of directory
	while ((dp = readdir(dir)) != NULL)
	{
		printf("%s\n", dp->d_name);
	}

	// Close directory stream
	closedir(dir);

	return 0;
}

void printEnv(char **environ){
	for (char **env = environ; *env != 0; env++) {
		char *thisEnv = *env;
		printf("%s\n", thisEnv);    
	}

}

void pauseShell(void){
	char temp;
	printf("myshell: paused: press enter to resume ");

	//scan for single characters until user presses enter            
	do {
		scanf("%c",&temp);
	} while(temp!='\n');
}

void clearScreen(void) {
	//char buffer that acts as a screen clear
	const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
	write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

void echo(int argc, char *args[]){
	for(int i = 0; i < argc; i++){
		if (args[i] == NULL){
			break;
		}
		printf("%s ",args[i]);
	}
	printf("\n");
}

void readMan(char *path){
	printf("\n");
	FILE *fp;
	fp = fopen(path, "r");
	char buffer[256];
	while(fgets(buffer, 256, fp) != NULL){
		printf("%s", buffer);
	}
	fclose(fp);
	printf("\n");
}	
