#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "bounce.h"
#include <curses.h>

#define SERVER_PORT 6666
#define LENGTH_OF_LISTEN_QUEUE 20
#define BUFFER_SIZE 1024

void release(char *);
void display1(int, int, char*);
int initServer()
{
    struct sockaddr_in server_addr;//socket address
    int new_server_socket;
	//step1: ask kernal for a socket
    int server_socket = socket(PF_INET, SOCK_STREAM, 0);
		if(server_socket == -1)
		{
		    perror("Creat socket failed!\n");
			exit(1);
		}
//    display1(MSG_X, MSG_Y, "Socket created");
    struct timeval timeout;      
    timeout.tv_sec = 60;
    timeout.tv_usec = 0;
    if (setsockopt (server_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
    {
	    perror("setsockopt failed\n");
		exit(1);
    }
    if (setsockopt (server_socket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,sizeof(timeout)) < 0)
    {
	    perror("setsockopt failed\n");
		exit(1);
	}

    //step2:bind address to socket. host & port
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
														
	//bind
	if(bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0)
	{
	    printf("Server bind port: %d failed\n", SERVER_PORT);
		exit(1);
	}
//	display1(MSG_X, MSG_Y, "Bind success!");

	//step3:allow incoming calls with Qsize=1 on socket
	if(listen(server_socket, LENGTH_OF_LISTEN_QUEUE))
	{
	    perror("Server listen failed\n");
		exit(1);
	}
//    display1(MSG_X, MSG_Y, "waiting for incoming connections...");
	//accept
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    new_server_socket = accept(server_socket, (struct sockaddr*)&client_addr, &length);
    if(new_server_socket < 0)
    {
        perror("Server accept failed!\n");
        exit(1);
    }
//	display1(MSG_X, MSG_Y, "client connected,waiting for client...");
    return new_server_socket;
}
/**
*receive message from client
*/
int receive(int new_server_socket, struct sppbtp *mysppbtp)
{
	int needRecv = sizeof(struct sppbtp);
	char * buffer = (char*)malloc(needRecv);
	int pos = 0;
	int len = 0;
	while(pos < needRecv)
	{
		len = recv(new_server_socket, buffer+pos, BUFFER_SIZE, 0);
		if(len < 0)
		{
		    perror("Server receive data failed!\n");
            close(new_server_socket);
			release(buffer);
			return 0;
		}else if(len == 0)//client stop connect
		{
		    release(buffer);
//            display1(MSG_X, MSG_Y, "connect lost, wait for client...");
			fflush(stdout);
			return 0;
		}
		pos+=len;
	}
   	memcpy(mysppbtp, buffer, needRecv);
    release(buffer);
//	display1(MSG_X, MSG_Y, "Playing...");
    return 0;
}
/**
*send message to client
*/
int sendball(int new_server_socket, struct sppbtp *mysppbtp)
{
    int needSend = sizeof(struct sppbtp);
	char * buffer = (char*)malloc(needSend);
	int pos = 0;
	int len = 0;
    memcpy(buffer, mysppbtp, needSend);

    while(pos < needSend)
    {
        len = send(new_server_socket, buffer+pos, BUFFER_SIZE, 0);
        if(len <= 0)
        {
            perror("send to client failed!\n");
			close(new_server_socket);
			release(buffer);
            return 0;
        }
        pos+=len;
    }
    release(buffer);
//	display1(MSG_X, MSG_Y, "Waiting for client");
	return 0;
}

/**
*release resource
*/
void release(char *buffer)
{
    free(buffer);
}

/**
*close socket
*/
void close_socket(int server_socket)
{
    close(server_socket);
}

void display1(int x, int y, char* message)
{
//    move(y, x);
//	addstr("                              ");
	move(y, x);
    addstr(message);
} 
