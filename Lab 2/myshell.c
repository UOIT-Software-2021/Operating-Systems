#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include "myshell.h"

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

#define test 1
int main (int argc, char *argv[])
{
	//create variables that need independence from loop
	char pwd[256]; 
	getcwd(pwd,256);
	char *command;
	FILE *batchptr;
	if (argc > 1){
		batchptr = fopen(argv[1], "r");
	}
	while (1) {
		//stores command from user
		char input[512];
		char allWhiteSpace = 1;
		while (allWhiteSpace){
			//get command from user or from batch file
			//input[0]=' ';
			if (argc == 1){
				input[0]='\n';
				printf("%s/myshell $ ", pwd);
				
				//read entire line
				fgets(input, 512, stdin);
			} else {
				if (fgets(input, 512, batchptr) == NULL) {
					fclose(batchptr);
					return 0;//quit shell if at end of batch file
				}
			}
			for (int i =0; i < strlen(input); i++){
				if (!isspace(input[i])) {
					allWhiteSpace = 0;
					break;
				}
			}
		}
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

		//first token is the desired command
		command = malloc(sizeof(token));
		strcpy(command, token);

		//get next token
		token = strtok(NULL, " ");
		
		for (int i = 0; i < argCount; i++){
			
			//get next token
			if (token != NULL){
				if (strcmp(token, "<")==0){
				//handle input redirection
				}
				else if (strcmp(token, ">")==0){
					//handle output redirection with truncation
				}
				else if (strcmp(token, ">>")==0){
					//handle output redirection with appending
				}
				else if (strcmp(token, "&")==0){
					//handle process in the background
				}
				else {
					//get next argument
					//arguments[i] =char[256];
					arguments[i] = malloc(sizeof(token));
					strcpy(arguments[i], token);

								
		    		}

				token = strtok(NULL, " ");			
			}
 
			//all tokens have been read: set remaining args to null
			else {
				//last arg should be null when using exec
				arguments[i] = NULL;
			}			
		}
		
		//restore input
		strcpy(input, inputCopy);
		if (strcmp(command, "quit")==0){
			//exit loop
			free(command);	
			for (int i = 0; i < argCount; i++){
				free(arguments[i]);
			}			
			break; 
		} 

		else if (strcmp(command, "cd")==0){
			token = strtok(arguments[0], " ");
			if (token == NULL){
				printf("%s\n", pwd);
			}
			else if (strcmp(arguments[0],"..")==0){
				deleteEndStr(pwd,'/');
			}
			else if(access(arguments[0], F_OK) != -1){
				strcat(pwd, "/");
				strcat(pwd, token); 
			} else {
				printf("Directory does not exist!\n");
			}
		} 

		else if (strcmp(command, "clr")==0){
			clearScreen();
		} 

		else if (strcmp(command, "dir")==0){
			readDirectoryContent(pwd);
		} 

		else if (strcmp(command, "environ")==0){
			//list environment strings???
		} 

		else if (strcmp(command, "echo")==0){

			for (int i = 5; i < strlen(input); i++){				
				printf("%c",input[i]);
			}			
			printf("\n");
		} 
		
		else if (strcmp(command, "help")==0){
			//display help manual
		}
		
		else if (strcmp(command, "pause")==0){
			pauseShell();
		} 
		else if(strlen(command)>0){
			//attempt to execute the command
			if (executeCommand(command, arguments) == -1){
				printf("myshell: %s: Could not execute\n", command);
			}
		}
		free(command);
		for (int i = 0; i < argCount; i++){
			free(arguments[i]);
		}
	}
	

	return 0;
}
