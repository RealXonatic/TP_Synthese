#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "functions.h"

#define MAX_INPUT_SIZE 1024


void welcome_message(){
	char message[] = "$ ./enseash \nBienvenue dans le Shell ENSEA. \nPour quitter, tapez 'exit'. \n";
	int message_length = sizeof(message)-1;
	write(1,message,message_length);
}	

void prompt(){
	char message[] = "enseash %";
	int message_length = sizeof(message)-1;
	write(1,message,message_length);
}

void exec_command_loop(){
	char input[MAX_INPUT_SIZE];
	
	while(1){
		prompt();
		
		if (read(0, input, MAX_INPUT_SIZE) == -1){
			perror("error reading input");
			break;
		}
		
		input[strcspn(input, "\n")] = '\0';
		
		//le user écrit exit
		if (strcmp(input, "exit") == 0){
			char exit_msg[] = "Bye Bye ! !\n";
			write(1, exit_msg, strlen(exit_msg));
			break;}
		
		
		//fork d'un processus enfant	
		pid_t pid = fork();
		
		if (pid < 0){
			perror("Fork failed");
			exit(EXIT_FAILURE); }
			
		else if (pid == 0){
			execlp(input, input, NULL);
			
			//s'il y a un problème avec execlp
			char error_msg[] = "Command execution failed\n";
			write(1, error_msg, strlen(error_msg));
			exit(EXIT_FAILURE);}
		
		else {
			wait(NULL);}
	}
}
