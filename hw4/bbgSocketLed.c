/****************************************************************
* file	: bbgSocketLed.c
* brief : This is the user application to control the Led on Beagle 
* Bone Green using my own led driver and communicating over the socket 
* to a laptop.
* author: Raj Kumar Subramaniam
* date	: 11/18/2017
*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

/*Libraries for Socket*/
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

/*Macros for LED Control*/
#define LED_CONTROL "/dev/myLedDriver"
#define FREQUENCY 250
#define DUTYCYCLE 50
#define LEDSTATE 1

/*Macros for Socket Communication*/
#define PORTNO 1000
#define BBG_ADDRESS "192.168.7.2"
#define RECEIVE_BUFF_LENGTH 50

		


int32_t main(void)
{
	/*LED Controlling Variables*/
	int32_t ledFileDes;
	int16_t retVal;
	uint8_t received[100];
	
	/*Socket Variables*/
	struct sockaddr_in  localAddr; /*Socket Local Address*/
	struct sockaddr_in  remoteAddr;/*Remote peer address*/
	socklen_t addrLen;
	int32_t socketfd, newSocketfd;
	size_t addrlen = sizeof(remoteAddr);
	uint8_t receiveBuff[RECEIVE_BUFF_LENGTH];
	uint8_t sendBuff[RECEIVE_BUFF_LENGTH];
	memset(&localAddr, 0, sizeof(localAddr));	/*Initialize to 0 to avoid errors*/
	socketfd = socket(AF_INET,SOCK_STREAM, 0);	/*Creating the socket*/
	if(socketfd == -1)
	{
		printf("Socket Creation Failed:%d\n",errno);
		return -1;
	}
	
	/*Binding the address to the socket*/
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(PORTNO);
	if(inet_pton(AF_INET,BBG_ADDRESS, &localAddr.sin_addr)<=0) 
    {
        printf("Local Address Invalid\n");
        return -1;
    }
    if (bind(socketfd, (struct sockaddr *)&localAddr, sizeof(localAddr))<0)
    {
       printf("Bind Failed\n");
       return -1;
    }
    
    /*Listening to connections over the socket*/
    if (listen(socketfd, 5) < 0)
    {
        printf("Listen Failed\n");
       return -1;
    }
    
	printf("Socket Established, Starting Application\n");
	
	ledFileDes = open(LED_CONTROL,O_RDWR);
	if(ledFileDes < 0)
	{
		printf("Failed to open the LED driver\n");
		return -1;
	}

	/*Accepting Socket Connection*/
	newSocketfd = accept(socketfd, (struct sockaddr *)&remoteAddr, (socklen_t*)&addrlen);
   	if(newSocketfd == -1)
	{
		printf("Socket Connection Failed:%d\n",errno);
		return -1;
	}
	printf("A connection was accepted\n");
	
	/*Receiving data over the socket*/
	if(read(newSocketfd,receiveBuff,RECEIVE_BUFF_LENGTH)<0)
	{
		printf("Read Failed\n");
	}
	printf("Received from User: %s\n", receiveBuff);
	
	printf("stringlen:%ld\n",strlen(receiveBuff));
	
	
	
	/*Appending Read Cmd*/
	sprintf(sendBuff,"%d %s", 0x00,receiveBuff);
	
	printf("Appended string:  %s\n", sendBuff);
	
	/*Sending the data received over the Socket to control the led*/
	retVal = write(ledFileDes, sendBuff, strlen(sendBuff));
	if(retVal < 0)
	{
		printf("Failed to send data to the LED driver\n");
	}
	
	
	/*Read the state of the LED*/
	retVal = read(ledFileDes,received,strlen(receiveBuff));
	if(retVal < 0)
	{
		printf("Failed to receive data from the LED driver\n");
	}
	printf("Frequency, Dutycycle, Status are %s respectively\n",received);
	
	/*Send the current status of LED over the socket*/
	if(send(newSocketfd,received,strlen(received),0)<0)
	{
		printf("Write Failed\n");
	}
	printf("Message Sent to User\n");
	
	
	
	/*Reading single data - example*/
	/*Appending Read Cmd*/
	sprintf(sendBuff,"%d %s", 0x10,received);
	/*Sending the data received over the Socket to control the led*/
	retVal = write(ledFileDes, sendBuff, strlen(sendBuff));

	if(retVal < 0)
	{
		printf("Failed to send data to the LED driver\n");
	}
	/*Read the state of the LED*/
	retVal = read(ledFileDes,received,strlen(received));
	if(retVal < 0)
	{
		printf("Failed to receive data from the LED driver\n");
	}
	printf("Queried Single Data %s\n",received);
	
	/*Closing the drive file descriptor*/
	retVal = close(ledFileDes);
	if(retVal != 0)
	{
		printf("Failed to close the LED driver\n");
	}
	printf("Finished LED Application\n");
	
	return 0;	
}
