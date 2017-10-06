/**
 * @file 5fileHandling.c
 * @brief This file implements the Thread 1, Thread2 and Thread 3
 * @author Raj Kumar Subramaniam
 * @date October 3,2017
 **/
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <semaphore.h>

/*Structure to load statistics*/
typedef struct{
    size_t words;
    size_t lines;
    size_t characters;
}statistics;

uint8_t filename[20];	/*File name*/

statistics values = {0, 0, 0};

struct sigaction sigInit;	/*Structure for Signal handler*/

sigset_t maskSet;

FILE *usrFile = NULL;	/*File Handle*/

sem_t signalThread2;            /*semaphore to siganl thread2*/
sem_t signalThread3;            /*semaphore to siganl thread2*/
pthread_mutex_t lockStat;       /*Mutex lock of the variable*/

/*User Defined Signal Handler*/
void sig_handler(int signum)
{
    printf("\nReceived signal %d\n", signum);
    if(signum == SIGUSR1)       /*Handling SIGUSR1*/
    {
        printf("Start Processing\n");
        sem_post(&signalThread2);
   
    }
    else if(signum == SIGUSR2)  /*Handling SIGUSR2*/
    {
        printf("Start Printing Statistics\n");
        sem_post(&signalThread3);
    }
    else if(signum == SIGINT)	/*For graceful exit when parent is killed*/
    {
    	printf("Received SIGINT");
    	pthread_mutex_destroy(&lockStat);
    	if(usrFile != NULL)
			fclose(usrFile);
    	exit(EXIT_SUCCESS);
    }
}

/*This function will be executed on a separate thread for calculating the statistics*/
void* statCalc(void* data)
{
    int out;
    size_t characters;
//  int32_t sigNum;
//  sigemptyset(&maskSet);
//  sigaddset(&maskSet, SIGUSR1);
//  pthread_sigmask(SIG_BLOCK, &maskSet, NULL);
//  sigwait(&maskSet, &sigNum);
//  while (! sig_not_occurred & 0x01);
    sem_wait(&signalThread2);			/*waiting for Sem - given by SIGUSR1*/
    usrFile = fopen(filename,"a+");   /*Using a+ will not clear the existing data*/
    if(usrFile == 0)
    {
        return NULL;
    }
    /*Read from the file until the EOF is reached*/
    while( (out = fgetc(usrFile) ) != EOF)
    {
        pthread_mutex_lock(&lockStat);	/*Lock and write data*/
        values.characters++;
        if(' ' == out)
        {
            values.words++;
        }
        else if('\n' == out)
        {
            values.lines++;
            values.words++;
        }
        pthread_mutex_unlock(&lockStat);	/*Unlock Mutex*/
    } 
    printf("\n");
    printf("Success\n");
    fclose(usrFile);
    usrFile = NULL;	/*Used to help in graceful exit in signal Handler*/
    pthread_exit(NULL);
}

/*This function will be executed on a separate thread for calculating the statistics*/
void* statPrint(void* data)
{
//    int32_t sigNum;
//    sigset_t mask;
//    sigemptyset(&mask);
//    sigaddset(&mask, SIGUSR2);
//    pthread_sigmask(SIG_BLOCK, &mask, NULL);
//    sigwait(&mask, &sigNum);
    sem_wait(&signalThread3);		/*waiting for Sem - given by SIGUSR2*/
    printf("Printing Statistics\n");
    pthread_mutex_lock(&lockStat);	/*Lock and read data*/
		printf("Words:%ld\n",values.words);
		printf("Lines:%ld\n",values.lines);
		printf("Characters:%ld\n",values.characters);
    pthread_mutex_unlock(&lockStat);	/*Unlock Mutex*/
    pthread_exit(NULL);
}


/*Main function for the code*/
int main(int argc,char *argv[])
{
    size_t i;
    int out;
    uint16_t error,ret;
    pthread_t newId, printId;
    pthread_attr_t newAttr; /*Attributes for new thread*/
    /*get the filename from argv*/
    if((argv+1) != NULL)
    {
        
        for(int i=0;argv[1][i] != '\0';i++) 
            filename[i] = argv[1][i];
    }
    else
    {
        printf("Please pass filename as arguments\n");
        return -1;
    }
    /*Initializing the struct variable*/
    sigInit.sa_handler = sig_handler;
    sigemptyset(&sigInit.sa_mask);
    
    /*Registering Handler for the Signals*/
    sigaction(SIGUSR1, &sigInit, NULL);
    sigaction(SIGUSR2, &sigInit, NULL);
    sigaction(SIGINT, &sigInit, NULL);
    
    /*Initialize semaphores*/
    if(sem_init(&signalThread2, 0, 0) != 0)
    {
        printf("Semaphore signalThread2 Init failed\n");
    }
    if(sem_init(&signalThread3, 0, 0) != 0)
    {
        printf("Semaphore signalThread3 Init failed\n");
    }
    
    /*Initializing Mutex*/
    pthread_mutex_init(&lockStat,NULL);
    
    
    error = pthread_attr_init(&newAttr); /*if Attr is NULL, default values will be used*/
    if(error != 0)
    {
        printf("Failed in pthread_attr_init\n");
        return -1;
    }
    
    
    /*Opens file to write data*/
    usrFile = fopen(filename,"w");
    if(usrFile == 0)
    {
        return -1;
    }
    /*Get inputs and write into the file until ENTER is pressed*/
    for(i=0;out != '\n';i++)
    {
        scanf("%c", (char *)&out);
        fputc(out , usrFile);
    }
    fclose(usrFile);
    usrFile = NULL;
    printf("Creating Stat Calculation Thread\n");
    /*Thread creation for the statistics calculator*/
    error = pthread_create(&newId, &newAttr, &statCalc, (void*)NULL);
    if(error != 0)
    {
        printf("Failed in pthread_create\n");
        return -1;
    }
    printf("Creating Stat Printer Thread\n");
    /*Thread creation for the statistics calculator*/
    error = pthread_create(&printId, &newAttr, &statPrint, (void*)NULL);
    if(error != 0)
    {
        printf("Failed in pthread_create\n");
        return -1;
    }
    /*wait for Child threads*/
    pthread_join(newId,(void*)&ret);
    
    pthread_join(printId,(void*)&ret);
    
    /*Destroy attributes*/
    pthread_attr_destroy(&newAttr);
    
    pthread_mutex_destroy(&lockStat);
    
    exit(EXIT_SUCCESS);
}





