#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <pthread.h>
#include <netdb.h>
#include "bounce.h"
#include <curses.h>

#define SERVER_PORT 6666
#define BUFFER_SIZE 1024

void display1(int, int, char*);

/**
*init client
*/
int initClient()
{
    struct sockaddr_in servaddr;

    //step1:get a socket
    int sock_cli = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_cli == -1)
	{
	    perror("Socket get failed!\n");
		exit(1);
	}
	display1(MSG_X, MSG_Y, "Socket created");
	//step2:connect to server
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVER_PORT);
//	servaddr.sin_addr.s_addr = inet_addr("144.167.58.4");
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    if(connect(sock_cli, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("server connect failed!\n");
	    exit(1);
   	}
	display1(MSG_X, MSG_Y, "connect success!...");
    return sock_cli;
}

/**
*receive message from server
*/
int receive(int sock_cli, struct sppbtp *mysppbtp)
{
    //receive info
    int needRecv = sizeof(struct sppbtp);
  	char *buffer = (char *)malloc(needRecv);
    int	pos = 0;
    int len = 0;
    while(pos < needRecv)
    {
        len = recv(sock_cli, buffer+pos, BUFFER_SIZE, 0);
        if(len < 0)
        {
            perror("Server receive data failed!\n");
            close(sock_cli);
            free(buffer);
			return 0;
        }
        pos+=len;
     }
     memcpy(mysppbtp, buffer, needRecv);
	 free(buffer);
	 display1(MSG_X, MSG_Y, "Playing...");
	 return 0;
}

/**
*send info to server
*/
int sendball(int sock_cli, struct sppbtp *mysppbtp)
{
    int needSend = sizeof(struct sppbtp);
	char *buffer = (char *)malloc(needSend);
	memcpy(buffer, mysppbtp, needSend);
	    
	int pos = 0;
	int len = 0;
    while(pos < needSend)
	{
	    len = send(sock_cli, buffer+pos, BUFFER_SIZE, 0);
		if(len <= 0)
    	{
		    perror("send to server failed!\n");
			close(sock_cli);
            free(buffer);
			return 0;
		}
		pos+=len;
	}
	free(buffer);
	display1(MSG_X, MSG_Y, "Waiting for server...");
	return 0;
}

/**
*close socket
*/
void close_socket(int sock_cli)
{
    close(sock_cli);
}

void display1(int x, int y, char* message)
{
//    move(y, x);
//	addstr("                              ");
	move(y, x);
    addstr(message);
} 
