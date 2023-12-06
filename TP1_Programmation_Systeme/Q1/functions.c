#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "functions.h"


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
