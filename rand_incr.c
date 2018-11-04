/*
   rand_incr

   Given 
     n -- the number of integers in a binary file
     m -- the number of times an integer is randomly selected and incremented
     filename -- the name of the binary file of integers to be created or
                 updated
   this program repeatedly (m times) selects one integer of the n integers
   in a file (indexed from 0 to n-1), reads it, increments it, and writes it
   back to the file.

   This program can be used to illustrate the Birthday Paradox as follows:

     rand_incr 365 50 birthdays.dat

   J. Mohr
   2015-01-21 (as a revision of a 2009 program)
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


const int NUM_INTS = 100;	/* the number of ints in the file */
const int COUNT = 100000;	/* how many times to increment ints in file */
const int PERMS = 0644;		/* file permissions */

/* Choose the data type of the file with the following two declarations.
   Be sure the format string to be used by 'printf' matches the data type. 
   (This is more general than the assignment specifies, and more general
   than the rest of the program below, the error messages, etc.)
*/

typedef int DATA_TYPE;
char FORMAT_STRING[] = "%5d:  %10d\n";
char SUM_FORMAT_STRING[] = "        %10d\n";

//typedef double DATA_TYPE;
//char FORMAT_STRING[] = "%5d:  %10.0f\n";
//char SUM_FORMAT_STRING[] = "        %10.0f\n";

void
fatalsys( const char* msg )
{
  perror( msg );
  exit( EXIT_FAILURE );
}


int
main( int argc, char * argv[] )
{
   int num_records;		/* how many integers are in the file */
   int count;			/* how many numbers to increment */
   int fd;			/* file descriptor */
   int retval;			/* system call return value */
   DATA_TYPE value;	        /* a single data value from the file */
   DATA_TYPE sum = 0;		/* sum of data values in the file */
   int i;			/* loop index variable */
   

   /* Check that a filename was given as an argument. */
   if ( argc != 4 )
   {
      fprintf( stderr, "Usage: %s n m filename\n"
	       "where n is the number of integers in a binary file,\n"
	       "      m is the number of times an integer is selected"
	       " and incremented\n"
	       "and filename is the name of the binary data file"
	       " of n integers\n", argv[0] );
      exit(1);
   }

   /* Get the number of records in the data file. */
   num_records = atoi( argv[1] );
   if ( num_records <= 0 )
       fatalsys("Number of integers in the file must be positive");

   /* Get the number of times an integer is to be selected and incremented. */
   count = atoi( argv[2] );
   if ( count <= 0 )
       fatalsys("Number of times an integer is to be selected and incremented"
		" must be positive.");

   /* Try to open the specified file; create it if it doesn't exist. */
   if ( (fd = open( argv[3], O_RDWR | O_CREAT, PERMS )) == -1 )
      fatalsys("Unable to open file for reading and writing");

   /* Initialize random number generator with process ID. */
   srand( getpid());

   /* Seek to a random integer position in the file and increment
      the integer at that position. Do this 'count' times. */
   for ( i = 0; i < count; i++ )
   {
      // off_t pos = (float) rand() / (float) RAND_MAX * NUM_INTS;
      // off_t pos = rand() / ((double) RAND_MAX + 1 ) * NUM_INTS;
      off_t pos = rand() % num_records;
	 
      if ( lseek( fd, pos * sizeof( DATA_TYPE ), SEEK_SET ) == -1 )
	 fatalsys("Seek failed");

      /* Set 'value' to default in case read/write pointer is
         positioned beyond the current end of the file,
         in which case 'read' will return a 0 without reading.
      */
      value = 0;
      if ( ( retval = read( fd, &value, sizeof( DATA_TYPE ))) == -1 )
	 fatalsys("Read failed");
      // The following lines explicitly check the return value.
      //else if ( retval == 0 )
      //  value = 1;
      //else
      value += 1;
	
      /* The read advanced the file pointer, so seek again. */
      if ( lseek( fd, pos * sizeof( DATA_TYPE ), SEEK_SET ) == -1 )
	 fatalsys("Seek failed");

      if ( write( fd, &value, sizeof( DATA_TYPE )) != sizeof( DATA_TYPE ))
	 fatalsys("Write failed");
   }

   /* Display the results, with a total of the integers in the file. */
   if ( lseek( fd, 0, SEEK_SET ) == -1 )
	 fatalsys("Seek to start of file failed");

   for ( i = 0; i < num_records; i++ )
   {
      if ( ( retval = read( fd, &value, sizeof( DATA_TYPE ))) ==  -1 )
	 fatalsys("Read failed");
      /* It is possible that not all possible positions were written. */
      else if ( retval == 0 )
      {
	 /* No value was read into 'value', so it will have whatever
	    value it had after the previous call to 'read'. */
	 value = 0;
      }

      if (value > 0)
	  printf( FORMAT_STRING, i, value );
      
      sum += value;
   }

   printf("        ----------\n");
   printf( SUM_FORMAT_STRING, sum );

   exit( EXIT_SUCCESS );
}
