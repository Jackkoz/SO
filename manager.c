#include <unistd.h> //linux
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "so_lib.h"
#include <fcntl.h> //open
#include <sys/wait.h>

int main(int argc, char* argv[]) {

	int     process_count;         /* number of this process (starting with 1) */
    int     childpid;              /* indicates process should spawn another   */
    int     max_processes;         /* total number of processes in ring        */
    int     fd[2];                 /* file descriptors returned by pipe        */
    int     error;                 /* return value from dup2 call              */

	char buffer[50], buffer2[50];

	if (argc != 2) {
		fprintf (stderr, "Usage: %s max_processes, input, output\n", argv[0]);
        exit(1);	
	}

	max_processes = atoi(argv[1]);

	/* check command line for a valid number of processes to generate */
    if (max_processes <= 0) {
        fprintf (stderr, "Max processes should be greater than 0.\n", argv[0]);
        exit(1);
    }

    if (pipe (fd) == -1) {
        perror("Could not create pipe");
        exit(1);
    }

    if ((dup2(fd[0], STDIN_FILENO) == -1) ||
            (dup2(fd[1], STDOUT_FILENO) == -1)) {
        perror("Could not dup pipes");
        exit(1);
    }

    if ((close(fd[0]) == -1) || (close(fd[1]) == -1)) {
        perror("Could not close extra descriptors");
        exit(1);
    }

	 for (process_count = 1; process_count <= max_processes; process_count++) {
        if (pipe (fd) == -1) {
            fprintf(stderr,"Could not create pipe %d: %s\n",
	            	process_count, strerror(errno));
            exit(1);
        }

        if ((childpid = fork()) == -1) {
            fprintf(stderr, "Could not create child %d: %s\n",
	            	process_count, strerror(errno));
            exit(1);
        }

        if (childpid > 0)
        	error = dup2(fd[1], STDOUT_FILENO);
        else
	    	error = dup2(fd[0], STDIN_FILENO);

        if (error == -1) {
            fprintf(stderr, "Could not dup pipes for iteration %d: %s\n",
	            	process_count, strerror(errno));
            exit(1);
        }

        if ((close(fd[0]) == -1) || (close(fd[1]) == -1)) {
            fprintf(stderr, "Could not close extra descriptors %d: %s\n",
	            	process_count, strerror(errno));
            exit(1);
        }

        if (childpid)
            execl("exec", "./exec", NULL);
    }

    sprintf(buffer, "%d", getpid());
    write(1, buffer, strlen(buffer) + 1);
    
    read(0, buffer, 50);
    sprintf(buffer2, "I'm #%d and got msg from #%s\n", getpid(), buffer);
    write(2, buffer2, strlen(buffer2) + 1);

    int input_file;
    input_file = open("in.in", O_RDONLY);

    int expressions = 0;
    read(input_file, buffer, 50);
    expressions = atoi(buffer);

    sprintf(buffer, "Number of expressions: %d\n", expressions);

    write(2, buffer, strlen(buffer) + 1);
    
    int processed_expressions = 0;
    
    while (processed_expressions < expressions) {
		
		read(input_file, buffer, 50);
		while (isExpression(buffer) > 0) {
			write(1, buffer, strlen(buffer) + 1);
			read(0, buffer, 50);
		}
		write(2, buffer, strlen(buffer) + 1);
		
		processed_expressions++;
	}
	
	write(1, kill_message, strlen(kill_message) + 1);
	
	process_count = 0;	
	while (process_count < max_processes) {
		read(0, buffer, 50);
		wait(0);
		process_count++;
	}
	
	write(2, "END\n", strlen("END\n") + 1);
	
	close(input_file);

	return 0;
	exit(0);
}

//getline i printf+flush
