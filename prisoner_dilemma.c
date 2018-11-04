/*
 *  prisoner_dilemma.c  -  Interprocess Communication with Pipes
 *
 *  This program is using the UNIX pipe and process control system calls (fork, exec,
 *  pipe, wait, exit, and possibly dup2) to create two child processes, each connected to the
 *  parent by two pipes, and to simulate the Prisoner¡¯s Dilemma (PD) game, where the parent
 *  is the jailer and the two child processes are each prisoners.
 *
 *  Naturally, there are different rewards or punishments (payoffs) for cooperating or
 *  defecting. In the simple, symmetrical version of PD, there are four levels of payoffs:
 *
 *  R The reward payoff received if both prisoners cooperate with each other (i.e.,
 *  remain silent). For example, both prisoners might receive short jail sentences on a
 *  lesser charge because authorities don¡¯t have enough evidence to convict them of
 *  the more serious offense.
 *
 *  P The punishment both receive if both defect (i.e., confess and implicate each
 *  other). For example, both might receive longer jail sentences due to confessing to
 *  the serious crime.
 *
 *  T The temptation payoff received by the single prisoner who defects. For example,
 *  the defecting criminal might get early parole in return for testifying against his/her
 *  partner in crime.'
 *
 *  S The sucker payoff received by the prisoner whose partner defects. For example,
 *  the prisoner might receive the maximum possible jail term for the offense because
 *  his/her partner testified against him/her; the prisoner remained silent, and thus was
 *  not able to get any reduction in the sentence by plea bargaining.

 *  The Tit-for-tat strategy is that ,what you rival choose last time, you choose the same: when you rival choose
 *  defect last time, you choose defect as well; when your rival choose cooperate last time ,you choose cooperate as well.
 *  First choice is cooperate.
 *
 *  The Agressive strategy is always chooce the agressive one to achieve more. Here is always
 *  choosing the defect one all the times, even the first time.
 *
 *  In the usual formulation of the game,
 *  T > R > P > S
 *  and
 *  R > (T + S) / 2 > P.
 *  In his famous "contest" between IPD strategies, Axelrod used the values
 *       T = 5
 *       R = 3
 *       P = 1
 *       S = 0
 *
 *                                                                  Prisoner1
 *                                                       Cooperate          Defect
 *      Prisoner1             Cooperate         (3, 3)                (0, 5)
 *                                  Defect              (5, 0)                (1, 1)
 *
 *
 *
 *  As a matter pf fact, in all the Prisoner's Dilemma, THE agressive one always wins.
 *
 *

 *  @Huachao Li, adopt patial codes from nonblock.c on eclass.
 *  AUCSC 380 Assignment3
 */

#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>

/* Function declarations. */
void parent( int p[2], int q[2], int r[2], int s[2] );
void childT( int w[2], int r[2] ); /* Tit-for-tat strategy for prisoner*/
void childA( int w[2], int r[2] ); /* Agressive strategy for prisoner*/
void fatalsys( char *str );


