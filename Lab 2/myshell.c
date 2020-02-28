#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h> 
#include <fcntl.h>
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
	} /*else {
		//clear screen if human user
		clearScreen();
	}*/
	
	while (1) {
		//stores command from user
		char input[512];
		char allWhiteSpace = 1;
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

		char *last;
		while(token != NULL){
			argCount++;
			last = strdup(token);
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
		//hold copies of stdin or out if needed or to redirect to stdin or stdout		
		int stdincopy, stdoutcopy, out = -230, in = -432;	
		//set certain execution statuses
		int restoreoriginalstdin = 0, restoreoriginalstdout = 0, notbgprocess = 1;
		
		//first token is the desired command
		command = strdup(token);
		//get next token
		token = strtok(NULL, " ");
		int tempargcount = 0;
		
		if (strcmp(last, "&")==0){
			//handle process in the background
			argCount -=2;
			notbgprocess = 0;			
		}
		free(last);
		for (int i = 0; i < argCount; i++){
			
			//get next token
			if (token != NULL){
				if (strcmp(token, "<")==0&& !restoreoriginalstdin){
				
					token = strtok(NULL, " ");
					i++;					
					infname = strdup(token);
					int in = open(infname,O_RDONLY);
					
					if(in) {
						stdincopy = dup(fileno(stdin));
						if (-1 == dup2(in, fileno(stdin))) { 
							perror("cannot redirect stdin");
							command = strdup("");
							free(infname);
							close(in);
						} else {
				      			restoreoriginalstdin = 1;
						}
					} else {
						command = strdup("");
						free(infname);
						perror("myshell: cannot open file to read");	 
					}
				}
				else if (strcmp(token, ">")==0 && !restoreoriginalstdout){
					token = strtok(NULL, " ");
					i++;
					outfname = strdup(token);
					int out = open(outfname,O_WRONLY|O_CREAT|O_TRUNC, 0600);
					
					if(out) {
						stdoutcopy = dup(fileno(stdout));
						if (-1 == dup2(out, fileno(stdout))) { 
							perror("cannot redirect stdout");
							command = strdup("");
							free(outfname);
							close(out);
						} else {
				      			restoreoriginalstdout = 1;
						}
					} else {
						command = strdup("");
						free(outfname);
						perror("myshell: cannot open file to write");	 
					}
				}
				else if (strcmp(token, ">>")==0 && !restoreoriginalstdout){	
					token = strtok(NULL, " ");
					i++;					
					outfname = strdup(token);
					int out = open(outfname,O_WRONLY|O_CREAT|O_APPEND, 0600);
					
					if(out) {
						stdoutcopy = dup(fileno(stdout));
						if (-1 == dup2(out, fileno(stdout))) { 
							perror("cannot redirect stdout");
							command = strdup("");
							free(outfname);
							close(out);
						} else {
				      			restoreoriginalstdout = 1;
						}
					} else {
						command = strdup("");
						free(outfname);
						perror("myshell: cannot open file to append");	 
					}
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
				break;
			}			
		}
		
		argCount = tempargcount;

		//append NULL if needed
		if (argCount > 0){
			if (arguments[argCount-1] != NULL){
				arguments[argCount] = NULL;
				argCount+= 1;
			}
		} else {
			arguments[0] = NULL;
			argCount = 1;
		}	
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
				perror("Directory does not exist!\n");
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
					perror("myshell: error: could not open specified directory\n");
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
			help(manualPath);
		}
		
		else if (strcmp(command, "pause")==0){
			//pause the shell
			pauseShell();
		} 
		else if(strlen(command)>0){
			//attempt to execute the command
						
			if (executeCommand(command,argCount, arguments, notbgprocess)== -1){
				perror("myshell: error: Could not execute\n");
			}
			
		}

		//free variables
		free(command);
		for (int i = 0; i < argCount; i++){
			
			free(arguments[i]);
		}


		//restore stdin and stdout
		if(restoreoriginalstdin){
			//flush and close stream
			fflush(stdin);  
              		close (in);
			//free memory 
			free(infname);

			//restore stdin              		
			dup2(stdincopy, fileno(stdin));
			close (stdincopy);							
			restoreoriginalstdin = 0;
		}
		if(restoreoriginalstdout){
			//flush and close stream
			fflush(stdout);  
              		close (out);
			//free memory
			free(outfname);
            		
			//restore stdout			
			dup2(stdoutcopy, fileno(stdout));
			close(stdoutcopy);
			restoreoriginalstdout = 0;
		}

	}
	

	return 0;
}

