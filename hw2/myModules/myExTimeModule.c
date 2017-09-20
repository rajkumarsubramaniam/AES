#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>

#define DELAY_500ms 125
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Module-Timer-Exercise");
MODULE_AUTHOR("Raj Kumar Subramaniam");

struct timer_list timer;

/*Call back function for the timer*/
void myExTimeModule_exit_callback(unsigned long param)
{
    static unsigned long count = 0;
    count++;
    printk("myExTimeModule Woken Up:%ld\n",count);
    /*Running again for 500 ms*/
    mod_timer(&timer, jiffies+DELAY_500ms);
}

/*Funtion to initialize the module*/
static int myExTimeModule_init(void)
{
    printk("Entered myExTimeModule Kernel Module\n");
    init_timer(&timer); /*initializing the timer*/
    timer.expires = jiffies + DELAY_500ms;
    /*Setting up Call back*/
    setup_timer(&timer, myExTimeModule_exit_callback, 0);
    /*Starting Timer*/
    add_timer(&timer);
    return 0;
}

/*Funtion to exit the module*/
static void myExTimeModule_exit(void)
{
    printk("Exiting the myExTimeModule Kernel Module\n");
    del_timer_sync(&timer);     /*Good practice to delete timers in Concurrent Programming*/
}

/*Registering Entry and Exit Modules*/
module_init(myExTimeModule_init);
module_exit(myExTimeModule_exit);
