/*
 *  process_control.c  -  sample solution to exercise using fork() and wait()
 *
 *  The main process reads a line from a specified text file, then creates 
 *  two child processes.  The first child invokes "ps -l" by calling 'exec'.
 *  The second child reads a line from the same file read by the parent, 
 *  echoes it to the standard output, then exits.  The parent waits for both
 *  child processes to terminate and decodes and displays their exit status.
 *  It then  tries to read another line form the input file, echoing it to the
 *  standard output.
 *
 *  Try running the program on a three-line (or longer) input file, and again
 *  on a one-line input file.  This demonstrates that child 2 receives a copy of
 *  the parent process's file pointer when it starts, but subsequent reads by the
 *  child and the parent affect the file pointer of only the process doing the reading.
 *
 *  Note the use of fflush() prior to each call to fork() and exec().
 *  Use feof() or ferror() to detect EOF or error if fgets() returns NULL.
 *
 *  J. Mohr
 *  1996 March 18, rev. 2013-03-12, 2015-03-12
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdbool.h>		/* added in C99 */


/* Function declarations. */
void child1();
void child2( FILE * fp, char buf[] );
bool readline( FILE * fp, char buf[], char *error_prefix );
void exit_status( int status );
void fatal( char* );
void fatalsys( char* );


/* Size of string buffer. */
const int BUFSIZE = 256;


int
main( int argc, char *argv[] )
{
    FILE * fp;			/* file pointer       */
    char buf[ BUFSIZE ];	/* line buffer        */
    pid_t pid1, pid2, exit_pid;	/* process IDs        */
    int i;			/* iterator           */
    int status;			/* termination status */
  
    /* Check that file name is given as command line argument. */
    if ( argc == 1 )
	fatal("ERROR: name of input file missing");  

    /* Open file. */
    if ( ( fp = fopen( argv[1], "r" )) == NULL )
	fatalsys("fopen() error"); 

    /* Read a line from the file and echo it to the standard output. */
    if ( readline( fp, buf, "Parent"))
    {
	if ( fputs( buf, stdout ) == EOF )
	    fatalsys("parent - error writing first line from file");
    }

    /* Create first child. */
    printf( "Creating child 1 ...\n" ); fflush(stdout);
    if ( ( pid1 = fork()) < 0 )	/* error occurred */
	fatalsys( "fork() failed to create child 1");
    else if ( pid1 == 0 )		/* child process */
	child1();

    /* Create second child. */
    printf( "Creating child 2 ...\n" ); fflush(stdout);
    if ( ( pid2 = fork()) < 0 )	/* error occurred */
	fatalsys( "fork() failed to create child 2");
    else if ( pid2 == 0 )		/* child process */
	child2( fp, buf );

    /* Wait for both children to terminate. */
    for ( i = 0; i < 2; i++ )
    {
	if ( ( exit_pid = wait( &status )) < 0 )
	    fatalsys("wait error");
	printf("Child %d terminated:  ", exit_pid == pid1 ? 1 : 2 );
	exit_status( status );
    }

    /* Read another line from the file and echo it to the standard output. */
    if ( readline( fp, buf, "Parent"))
    {
	if ( fputs( buf, stdout ) == EOF && ferror(stdout))
	    fatalsys("Parent - error writing second line read from file");
    }

    exit( EXIT_SUCCESS );
}


/* 
 * Child 1 announces its presence, then execs "ps -l".
 */
void 
child1()
{
    printf( "\tChild 1 PID = %d\n", getpid() ); fflush(stdout);
    execlp( "ps", "ps", "-l", NULL ); /* will only return if it fails */
    fatalsys("execlp() error");
}


/* 
 * Child 2 announces its presence, then attempts to read a line from the 
 * specified file and echo it to the standard output.
 */
void 
child2( FILE * fp, char buf[] )
{
    printf( "\tChild 2 PID = %d\n", getpid());
    sleep(1);			/* give child 1 a chance to start */

    /* Attempt to read a line from the specified file. */
    if ( readline( fp, buf, "Child 2"))
    {
	/* Write the line to the standard output. */
	printf("\t");		/* indent child's output */
	if (fputs( buf, stdout ) == EOF && ferror(stdout))
	    fatalsys("Child 2 - error writing to stdout");
    }

    exit( EXIT_SUCCESS );
}

 
/* 
 * Read a line from the specified file into the provided string buffer,
 * checking for EOF and read errors and writing error messages accordingly.
 * Returns true if the read is successful, or false if EOF is encountered.
 * Writes a system error message and exits if a read error occurs.
 */
bool
readline( FILE * fp, char buf[], char *error_prefix )
{
    bool status = true;

    if ( fgets( buf, BUFSIZE, fp ) == NULL )
    {
	if ( feof( fp ))
	{
	    fprintf(stderr,"%s - EOF encountered.\n", error_prefix);
	    status = false;
	}
	else /*  ferror( fp ) */
	{
	    fprintf(stderr, "%s - ", error_prefix);
	    fatalsys("error occurred while reading from file");
	}
    }

    return status;
}


/* 
 * Decode the exit status of a program as received by the parent process
 * by calling wait() or waitpid().
 *
 * Based on Program 8.3 (p. 199) in W. Richard Stevens,
 * "Advanced Programming in the UNIX Environment"
 * (Addison-Wesley, 1992).
 */
void
exit_status( int status )
{
    if (WIFEXITED( status ))
	printf("normal termination - exit status = %d\n",
	       WEXITSTATUS( status ));
    else if (WIFSIGNALED( status ))
    {
	printf("abnormal termination - signal number = %d",
	       WTERMSIG( status ));
#ifdef WCOREDUMP
	if (WCOREDUMP( status ))
	    printf(" (core dumped)");
#endif
	printf("\n");
    }
    else if (WIFSTOPPED( status ))
	printf("child stopped - signal number = %d\n",
	       WSTOPSIG( status ));
}


/*
 * Print an error message and exit.
 */
void fatal( char* msg )
{
    fprintf( stderr, "%s\n", msg );
    exit( EXIT_FAILURE );
}


/*
 * Print a system error message and exit.
 */
void fatalsys( char* msg )
{
    perror( msg );
    exit( EXIT_FAILURE );
}
