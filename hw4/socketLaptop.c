/****************************************************************
* file	: socketApp.c
* brief : This is the user application to use sockets to send and receive
* receive data over the IPv4 network to communicate with BBG.
* description: This application run on the laptop connected to BBG and
* send commands from laptop to control the LED using my driver.
* author: Raj Kumar Subramaniam
* date	: 11/18/2017
*****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORTNO 1000
#define BBG_ADDRESS "192.168.7.2"
#define BUFF_LENGTH 50

int main(void)
{
	socklen_t addrLen;
	int32_t socketfd, newSocketfd;
	struct sockaddr_in  remoteAddr;/*Remote peer address*/
	size_t addrlen = sizeof(remoteAddr);
	uint8_t receiveBuff[BUFF_LENGTH];
	uint8_t sendBuff[BUFF_LENGTH];
	
	
	/*Variables for Commands to LED*/
	uint32_t period = 0;
	uint32_t onTime = 0;
	uint32_t tStatus;
	
	/*Initialize to 0 to avoid errors*/
	memset(&remoteAddr, '0', sizeof(remoteAddr));
	/*Creating the socket*/
	socketfd = socket(AF_INET,SOCK_STREAM, 0);
	if(socketfd == -1)
	{
		printf("Socket Creation Failed:%d\n",errno);
		return -1;
	}

	/*Connecing to the Beagle Bone*/
	remoteAddr.sin_family = AF_INET;
	remoteAddr.sin_port = htons(PORTNO);
	if(inet_pton(AF_INET,BBG_ADDRESS, &remoteAddr.sin_addr)<=0) 
    {
        printf("Local Address Invalid\n");
        return -1;
    }
   
   	if( connect(socketfd, (struct sockaddr *)&remoteAddr, (socklen_t)addrlen) == -1)
	{
		printf("Socket Connection Failed:%d\n",errno);
		return -1;
	}

	printf("Socket Connection Successful\n");
	
	printf("Enter Period(ms), On time(ms), Toggle Status(0/1) to send:\n");
	scanf("%u",&period);
	scanf("%u",&onTime);
	scanf("%u",&tStatus);
	//freq = freq/4;
	//onTime = onTime/4;
	printf("printf:%u %u %u\n", onTime, period, tStatus);
	sprintf(sendBuff,"%d %d %d", period, onTime, tStatus);		/*Creating the buffer to send data*/
	
	printf("Data to be sent: %s\n",sendBuff);
	/*Sending to BBG*/
	if(send(socketfd,sendBuff,strlen(sendBuff),0) == -1)
	{
		printf("Write Failed: %d\n",errno);
	}
	printf("Message Sent to BBG\n");
	
	/*Receiving the status back from BBG*/
	if(read(socketfd,receiveBuff,BUFF_LENGTH)<0)
	{
		printf("Read Failed\n");
	}
	printf("LED status Received from BBG as: %s\n", receiveBuff);
	
	return 0;
}
