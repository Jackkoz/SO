#include <unistd.h> //OS
#include <stdio.h> //sprintf
#include <stdlib.h> //exit
#include <string.h> //strlen
#include "so_lib.h"
#include <sys/wait.h>

int main() {

	char buffer[50], buffer2[50];

	// char buffer[] = "Hello brave new world\n";
	// write(2, buffer, sizeof(buffer) + 1);

	read(0, buffer, 50);
    sprintf(buffer2, "I'm #%d and got msg from #%s\n", getpid(), buffer);
    write(2, buffer2, strlen(buffer2) + 1);

	sprintf(buffer, "%d", getpid());
    write(1, buffer, strlen(buffer) + 1);
    
    while (true) {
        read(0, buffer, 50);
        if (strcmp(buffer, kill_message) == 0) {
            write(2, "GOT KILLMSG\n", strlen("GOT KILLMSG\n") + 1);
            write(1, kill_message, strlen(kill_message) + 1);
            return 0;
            exit(0);
        }
        
        if (isExpression(buffer) == 0)
            write(1, buffer, strlen(buffer) + 1);
        else {
            
        }
    }
    
	// return 0;
	// exit(0);
}
