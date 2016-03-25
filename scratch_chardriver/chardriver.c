   /*
    AUTHOR: Alok Mistry
    PROGRAM: Character Device Driver
    VERSION: 1.0
    */
     
    #include <linux/kernel.h>
    #include <linux/module.h>
    #include <linux/moduleparam.h>
    #include <linux/init.h>
    #include <linux/slab.h>
    #include <linux/fs.h>
    #include <linux/fcntl.h>
    #include <linux/stat.h>
    #include <linux/types.h>
    #include <linux/errno.h>
    #include <asm/uaccess.h>

/* Name of the driver and buffer size that we want */
    #define DEVICE_NAME "chardev"
    #define BUFFER_SIZE 1024

/* Some Macros to define the driver */
    MODULE_LICENSE("GPL");
    MODULE_AUTHOR("Alok Mistry");
    MODULE_DESCRIPTION("A simple character device driver.");
    MODULE_SUPPORTED_DEVICE(DEVICE_NAME);


/* struct to perform file ops */ 
    static struct file_operations fops = {
        .read = device_read,
        .write = device_write,
        .open = device_open,
        .release = device_release
    };



/* Make some Major and Minors ! */
    static int device_major = 60;
    static int device_opend = 0;
    static char device_buffer[BUFFER_SIZE];
    static char *buff_rptr;
    static char *buff_wptr;



/* Initialization when everything starts */
int char_initialization(void_
	{
	
	}

