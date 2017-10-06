/**
 * @file kthread_profiler.c
 * @brief This file implements the kthread_create to profiling the time in kernel thread creation
 * @author Raj Kumar Subramaniam
 * @date October 5,2017
 **/
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched/task.h>
#include <linux/kthread.h>
#include <linux/timex.h>	

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("KthreadProfiler-Exercise");
MODULE_AUTHOR("Raj Kumar Subramaniam");

/*Struct to point new thread*/
struct task_struct* newThread;

/*Get CPU cycles*/
cycles_t startClk;
cycles_t endClk;

/*New thread will execute this function*/
int kthreadTest(void * data)
{
	printk("Inside the created Kernel Thread\n");
	do_exit(0);		/*Exit when done*/
}

/*
 * Init routine for the module
 */
static int kthreadProfiler_init(void)
{
	printk("kthreadProfiler::Init\n");
	startClk = get_cycles();		/*get CPU clk cycles*/
	newThread = kthread_create(kthreadTest, NULL, "kthreadProfilerTest");
	endClk = get_cycles();			/*get CPU clk cycles*/
	printk("CPU Cycles before kthread_create:%llu\n",startClk);
	printk("CPU Cycles after  kthread_create:%llu\n",endClk);
	printk("CPU Cycles used for kthread_create:%llu\n", endClk-startClk);	/*Number of CPU cycles taken*/
	return 0; 
}

static void kthreadProfiler_exit(void)
{
    if(newThread)
    {
    	kthread_stop(newThread);		/*Force stop when removing mod*/
	}
	printk("kthreadProfiler::Exit\n");
}

 
/*Registering Entry and Exit Modules*/
module_init(kthreadProfiler_init);
module_exit(kthreadProfiler_exit);
