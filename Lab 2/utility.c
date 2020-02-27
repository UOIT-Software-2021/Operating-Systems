#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>


//fork and execute the specified command using the child process
//wait for command to complete before proceeding
int executeCommand(const char *pathName, char *const argv[]){
	pid_t pid = fork();

	//parent: wait
	if (pid != 0){
		wait(NULL);
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
