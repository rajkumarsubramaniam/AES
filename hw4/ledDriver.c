/****************************************************************
* file	: ledDriver.c
* brief : This is the loadable kernel module of char driver for 
driving Led on Beagle Bone Green with it differnet characteristics.
* author: Raj Kumar Subramaniam
* date	: 11/18/2017
*****************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h> 		/*Required for File System Implementations*/
#include <linux/device.h>	/*Required to use device creation APIs*/
#include <linux/fcntl.h>	/*Required to use the file structures*/
#include <linux/err.h>		/*Reuired to read errors from pointers*/
#include <linux/gpio.h>		/*Required to control LEDs*/
#include <linux/timer.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LED driver support for Beagle Bone Green");
MODULE_AUTHOR("Raj Kumar Subramaniam");
MODULE_VERSION("0.1"); 

#define DRIVER_NAME "myLedDriver"	/*Name of driver will appear in /dev/myLedDriver*/
#define CLASS_NAME "LED"			/*Class name for the device*/
#define FIRST_MINOR_NUMBER 0		/*First minor number for the device*/
#define DEVICE_COUNT 1 				/*Total number of devices supported for the driver*/
#define USER_LED 53
#define DEFAULT_DELAY 1000

/*Global Variables*/
static dev_t dev;		/*Stores device major and minor number of the driver*/
unsigned int majorNum; 	/*majorNum for the driver*/
static struct class*  ledDriverClass  = NULL;	/*Driver class Struct Pointer*/
static struct device* ledDriverDevice = NULL; 	/*Driver device Struct Pointer*/
struct timer_list timer;
uint16_t ledOnTime = DEFAULT_DELAY;
uint16_t ledOffTime = DEFAULT_DELAY;
uint8_t* kMemory = NULL;	/*Maximum Size possible per user data is 256*/
static bool led = false;
/*
Command variable to read the values, 
	0x00 - Send all data
	0x01 - Send Period of LED
	0x10 - Send Duty Cycle
	0x11 - Send Toggling Status
*/
uint8_t readCmd = 0x0000;	
/*Function declarations*/
static int ledDriver_open(struct inode *, struct file *);						/*FS open support*/
static ssize_t ledDriver_read(struct file *, char *, size_t, loff_t *);			/*FS read support*/
static ssize_t ledDriver_write(struct file *, const char *, size_t, loff_t *);	/*FS read support*/
static int ledDriver_release(struct inode *, struct file *);    				/*FS release support*/
void myLedTimerCallback(unsigned long param);

/*Structure to maintain the state of the LED*/
static struct ledState
{
	uint32_t rate;		/*Period of a cycle in ms*/
	uint32_t dutycycle;	/*Duty-cycle of LED cycle*/
	bool ledToggle;
}currentState;

/*File structure to support file operations fro user sapce for the Led Driver*/
static struct file_operations led_fops = 
{
	.open = ledDriver_open,
	.read = ledDriver_read,
	.write = ledDriver_write,
	.release = ledDriver_release
};

/****************************************************************
* name	: ledDriver_init
* brief	: This function is the initialization module for led Driver module
* param	: none
* return: returns 0 on success, or -1 on failure
*****************************************************************/
static int ledDriver_init(void)
{
	/*Registering Char Driver*/
	majorNum = register_chrdev(0, DRIVER_NAME, &led_fops);	/*Allocates a major numbers and returns in dev*/
	if(majorNum < 0)
	{
		printk(KERN_ALERT "Led Driver:Failed Major Number allocation\n");
		return -1;
	}
	printk(KERN_INFO "Led Driver: Driver Initialized: %u\n", majorNum);
	dev = MKDEV(majorNum, 0);
	/*Creating class structure to create device files*/
	ledDriverClass = class_create(THIS_MODULE,CLASS_NAME);
	if(IS_ERR(ledDriverClass))
	{
		unregister_chrdev(majorNum, DRIVER_NAME);
		printk(KERN_ALERT "Led Driver:Failed Class Create\n");
		return PTR_ERR(ledDriverClass);
	}
	printk(KERN_INFO "Led Driver: Class Structure Created\n");

	/*Creating Device files and registering it with sysfs*/
	ledDriverDevice = device_create(ledDriverClass, NULL, dev, NULL, DRIVER_NAME);
	if(IS_ERR(ledDriverDevice))
	{
		class_destroy(ledDriverClass);
		unregister_chrdev(majorNum, DRIVER_NAME);
		printk(KERN_ALERT "Led Driver:Failed Device Create\n");
		return PTR_ERR(ledDriverDevice);
	}
	printk(KERN_INFO "Led Driver: Device Structure Created and Registered\n");

	if(gpio_is_valid(USER_LED)==0)
	{
		printk(KERN_ALERT "The GPIO is not valid\n");
	}
/*	if(gpio_request(USER_LED, "ledDriverSys"))
	{
		printk(KERN_ALERT "Led Driver: Failed to set up LED GPIO\n");
	}         
*/
   	if(gpio_direction_output(USER_LED, 0))   /*Set GPIO to output and turn the value 0*/
   	{
		printk(KERN_ALERT "Led Driver: Failed to set GPIO Direction\n");
	}  
  	if(gpio_export(USER_LED, false))
  	{
		printk(KERN_ALERT "Led Driver: Failed GPIO Export\n");
	} 	
	/*seting up Timer To turn ON and OFF Leds*/		
	init_timer(&timer); /*initializing the timer*/
    /*Setting up Call back*/
    setup_timer(&timer, myLedTimerCallback, 0);
	return 0;
}

