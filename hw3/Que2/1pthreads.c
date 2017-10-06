/**
 * @file 1pthreads.c
 * @brief This file implements the examples for use of pthread APIs
 * @author Raj Kumar Subramaniam
 * @date October 5,2017
 **/
#define _GNU_SOURCE
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

pthread_mutex_t lockStat;   /*Mutex*/
pthread_cond_t condLock;    /*Condition variable*/
int32_t shared=0;

/*Function to create an example for get and set attributes in a thread */
void* startFunction(void * msg)
{
    int32_t data;
    pthread_attr_t currentAttr;
    printf("Child Thread Id:%ld\n",pthread_self());
    
    /*Read attributes of current thread*/
    pthread_getattr_np(pthread_self(), &currentAttr);
    printf("Thread1 created\n");
    
    pthread_attr_getdetachstate(&currentAttr,&data);
    printf("Thread1 Detached State:%d\n",data);    /*PTHREAD_CREATE_JOINABLE or PTHREAD_CREATE_DETACHED*/
    
    pthread_attr_getschedpolicy(&currentAttr,&data);
    printf("Thread1 scheduling policy:%d\n",data); 
    
    pthread_attr_getscope(&currentAttr,&data);
    printf("Thread1 Scope:%d\n",data);  /*PTHREAD_SCOPE_SYSTEM=0,PTHREAD_SCOPE_PROCESS=1*/
    
    
    pthread_mutex_lock(&lockStat);
        shared+=20;
        printf("Thread1 Shared:%d\n", shared);
        pthread_cond_signal(&condLock);
    pthread_mutex_unlock(&lockStat);
    
    pthread_attr_destroy(&currentAttr);	/*destroys the attribute*/
    pthread_exit(NULL); /*Exits current thread*/
}

/*Function to create an example for mutex lock, try lock and unlock , condition variables*/
void* secondFun(void* msg)
{
    if(!pthread_mutex_trylock(&lockStat))	/*Tries to lock and continues*/
    {   
        pthread_cond_wait(&condLock, &lockStat);	/*waits if thread 3 reads before thread 2 writes*/
        shared++;
        printf("Thread2 Sucesss Shared:%d\n", shared);
        pthread_mutex_unlock(&lockStat);
    }
    else	/*if trylock failed - does not block*/
    {
         printf("Thread2 Failed Shared:%d\n", shared);
    }
     
}

/*main function - creates two threads*/
int main()
{
    uint16_t error,ret;
    int32_t data;
    pthread_t newId, thread2;
    pthread_attr_t newAttr; /*Attributes for new thread*/
    pthread_attr_t defaultAttr;
    
    /*Initializing Mutex*/
    pthread_mutex_init(&lockStat,NULL);
    
    /*Initializing Cond variable*/
    pthread_cond_init(&condLock, NULL);
    
    pthread_getattr_default_np(&defaultAttr);
    
    pthread_attr_getdetachstate(&defaultAttr,&data);
    printf("Default Detached State:%d\n",data);    /*PTHREAD_CREATE_JOINABLE or PTHREAD_CREATE_DETACHED*/
    
    pthread_attr_getschedpolicy(&defaultAttr,&data);
    printf("Default scheduling policy:%d\n",data); 
    

    /*if Attr is NULL, default values will be used*/
    error = pthread_attr_init(&newAttr); 
    if(error != 0)
    {
        printf("Failed in pthread_attr_init\n");
        return -1;
    }
    printf("Parent Thread Id:%ld\n",pthread_self());
    
    pthread_attr_setdetachstate(&newAttr, PTHREAD_CREATE_JOINABLE);     /*Set attribute detach state - PTHREAD_CREATE_JOINABLE = 0, PTHREAD_CREATE_DETACHED=1*/
    
    /*Pthread create*/
    error = pthread_create(&newId, &newAttr, &startFunction, (void*)NULL);	/*Create Thread*/
    if(error != 0)
    {
        printf("Failed in pthread_create\n");
        return -1;
    }
    
    error = pthread_create(&thread2, NULL, &secondFun, (void*)NULL);		/*Create Thread*/
    if(error != 0)
    {
        printf("Failed in pthread_create\n");
        return -1;
    }
    
    /*Attr Destroy*/
    pthread_attr_destroy(&newAttr);
    pthread_attr_destroy(&defaultAttr);
    
    /*Waits for the child thread to finish*/
    pthread_join(newId,(void*)&ret);
    
    pthread_mutex_destroy(&lockStat);	
    
    pthread_cond_destroy(&condLock);
    
    exit(EXIT_SUCCESS);
}
