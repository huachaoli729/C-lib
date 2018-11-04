/* O_NONBLOCK example */
/* Based on Haviland & Salama, p. 151 */

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

const int MSGSIZE = 6;

char *msg1 = "hello";
char *msg2 = "bye!";

void parent( int p[2] );
void child( int p[2] );
void fatalsys( char *str );


int
main( void )
{
	int pfd[2];
	int flags;

	/* open pipe */
	if ( pipe(pfd) < 0 )
		fatalsys("pipe call");

	/* set O_NONBLOCK flag for p[0] */
	/* first get current flags */
	if ( (flags = fcntl(pfd[0], F_GETFL )) < 0 )
		fatalsys("fcntl get flags call");
	/* add the nonblocking flag (using bitwise OR) */
	flags |= O_NONBLOCK;
	/* set the flags */
	if ( fcntl(pfd[0], F_SETFL, flags ) < 0 )
		fatalsys("fcntl set flags call");

	switch ( fork()) {
	    case -1:	/* error */
		fatalsys("fork call");
	    case  0:	/* child */
		child( pfd );
	    default:	/* parent */
		parent( pfd );
	}
}


void
parent( int p[2] )		/* code for parent */
{
	int nread;
	char buf[MSGSIZE];

	close( p[1] );

	for (;;) {
		switch ( nread = read( p[0], buf, MSGSIZE )) {
		    case -1:	/* pipe empty */
			printf("< pipe empty >\n");
			sleep(1);
			break;
		    case  0:	/* end of file */
			printf("< end of file >\n");
			exit( EXIT_FAILURE );
		    default:	/* characters read */
			printf("message = %s\n", buf );
			if ( strcmp( buf, msg2 ) == 0 ) {
				printf("End of conversation.\n");
				exit( EXIT_SUCCESS );
			}
		}
	}
}


void
child( int p[2] )		/* code for child */
{
	int count;

	close( p[0] );

	for ( count = 0; count < 3; count++ ) {
		write( p[1], msg1, MSGSIZE );
		sleep( 3 );
	}

	/* send final message */
	/* If you comment out the next line, the parent will
	   report end-of-file ('read' will return 0 bytes). */
	write( p[1], msg2, MSGSIZE );
	exit( EXIT_SUCCESS );
}


void
fatalsys( char *errmsg )		/* print error message and die */
{
	perror( errmsg );
	exit( EXIT_FAILURE );
}
