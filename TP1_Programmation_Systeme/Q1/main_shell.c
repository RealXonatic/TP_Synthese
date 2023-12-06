#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "functions.h"
#include "functions.c"



int main (int argc, char ** argv) {
    welcome_message();
    prompt();
	return 0 ;
}
