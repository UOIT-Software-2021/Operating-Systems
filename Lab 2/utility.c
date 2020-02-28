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
int executeCommand(char *pathName, int argCount, char *const argv[],int notbgprocess){
	char *arguments[argCount+1];
	
	arguments[0] = pathName;
	for (int i = 1; i < argCount+1; i++){
		arguments[i]=argv[i-1];
	}
	
	if (notbgprocess){
		pid_t pid = fork();
		//parent: wait
		int wstatus;
		if (pid > 0){
			
				//return child exit status
				char es = 0;
				waitpid(pid, &wstatus, 0);			
				if( WIFEXITED(wstatus) ) {
					es = WEXITSTATUS(wstatus);
				}
				return es;
			
		} 
		//child: execute function	
		else {
			
			int status = execv(pathName, arguments);
					
			//should only be reached if an error occurs		
			if (status == -1){
				exit(-1);//release child process
			}		
		}
	} else {
		pid_t pid = fork();
		//parent has things to do: return
		if (pid > 0){
			return 0;
		}
		else {
			pid_t pid2 = fork();
			//parent: wait
			int wstatus;
			if (pid2 > 0){
				
				//return child exit status
				char es = 0;
				waitpid(pid2, &wstatus, 0);			
				if( WIFEXITED(wstatus) ) {
					es = WEXITSTATUS(wstatus);
				}
				if (es == -1){
					perror("myshell: error: Could not execute");
				}
				exit(0);
				
			} 
			//child: execute function	
			else {
				
				int status = execv(pathName, arguments);
						
				//should only be reached if an error occurs		
				if (status == -1){
					exit(-1);//release child process
				}		
			}
		}
	}
	return 0;
	
}

int checkIfDirectory(const char *directoryName){
	DIR* dir = opendir(directoryName);
	if (dir) {
	    	closedir(dir);
		return 1;
	} else {
		return 0;
	}
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

void help(char *path){
	int c;
	FILE *file;
	file = fopen(path, "r");
	if (file) {
    		while ((c = getc(file)) != EOF)
        		putchar(c);
    		fclose(file);
	}
}	
