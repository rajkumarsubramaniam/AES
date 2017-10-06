/**
 * @file 4parentsTraverse.c
 * @brief This file implements the Kernel module to traverse the parents and prints data
 * @author Raj Kumar Subramaniam
 * @date October 1,2017
 **/
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched/task.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Traversing-Parents-Exercise");
MODULE_AUTHOR("Raj Kumar Subramaniam");

/*
 * Init routine for the module
 */
static int parentFinder_init(void)
{
    struct task_struct *tempProcess;
    struct list_head* tempList;
    size_t childNo = 0;
    printk("ParentFinder::Init\n");
    /*Iterating from the current process using the parent variable*/
    for (tempProcess = current; tempProcess != &init_task; tempProcess = tempProcess->parent)   
    {
        printk("Thread Name:%s\n", tempProcess->comm);      /*Name of the process*/
        printk("Thread Id:%d\n", tempProcess->pid);         /*PID of the process*/
        printk("Thread State:%ld\n", tempProcess->state);   /* -1 unrunnable, 0 runnable, >0 stopped: */
        list_for_each(tempList, &tempProcess->children) /*list_for_each - This iterates for all the child nodes*/
        {
            childNo++;  /*Counts the number of children*/
        }
        printk("Number of Children:%ld\n",childNo);
        childNo = 0;
    }
    
    /*Print Init Process*/
    printk("Thread Name:%s\n", tempProcess->comm);
    printk("Thread Id:%d\n", tempProcess->pid);
    printk("Thread State:%ld\n", tempProcess->state);
    list_for_each(tempList, &tempProcess->children) /*list_for_each - This iterates for all the child nodes*/
    {
        childNo++;
    }
    printk("Number of Children:%ld\n",childNo);
    return 0;
}

/*
 * Exit routine for the module
 */
static void parentFinder_exit(void)
{
    printk("ParentFinder::Exit\n");
}


/*Registering Entry and Exit Modules*/
module_init(parentFinder_init);
module_exit(parentFinder_exit);