/****************************************************************
* name	: ledDriver_exit
* brief	: This function is the exit module for led Driver module
* param	: none
* return: none
*****************************************************************/
static void ledDriver_exit(void)
{
	/*Chronogically destroy the creations made before*/
	gpio_unexport(USER_LED);	/*releases the LED from sysfs*/
	//gpio_free(USER_LED);		/*Releases the LED to the system*/=
	del_timer(&timer);
	device_destroy(ledDriverClass, dev);
	class_unregister(ledDriverClass);
	class_destroy(ledDriverClass);
	unregister_chrdev(majorNum, DRIVER_NAME);
	
	printk(KERN_INFO "Led Driver: Device Destroyed\n");
}

/****************************************************************
* name	: myLedTimerCallback
* brief	: This function is a callback timer of timer
* param	: unsigned long
* return: none
*****************************************************************/
void myLedTimerCallback(unsigned long param)
{
    printk( KERN_INFO "Led Driver: Timer callback\n");
    if(led == 0)
    {
    	led = true;
    	gpio_set_value(USER_LED,true);
    	mod_timer(&timer, jiffies+ledOnTime);	/*Turning on Led for ON time*/
    }
    else
    {
    	led = false;
    	gpio_set_value(USER_LED,false);
    	mod_timer(&timer, jiffies+ledOffTime);	/*Turning Off led for OFF time*/
    }
}

/****************************************************************
* name	: ledDriver_open
* brief	: This function is called when a device is opened from user space
* param	: struct inode *, struct file *
* return: 0 on success and -1 on failure
*****************************************************************/
static int ledDriver_open(struct inode *pinode, struct file *pfile)
{
	static int16_t count = 0;		
	count++;		/*Keeps the count of number of device opened*/
	/*Initializing the state of the LED*/
	currentState.rate = 1000;
	currentState.dutycycle = 50;
	currentState.ledToggle = false;
	pfile->private_data = (void*)&currentState;
	printk(KERN_INFO "Led Driver: Device opened: %u\n", count);
	return 0;
}

/****************************************************************
* name	: ledDriver_read
* brief	: This function is called when user application needs to 
retrive data from the device
* param	: struct file *, char __user *, size_t, loff_t *
* return: Negative value on failure, Number of bytes read on success
*****************************************************************/
static ssize_t ledDriver_read(struct file *pfile, char *buff, size_t count, loff_t * position)
{
	char data[100];
	int32_t retVal;
	struct ledState* state;
	printk(KERN_INFO "Led Driver: Device read called\n");
	state = pfile->private_data;
	/*Based on the read commands data will be sent back*/
	if(readCmd == 0x00)
	{
		sprintf(data,"%d %d %d",state->rate,state->dutycycle,state->ledToggle);
	}
	else if(readCmd == 0x01)
	{
		sprintf(data,"%d",state->rate);
	}
	else if(readCmd == 0x10)
	{
		sprintf(data,"%d",state->dutycycle);
	}
	else
	{
		sprintf(data,"%d",state->ledToggle);
	}
	if(buff != NULL)
    {
    	retVal = copy_to_user((void*)buff, (void*)data, count);
        if(retVal)
        {
			printk(KERN_ALERT "Led Driver: Failed Copy to User:%d\n",retVal);
		}
    }
	return 0;
}

/****************************************************************
* name	: ledDriver_write
* brief	: This function is called when user application needs to 
send data into the device
* param	: struct file *, const char __user *, size_t, loff_t *
* return: Negative value on failure, Number of bytes sent on success
*****************************************************************/
static ssize_t ledDriver_write(struct file *pfile, const char *buff, size_t count, loff_t * position)
{
	struct ledState* state;
	state = pfile->private_data;
	kMemory = (uint8_t*)kmalloc(count, GFP_KERNEL);     /*Dynamic Kernel Memory allocation*/
	printk(KERN_INFO "Led Driver: Device write called\n");
	if(copy_from_user((void*)kMemory, (void*)buff, count+1))
	{
		printk(KERN_ALERT "Led Driver: Failed Copy from User\n");
	}
	printk(KERN_INFO "Led Driver: The received message is %s\n", kMemory);
	
	/*Assuming write command is always in the order, read Command, rate-dutyCycle-LedToggling*/
	sscanf(kMemory, "%d %d %d %d",&readCmd, &state->rate,&state->dutycycle,&state->ledToggle);
	
	ledOnTime = state->dutycycle;
	ledOffTime = state->rate - ledOnTime;
	
	timer.expires = jiffies;	/*Updating Timer variable*/
	/*Free Kernel Memory*/
    if(kMemory != NULL)
    {
    	kfree(kMemory);
    	kMemory = NULL;
    }
	/*Starting Timer*/
	if(state->ledToggle)
	{
		mod_timer(&timer,jiffies);
	}
    else
    {
    	del_timer(&timer);
    	printk(KERN_INFO "Led Driver: Led Toggling stopped based on the request\n");
    	gpio_set_value(USER_LED,false);
    }	

	return 0;
}

/****************************************************************
* name	: ledDriver_release
* brief	: This function is called when the device is closed from user space
* param	: struct inode *, struct file *
* return: 0 on success and -1 on failure
*****************************************************************/
static int ledDriver_release(struct inode *pinode, struct file *pfile)
{
	printk(KERN_INFO "Led Driver: Device closed\n");
	return 0;
}

/*Registering Entry and Exit Modules*/
module_init(ledDriver_init);
module_exit(ledDriver_exit);