int
main( void )
{
	int pfd1[2];
	int pfd2[2];
	int flags1;
	int flags2;

	/* open pipe 1 for child 1*/
	if ( pipe(pfd1) < 0 )
		fatalsys("pipe call");

	/* set O_NONBLOCK flag for pfd1[0] */
	/* first get current flags */
	if ( (flags1 = fcntl(pfd1[0], F_GETFL )) < 0 )
		fatalsys("fcntl get flags call");
	/* add the nonblocking flag (using bitwise OR) */
	flags1 |= O_NONBLOCK;
	/* set the flags */
	if ( fcntl(pfd1[0], F_SETFL, flags1 ) < 0 )
		fatalsys("fcntl set flags call");

	/* open pipe 2 for child 1*/
	if ( pipe(pfd2) < 0 )
		fatalsys("pipe call");

	/* set O_NONBLOCK flag for pfd2[0] */
	/* first get current flags */
	if ( (flags2 = fcntl(pfd2[0], F_GETFL )) < 0 )
		fatalsys("fcntl get flags call");
	/* add the nonblocking flag (using bitwise OR) */
	flags2 |= O_NONBLOCK;
	/* set the flags */
	if ( fcntl(pfd2[0], F_SETFL, flags2 ) < 0 )
		fatalsys("fcntl set flags call");

	/* Create child 1 process */
	if ( ( pid1 = fork()) < 0 )	/* error occurred */
	    fatalsys( "fork() failed to create child 1");
	else if ( pid1 == 0 )		/* child process */
	    childT(pfd1, pfd2); /* You can reset the strategy to agressive: childA() */

	int pfd3[2];
	int pfd4[2];
	int flags3;
	int flags4;

	/* open pipe 3 for child 2*/
	if ( pipe(pfd3) < 0 )
		fatalsys("pipe call");

	/* set O_NONBLOCK flag for pfd3[0] */
	/* first get current flags */
	if ( (flags3 = fcntl(pfd3[0], F_GETFL )) < 0 )
		fatalsys("fcntl get flags call");
	/* add the nonblocking flag (using bitwise OR) */
	flags3 |= O_NONBLOCK;
	/* set the flags */
	if ( fcntl(pfd3[0], F_SETFL, flags3 ) < 0 )
		fatalsys("fcntl set flags call");

	/* open pipe 4 for child 2*/
	if ( pipe(pfd4) < 0 )
		fatalsys("pipe call");

	/* set O_NONBLOCK flag for pfd4[0] */
	/* first get current flags */
	if ( (flags4 = fcntl(pfd4[0], F_GETFL )) < 0 )
		fatalsys("fcntl get flags call");
	/* add the nonblocking flag (using bitwise OR) */
	flags4 |= O_NONBLOCK;
	/* set the flags */
	if ( fcntl(pfd4[0], F_SETFL, flags4 ) < 0 )
		fatalsys("fcntl set flags call");

	int pid2;
	/* Create child 2 process */
	if ( ( pid2 = fork()) < 0 )	/* error occurred */
	    fatalsys( "fork() failed to create child 2");
	else if ( pid2 == 0 )		/* child process */
        childT(pfd3, pfd4); /* You can reset the strategy to agressive: childA() */

	/* Time for parent process */
	parent(pfd1, pfd2, pfd3, pfd4);
}

/*
 *  The parent process read the value which both child prsioners process wrote, and then judge the score ,
 *  write the score value to each prosoner. It teminates when all the round are done.
 *
 */
void
parent( int p[2], int q[2], int r[2], int s[2] )		/* code for parent */
{
	int nread;
	int num1; /* Number read from and sent to child 1*/
	int num2; /* Number read from and sent to child 2*/

	close( p[1] ); /* p pipe for read from child 1*/
	close( q[0] ); /* q pipe for write to child 1*/
	close( r[1] ); /* r pipe for read from child 2*/
	close( s[0] ); /* s pipe for write to child 2 */

	int counter = 0;
	int count;
	int times = 30; /* Fixed value, Number of round which is about to take between the two prisoners. */
	int prisoner1 = 0;
	int prisoner2 = 0;
	int t = 5;
    int r = 3;
    int p = 1;
    int s = 0;

	for (count = 0; count < times; count++ )
	{
		switch ( nread = read( p[0], &num, sizeof(int) ))
		{
		    case -1:	/* pipe empty */
			printf("< pipe empty >\n");
			sleep(1);
			break;
		    case  0:	/* end of file */
			exit( EXIT_FAILURE );
		    default:	/* integer read */
			read( p[0], &num1, sizeof(int) ); /*Read the integer from child 1 */
			read( r[0], &num2, sizeof(int) ); /*Read the integer from child 2 */
			counter++;
			if ((num1 = 1)&&(num2 = 1)) /* P/P Both prisoner1 and prisoner1 defect*/
			{
				prisoner1 = prisoner1 + p;
				prisoner2 = prisoner2 + p;
				num1 =p;
				num2 =p;
				write(q[1], &num1, sizeof(int) );
				write(s[1], &num2, sizeof(int) );
			}
			else if ((num1 = 0)&&(num2 = 0)) /* R/R, Both prisoner1 & prisoner2 cooperate*/
			{
				prisoner1 = prisoner1 + r;
				prisoner2 = prisoner2 + r;
				num1 = r;
				num2 = r;
				write(q[1], &num1, sizeof(int) );
				write(s[1], &num2, sizeof(int) );
			}
			else if ((num1 = 0)&&(num2 = 1)) /* S/T, prisoner1 cooperate, prisoner2 defect*/
			{
				prisoner1 = prisoner1 + s;
				prisoner2 = prisoner2 + t;
				num1 = s;
				num2 = t;
				write(q[1], &num1, sizeof(int) );
				write(s[1], &num2, sizeof(int) );
		    }
		    else /*T/S,  prisoner1 defect, prisoner2 cooperate*/
		    {
				prisoner1 = prisoner1 + t;
				prisoner2 = prisoner2 + s;
				num1 = t;
				num2 = s;
				write(q[1], &num1, sizeof(int) );
				write(s[1], &num2, sizeof(int) );
			}


		}

		if(count = times -1) /* Last round has end, close all the pipes*/
		{
				close( p[0] );
				close( q[1] );
				close( r[0] );
				close( s[1] );
				printf("The first prisoner earns  %d\n dollars", prisoner1);
				printf("The second prisoner earns  %d\n dollars", prisoner2);
				exit( EXIT_SUCCESS );
		}
	}
}

