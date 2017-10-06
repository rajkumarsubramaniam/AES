/**
 * @file profiler.c
 * @brief This file implements the pthread_create, fork, exec to profile the time taken
 * @author Raj Kumar Subramaniam
 * @date October 5,2017
 **/

#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>	//Added for waitpid()


#define NSEC 1000000000	//SEC to Nsec value


/*Time Spec Structures*/
struct timespec startTime;
struct timespec endTime;

/*Function to get the time difference*/
uint64_t timeDifference(struct timespec start, struct timespec end)
{
	uint32_t xSec = start.tv_sec;
	uint32_t xnSec = start.tv_nsec;
	uint32_t ySec = end.tv_sec;
	uint32_t ynSec = end.tv_nsec;
	uint64_t time = 0;
	time = ((ySec * NSEC) + (ynSec)) - ((xSec * NSEC) + (xnSec)); /*calculating time differnece*/
	return time;
}

/*Function to execute pthread create*/
void* startFunction(void * msg)
{
	printf("The pthread_create function\n");
	pthread_exit(NULL);
}

/*Main Function*/
int main()
{
	pid_t processId;
	pthread_t pid;
	pthread_attr_t attr; 
	uint16_t error;
	int32_t status;
	
	/*Gets the start Time*/
	if(clock_gettime(CLOCK_THREAD_CPUTIME_ID,&startTime))	/*Gives the time from the process start*/
		printf("clok_gettime_error\n");
		
    error = pthread_create(&pid, NULL, &startFunction, (void*)NULL);	/*Child thread creation*/
    
    /*Gets the end Time*/
    if(clock_gettime(CLOCK_THREAD_CPUTIME_ID,&endTime))
		printf("clok_gettime_error\n");
		
    if(error != 0)
    {
        printf("Failed in pthread_create\n");
        return -1;
    }
    
    printf("Time for pthread_create is %lu ns\n", timeDifference(startTime, endTime));	/*Time for Pthared_create()*/
    
    /*Gets the start Time*/
	if(clock_gettime(CLOCK_THREAD_CPUTIME_ID,&startTime))
		printf("clok_gettime_error\n");
	/*Forking the process*/	
	processId = fork();
		
	if(processId < 0)
		printf("Fork failed with error:%d\n",processId);
	else if(processId == 0)
	{
		static char *argv[]={"myExecFork", "0", NULL};
		execv("exit", argv);     
		exit(0);	/*if execv fails - the code will exit*/
	}
	else
	{	
		if(waitpid(processId, &status, 0) == -1)	/*waiting for child to finish*/
			printf("Error Calling wait\n");
			
		/*Gets the end Time*/
    	if(clock_gettime(CLOCK_THREAD_CPUTIME_ID,&endTime))
			printf("clok_gettime_error\n");
		
		printf("Time for fork and exec is %lu ns\n", timeDifference(startTime, endTime));	/*Time for Fork and Exec*/
		printf("This is the parent\n");
	}
	printf("This is the end Fork Returned:%d\n",processId);
	return 0;
}
