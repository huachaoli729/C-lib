/*
   server.c
   The server  implement an alpha-to-phone number service. Given a string of
   characters, the server should return the telephone number equivalent of that string, based on
   the letters associated with each digit (except 1) on a telephone keypad. For example, the
   string ¡°1-888-SKY-DISH¡± should be translated to ¡°1-888-759-3474¡±.

   The server could deal with multiple clients.(This is not work at time file.)

   Adapted the code from: http://www.binarytides.com/server-client-example-c-sockets-linux/


   Huachao Li
   Programing assignment 4, AUCSC380, 2015-04-02
 */

#include <sys/types.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

//Adapted the code from: http://www.thegeekstuff.com/2011/12/c-socket-programming/


#include <sys/types.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[1024];

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    int portno = atoi(argv[1]);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( portno );

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }

    //Listen
    if (listen(socket_desc , 3)== -1)
    {
	    perror("Listening Failure\n");
        exit(1);
	}

    //Accept and incoming connection
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if ( (fork()) < 0 )	/* error occurred */
    {
		perror( "fork() failed to create child 1");
     }

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");

    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , 1024 , 0)) > 0 )
    {
	    printf("The client %d sent telephone number: %s", getpid(), client_message);
        //Transfer the string
         int i;
        for(i = 0; client_message[i] != 0;i++)
        {
            switch(client_message[i])
            {
				//A,B,C,a,b,c transfer into 2
				case 'A':
				client_message[i] = '2';
				break;
				case 'B':
				client_message[i] = '2';
				break;
				case 'C':
				client_message[i] = '2';
				break;
				case 'a':
				client_message[i] = '2';
				break;
				case 'b':
				client_message[i] = '2';
				break;
				case 'c':
				client_message[i] = '2';
				break;
				//D,E,Fd,e,f transfer into 3
				case 'D':
				client_message[i] = '3';
				break;
				case 'E':
				client_message[i] = '3';
				break;
				case 'F':
				client_message[i] = '3';
				break;
				case 'd':
				client_message[i] = '3';
				break;
				case 'e':
				client_message[i] = '3';
				break;
				case 'f':
				client_message[i] = '3';
				break;
				//G,H,I,g,h,i transfer into 4
				case 'G':
				client_message[i] = '4';
				break;
				case 'H':
				client_message[i] = '4';
				break;
				case 'I':
				client_message[i] = '4';
				break;
				case 'g':
				client_message[i] = '4';
				break;
				case 'h':
				client_message[i] = '4';
				break;
				case 'i':
				client_message[i] = '4';
				break;
				//J,K,L,j,k,l transfer into 5
				case 'J':
				client_message[i] = '5';
				break;
				case 'K':
				client_message[i] = '5';
				break;
				case 'L':
				client_message[i] = '5';
				break;
				case 'j':
				client_message[i] = '5';
				break;
				case 'k':
				client_message[i] = '5';
				break;
				case 'l':
				client_message[i] = '6';
				break;
				//M,N,O,m,n,o transfer into 6;
				case 'M':
				client_message[i] = '6';
				break;
				case 'N':
				client_message[i] = '6';
				break;
				case 'O':
				client_message[i] = '6';
				break;
				case 'm':
				client_message[i] = '6';
				break;
				case 'n':
				client_message[i] = '6';
				break;
				case 'o':
				client_message[i] = '6';
				break;
				//P,Q,R,S,p,q,r,s transfer into 7
				case 'P':
				client_message[i] = '7';
				break;
				case 'Q':
				client_message[i] = '7';
				break;
				case 'R':
				client_message[i] = '7';
				break;
				case 'S':
				client_message[i] = '7';
				break;
				case 'p':
				client_message[i] = '7';
				break;
				case 'q':
				client_message[i] = '7';
				break;
				case 'r':
				client_message[i] = '7';
				break;
				case 's':
				client_message[i] = '7';
				break;
				//T,U,V,t,u,v transfer into 8
				case 'T':
				client_message[i] = '8';
				break;
				case 'U':
				client_message[i] = '8';
				break;
				case 'V':
				client_message[i] = '8';
				break;
				case 't':
				client_message[i] = '8';
				break;
				case 'u':
				client_message[i] = '8';
				break;
				case 'v':
				client_message[i] = '8';
				break;
				//W,X,Y,Z,w,x,y,z transfer into 9
				case 'W':
				client_message[i] = '9';
				break;
				case 'X':
				client_message[i] = '9';
				break;
				case 'Y':
				client_message[i] = '9';
				break;
				case 'Z':
				client_message[i] = '9';
				break;
				case 'w':
				client_message[i] = '9';
				break;
				case 'x':
				client_message[i] = '9';
				break;
				case 'y':
				client_message[i] = '9';
				break;
				case 'z':
				client_message[i] = '9';
				break;
				default: //Non-letter value remains
				break;
			}//End swith
	    }//End for
             client_message[read_size] = '\0';
	    printf("Server sent transfered telephone number to client %d: %s \n", getpid(), client_message);
	    //Send the message back to client
           write(client_sock , client_message , strlen(client_message));
           fflush(stdout);
    }

    if(read_size == 0)
    {
        printf("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }

    return 0;
}