/*
 *  The Tit-for-tat child(prisoner) strategy is that: what the rival choose last time, this child choose the same: when the rival choose
 *  defect last time, this child choose defect as well; when the rival choose cooperate last time,this child choose cooperate as well.
 *  First choice is cooperate.
 */
void
childT( int w[2], int r[2] )		/* code for child */
{
	close( w[0] ); /* p pipe for write to parent */
	close( r[1] ); /* p pipe for read from parent */

	int decision = 0; /* First decision is cooperate */
	int num;
    int nread;
    int count = 0;
	for ( ; ; )
	{
		switch ( nread = read( p[0], &num, sizeof(int) ))
		{
				    case -1:	/* pipe empty, first time for making the decision */
					num = decision; /* mantance the cooperate decision */
					write( w[1], &num, sizeof(int) ); /* Write to the parent through pipe */
			        count++;
				    case  0:	/* pipe is closed */
					exit( EXIT_SUCCESS ); /* Child is teminated */
		            default: /*Decison making for child process */
		            if  (num =3) /* If both cooperate*/
		            {
						num = decision;
					    write( w[1], &num, sizeof(int) ); /* Write to the parent through pipe */
				    }
				    else if (num =1)
		            {
						num = decision;
					    write( w[1], &num, sizeof(int) );
					}
					else if (num = 5)
		            {
						num = decision;
					    write( w[1], &num, sizeof(int) );
				    }
				    else if (decision > 0)
				    {
						num = decision;
						write( w[1], &num, sizeof(int) );
				    }
				    else
				    {
						choice = 1;
						num = decision;
						write( w[1], &num, sizeof(int) );
					}
		}
	}
}

/*
 *   The Agressive child (prisoner) strategy is always to chooce the agressive one to achieve no less than the other one. Here is always
 *   choosing the defect one all the times, even the first time.
 */
void
childA( int w[2], int r[2] )		/* code for child */
{
	close( w[0] ); /* p pipe for write to parent */
	close( r[1] ); /* p pipe for read from parent */

	int decision = 1; /* First decision is defect */
	int num;
    int nread;
	for ( ; ; )
	{
		switch ( nread = read( p[0], &num, sizeof(int) ))
		{
				    case -1:	/* pipe empty, first time for making the decision */
					num = decision; /* mantance the cooperate decision */
					write( w[1], &num, sizeof(int) ); /* Write to the parent through pipe */
				    case  0:	/* pipe is closed */
					exit( EXIT_SUCCESS ); /* Child is teminated */
		            default: /*Decison making for child process */
		            num = decision;
		            write( w[1], &num, sizeof(int) ); /* Write to the parent through pipe */
		}
	}
}

/*
 * Print a system error message and exit.
 */
void
fatalsys( char *errmsg )		/* print error message and die */
{
	perror( errmsg );
	exit( EXIT_FAILURE );
}
