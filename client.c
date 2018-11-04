/*
   client.c
   The client implement an alpha-to-phone number request. Given a string of
   characters, the client would send them to the server ,and the server should return the telephone number equivalent of that string, based on
   the letters associated with each digit (except 1) on a telephone keypad. For example, the
   string ¡°1-888-SKY-DISH¡± should be translated to ¡°1-888-759-3474¡±.


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

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1024] , server_reply[1024]; //message is the string send to the server, server_reply is the string receive from the server

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");


    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons( atoi(argv[2]) );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("Connection failed. Error");
        return 1;
    }
    puts("Connected. \n");

    //Bind
	if( bind(sock,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
	    //print the error message
	    perror("bind failed. Error");
	    return 1;
    }



    //keep communicating with server
    while( (read(sock, server_reply, sizeof(server_reply)-1)) > 0)
    {
		printf("Enter the telephone number:");

        if (feof(stdin))
        {
		    fprintf(stdout, "Reached EOF.\n");
		    return 1;
		}
		else if (ferror(stdin))
		{
		    fprintf(stdout, "Error while reading the input.\n");
		    return 1;
       }
       fgets(message,1024-1,stdin);

        //Send the string data
        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Send failed");
            return 1;
        }

        //Receive a reply from the server
        if( recv(sock , server_reply , 1024 , 0) < 0)
        {
            puts("recv failed");
            break;
        }
        else if (recv(sock , server_reply , 1024 , 0) = 0)
        {
			close(sock);
		}

        puts("\n Server reply telephone number: ");
        puts(server_reply);
    }

    close(sock);
    return 0;
}