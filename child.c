/*  child.c
    Given
      filename --the name of the string file of strings to be excuted by The parent process and one of its child.
    This program uses the UNIX process control system calls (fork, exec, wait, exit,
and getpid) to create two child processes, one of which executes a system utility (ps), while
the other one reads from a file shared with the parent process. After reading from the shared
file, the parent process waits for each child process to terminate and prints its termination
status, then reads from the shared file again.

   Huachao Li
   2015-03-05
*/


#include<sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int
main( int argc, char * argv[] )
{
	char buf[256];
	FILE *fd;
	int reval;
	/* Check that a filename was given as an argument. */
	if ( argc != 2 )
	{
		fprintf( stderr, "Usage: %s filename\n"
		"where filename is the name of the string file \n", argv[0] );
		exit(1);
    }

	if( (fd = fopen( argv[1]) ) != NULL )
	{
		 if( fgets(buf, 256, fd) == NULL)
		 {
			 printf("No string in the file. \n");/* perror prints error message. */
			 exit( EXIT_FAILURE );
		 }
		 else
		 {
                          /* The parent process print the first line of the file. */
			 printf( "%s", buf);
		 }
	}
	else
	{
		perror("error! No file exists. \n");/* perror prints error message. */
		exit( EXIT_FAILURE );
	}

	retval = fork();/* Create child 1 process. */
	if(retval == 0)
	{
		/* This is the child 1 process */
		printf("The clild 1 ID is:  %s\n", pid_t getpid(void));
		printf("The parent ID is:  %s\n", pid_t getppid(void));
		execlp("ps -l"); /* Load the program */
	}
	else if (retval < 0)
	{
		perror("error! Child process is not created.\n");/* perror prints error message. */
		exit( EXIT_FAILURE );
	}
	else
	{
		/* This is the parent process */
		sleep (1); /* Give my child time to start. */
		waitpid(retval, 0, 0); /* Wait for my child to terminate. */
	}

	retval = fork();/* Create child 2 process. */
	if(retval == 0)
	{
		/* This is the child 2 process */
		printf("The clild 2 ID is:  %s\n", pid_t getpid(void));
		printf("The parent ID is:  %s\n", pid_t getppid(void));
		fgets(buf, 256, fd);
		/* print the read line of the file.  */
        printf("%s\n", buf);
	}
	else if (retval < 0)
	{
		perror("error! Child process is not created.\n");/* perror prints error message. */
		exit( EXIT_FAILURE );
	}
	else
	{
		/* This is the parent process */
		sleep (1); /* Give my child time to start. */
		waitpid(retval, 0, 0); /* Wait for my child to terminate. */
	}
    exit( EXIT_SUCCESS );
}