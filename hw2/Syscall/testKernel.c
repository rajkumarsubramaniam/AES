#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
    int32_t* src;
    int32_t* dst;
    size_t len = 256;
    
    src = (int32_t*)malloc(len*(sizeof(int32_t)));
    dst = (int32_t*)malloc(len*(sizeof(int32_t)));
    
    for(size_t i=0;i<len;i++)
    {
        *(src+i) = (int32_t)rand()%150; /*Add random Values*/
    }
   /*First call - size 256*/
   if(syscall(333, src, len, dst))
   {
     printf("Failed\n");
   }
   else
   {
        printf("Success\n");
   }
   
   for(size_t i=0;i<len;i++)
   {
        printf("%d ",*(dst+i));
   }
   printf("\n");
   
   /*Sys call - size 0*/ 
   len = 0;
   for(size_t i=0;i<len;i++)
   {
        *(src+i) = (int32_t)rand()%99; /*Add random Values*/
   }
   if(syscall(333, src, len, dst))
   {
     printf("Failed\n");
   }
   else
   {
        printf("Success\n");
   }
    
    free(src);
    len = 256;
    /*Sys call - src=NULL*/
    src = NULL;
   if(syscall(333, src, len, dst))
   {
     printf("Failed\n");
   }
   else
   {
        printf("Success\n");
   }
   
    free(dst);
    return 0;
}

