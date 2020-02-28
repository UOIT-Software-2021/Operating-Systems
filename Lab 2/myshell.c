#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h> 
#include "myshell.h"

extern char **environ;

void deleteEndStr (char* str, char del){

    char *toDel = &str[strlen(str)];

    while (toDel > str && *toDel != del){
		toDel--;	
	}
    
    if (*toDel== '/'){
		*toDel= '\0';
	}

    return;
}

int main (int argc, char *argv[])
{
	//create variables that need independence from loop

	//working directory
	char pwd[256]; 
	getcwd(pwd,256);
	
	//path to manual
	char manualPath[256];
	strcpy(manualPath,pwd);
	strcat(manualPath, "/readme");
	putenv(manualPath);

	//path to shell
	char shell[256];
	strcpy(shell,pwd);
	strcat(shell, "/myshell");
	setenv("shell", shell, 1);	
	
	//string to hold shell command
	char *command;

	//pointer to possible batch file
	FILE *batchptr;
	if (argc > 1){
		batchptr = fopen(argv[1], "r");
	} else {
		//clear screen if human user
		clearScreen();
	}
	
	while (1) {
		//stores command from user
		char input[512];
		char allWhiteSpace = 1;

		//hold copies of stdin or out if needed		
		int stdin_copy = 0, stdout_copy = 0;	
		
		//set certain execution statuses
		int restoreoriginalstdin = 0, restoreoriginalstdout = 0, notbgprocess = 1;
		//do not progress if buffer only has white space		
		while (allWhiteSpace){
			//get command from user or from batch file
			//input[0]=' ';
			if (argc == 1){
				printf("\033[0;36m"); 
				printf("%s/myshell $ ", pwd);
				printf("\033[0m"); 
				//read entire line
				fgets(input, 512, stdin);
			} else {
				if (fgets(input, 512, batchptr) == NULL) {
					fclose(batchptr);
					return 0;//quit shell if at end of batch file
				}
			}
			//check each character in input buffer and break if not space
			for (int i =0; i < strlen(input); i++){
				if (!isspace(input[i])) {
					allWhiteSpace = 0;
					break;
				}
			}
		}
		//set last character from '\n' to 0
		input[strlen(input)-1]=0;
		//copy of input to use as backup
		char inputCopy[512];
		strcpy(inputCopy, input);

		//first pass of command: count number of tokens
		int argCount = 0;
		char *token = strtok(input, " ");

		while(token != NULL){
			argCount++;

			token = strtok(NULL, " ");			
						
	    	}

		//restore input
		strcpy(input, inputCopy);
		//reset token for second pass
		token = strtok(input, " ");
							
		//to store arguments		
		char *arguments[argCount];

		//needed in the case of io redirection		
		char *infname;	
		char *outfname;

		//first token is the desired command
		command = strdup(token);
		//get next token
		token = strtok(NULL, " ");
		int tempargcount = 0;
		for (int i = 0; i < argCount; i++){
			
			//get next token
			if (token != NULL){
				if (strcmp(token, "<")==0){
					token = strtok(NULL, " ");
					infname = strdup(token);
					if( access( infname, F_OK|R_OK ) != -1 && !restoreoriginalstdin) {
						//dup2(stdin_copy, 0);
						
					      	freopen(infname, "r", stdin);
						  
				      		restoreoriginalstdin = 1;
					} else {
						command = strdup("");
						perror("myshell: cannot open file to read");	 
					}
				}
				else if (strcmp(token, ">")==0){
					token = strtok(NULL, " ");
					outfname = strdup(token);
					printf("%s\n",outfname);
					if((access(outfname, W_OK ) != -1 || access(outfname, F_OK) == -1)&& !restoreoriginalstdout) {
						//dup2(stdout_copy, 1);
						
					      	freopen(outfname, "w", stdout);
						//freopen("test.txt", "w", stdout);  
					
						restoreoriginalstdout = 1;
					} else {
						strcpy(command, "");
						perror("myshell: cannot open file to write");	 
					}
				}
				else if (strcmp(token, ">>")==0){	
					token = strtok(NULL, " ");
					outfname = strdup(token);
					if(( access(outfname, W_OK ) != -1 || access(outfname, F_OK) == -1)&& !restoreoriginalstdout) {
						//dup2(stdout_copy, 1);
					      	freopen(outfname, "a", stdout);  
				      		restoreoriginalstdout = 1;
					} else {
						strcpy(command, "");
						perror("myshell: cannot open file append"); 
					
					}
				}
				else if (strcmp(token, "&")==0){
					//handle process in the background
					notbgprocess = 0;
					
				}
				else {
					//get next argument
					arguments[i] = strdup (token);
					tempargcount++;
								
		    		}

				token = strtok(NULL, " ");			
			}
 
			//all tokens have been read: set remaining args to null
			else {
				//last arg should be null when using exec
				arguments[i]=NULL;
				tempargcount++;
				break;
			}			
		}
		argCount = tempargcount;

		//long if-else chain to choose appropriate command
		if (strcmp(command, "quit")==0){
			//exit loop: then exit shell
			free(command);	
			for (int i = 0; i < argCount; i++){			
				free(arguments[i]);
			}			
			break; 
		} 

		else if (strcmp(command, "cd")==0){
			
			//select option based on number of args
			if (argCount==1){
				printf("%s\n", pwd);
			}
			else if(checkIfDirectory(arguments[0])){
				chdir(arguments[0]);
				getcwd(pwd,256);
			} else {
				printf("Directory does not exist!\n");
			}
		} 

		else if (strcmp(command, "clr")==0){
			//clear screen
			clearScreen();
		} 

		else if (strcmp(command, "dir")==0){
			//print directory contents
			if (argCount == 1){
				readDirectoryContent(pwd);
			} else {
				if( readDirectoryContent(arguments[0])) {
					printf("myshell: error: could not open specified directory\n");
				}
			}
		
		} 

		else if (strcmp(command, "environ")==0){
			//list environment strings
			printEnv(environ);
		} 

		else if (strcmp(command, "echo")==0){
			//echo rest of line
			echo(argCount, arguments);
		} 
		
		else if (strcmp(command, "help")==0){
			//display help manual
			readMan(manualPath);
		}
		
		else if (strcmp(command, "pause")==0){
			//pause the shell
			pauseShell();
		} 
		else if(strlen(command)>0){
			//attempt to execute the command
			if (executeCommand(command, arguments, notbgprocess)== -1){
				printf("myshell: %s: Could not execute\n", command);
			}
		}

		//free variables
		free(command);
		for (int i = 0; i < argCount; i++){
			
			free(arguments[i]);
		}


		//restore stdin and stdout
		if(restoreoriginalstdin){
			fflush(stdin);  
              		fclose(stdin);
			free(infname);              		
			stdin = fdopen(stdin_copy, "r");
			restoreoriginalstdin = 0;
		}
		if(restoreoriginalstdout){
			fflush(stdout);  
              		fclose(stdout);

			free(outfname);
            		
			stdout = fdopen(stdout_copy, "w");
			dup2(stdout_copy, 1);

			close(stdout_copy);
			restoreoriginalstdout = 0;
		}

	}
	

	return 0;
}

