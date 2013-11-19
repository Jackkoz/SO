#include <unistd.h> //OS
#include <stdio.h> //sprintf
#include <stdlib.h> //exit
#include <string.h> //strlen
#include "so_lib.h"

int main() {

	char buffer[50], buffer2[50];

	// char buffer[] = "Hello brave new world\n";
	// write(2, buffer, sizeof(buffer) + 1);

	read(0, buffer, 50);
    sprintf(buffer2, "I'm #%d and got msg from #%s\n", getpid(), buffer);
    write(2, buffer2, strlen(buffer2) + 1);

	sprintf(buffer, "%d", getpid());
    write(1, buffer, strlen(buffer) + 1);
    
	return 0;
	exit(0);
}