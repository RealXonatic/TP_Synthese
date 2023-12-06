#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "functions.h"
#include <time.h>
#define MAX_INPUT_SIZE 1024


void welcome_message(){
	char message[] = "$ ./enseash \nBienvenue dans le Shell ENSEA. \nPour quitter, tapez 'exit'. \n";
	int message_length = sizeof(message)-1;
	write(1,message,message_length);
}	

void prompt(){
	char message[] = "enseash % ";
	int message_length = sizeof(message)-1;
	write(1,message,message_length);
}

void exec_command_loop(){
	
	struct timespec start_time, end_time;
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
		    // Tokenisation des arguments
			char* args[MAX_INPUT_SIZE];
			int i = 0;
			char* token = strtok(input, " ");
			while (token != NULL) {
			args[i++] = token;
			token = strtok(NULL, " ");
			}
			args[i] = NULL; // La liste d'arguments doit se terminer par NULL

			execvp(args[0], args);
			clock_gettime(CLOCK_MONOTONIC, &start_time);

			/*execlp(input, input, NULL);*/
			
			//s'il y a un problème avec execlp
			char error_msg[] = "Command execution failed\n";
			write(1, error_msg, strlen(error_msg));
			exit(EXIT_FAILURE);}
			
		else {// Processus parent
			int status;
			waitpid(pid, &status, 0);
			//printf("end time = %ld", start_time.tv_nsec);
	      	
			if (WIFEXITED(status)) {
				clock_gettime(CLOCK_MONOTONIC, &end_time);
				//printf("end time = %ld", end_time.tv_nsec);
				long elapsed_time = abs(end_time.tv_nsec - start_time.tv_nsec)/1000000;
				printf("enseash [exit:%d|%ld ms] %% ", WEXITSTATUS(status), elapsed_time);
				fflush(stdout);}
				
			else if (WIFSIGNALED(status)) {
				clock_gettime(CLOCK_MONOTONIC, &end_time);
				long elapsed_time = abs(end_time.tv_nsec - start_time.tv_nsec)/1000000;
				printf("enseash [sign:%d|%ld ms] %% ", WIFSIGNALED(status), elapsed_time);
				fflush(stdout);}}
}};


