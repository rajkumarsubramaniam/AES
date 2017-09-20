#include <linux/kernel.h>
#include <asm-generic/errno.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/gfp.h>

SYSCALL_DEFINE3(myaes_sorting,int32_t*, src, size_t, len, int32_t*, dst )
{
    int32_t* ksrc;
    size_t i;
    size_t j;
    size_t status;
    printk("myaes_sorting()::Entered:Buffer Size:%ld\n",len);
    
    if(len == 0)
    {
        return -EINVAL;
    }
    
    ksrc = (int32_t*)kmalloc(len*(sizeof(int32_t)), GFP_KERNEL);     /*Dynamic Kernel Memory allocation*/
    if(ksrc == NULL)
    {
        printk("Kernel Memory not alloted\n");
        return -ENOSPC;
    }

    /*Copying input from user space*/
    if(src != NULL)
    {
        status = copy_from_user((void*)ksrc, (void*)src, len*(sizeof(int32_t)));
        if(status != 0)
        {
            printk("Copy to kernel space failed:Bytes:%ld",status);
            return -ENOSPC;
        }    
    }
    else
    {
        return -EINVAL;  /*No memory*/ 
    }   
    
    printk("myaes_sorting()::Started Sorting");
    /*Sorting Algorithm - Bubble Sort*/
    for(i=0;i<len-1;i++)
    {
        for(j=0;j<(len-i-1);j++)
        {
            if(*(ksrc+j) < *(ksrc+j+1))
            {
                int32_t temp;
                temp = *(ksrc+j);
                *(ksrc+j) = *(ksrc+j+1);
                *(ksrc+j+1) = temp;
            }
        }
    }
    printk("myaes_sorting()::Finished Sorting");
    /*Copying results to user space*/
    if(dst != NULL)
    {
        status = copy_to_user((void*)dst, (void*)ksrc, len*(sizeof(int32_t)));
        if(status != 0)
        {
            printk("Copy from kernel space failed:Bytes:%ld",status);
            return -ENOSPC;
        }
    }
    else
    {
        printk("Dst is NULL\n");
        return -EINVAL;  /*No memory*/ 
    }
    
    /*Free Kernel Memory*/
    if(ksrc != NULL)
        kfree(ksrc);
    
    printk("myaes_sorting()::Exit");
    return 0;
}
