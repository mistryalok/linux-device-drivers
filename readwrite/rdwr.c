   /*
    AUTHOR: Alok Mistry
    PROGRAM: Character Device Driver
    VERSION: 2.0
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
    #include <linux/string.h>
    #include <linux/device.h>
    #include <linux/cdev.h>
//    #include <asm/system.h>
    #include <asm/uaccess.h>
     
    #define DEVICE_NAME "chardev"
    #define CLASS_NAME "AlokMistry"
    #define BUFFER_SIZE 1024
     
   /*
    * License and other infos.
    */
    MODULE_LICENSE("Dual BSD/GPL");
    MODULE_AUTHOR("Alok Mistry");
    MODULE_DESCRIPTION("A simple character device driver.");
    MODULE_SUPPORTED_DEVICE(DEVICE_NAME);
     
    /* Current len in read and write */
    static int currLen=0;
    /* to have some memory to write and rd */
    static char ker_buf[100];
    
    static dev_t first;
    static struct cdev c_dev;
    
    static struct class *driver_class=NULL;
    /* our dev init and exit modules */
    int device_init(void);
    void device_exit(void);
    
    
    static int device_open(struct inode *, 	struct file *);
    static int device_release(struct inode *, struct file *);
    static ssize_t device_read(struct file *, char *, size_t, loff_t *);
    static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
     
    /* defining that they are actually ! */
    module_init(device_init);
    module_exit(device_exit);
     

    /* 
     *	file operations that we can do with our device
     *	
     */
    static struct file_operations fops = {
        .read = device_read,
        .write = device_write,
        .open = device_open,
        .release = device_release
    };

    /* Mentioning major and minor here */     
    static int device_major = 60;
    static int device_opend = 0;
    //static char device_buffer[BUFFER_SIZE];
    //static char *buff_rptr;
    //static char *buff_wptr;
     
    module_param(device_major, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    MODULE_PARM_DESC(device_major, DEVICE_NAME " major number");
     
    
/*
 * Registering my device with new method
 */
 static int __init device_init(void) /* Constructor */
{
      printk(KERN_INFO "Namaskar: Your device is registered");
      if (alloc_chrdev_region(&first, 0, 1, "alok") < 0) {
	return -1;
      }
      
      if ((driver_class = class_create(THIS_MODULE, "myclass")) == NULL) {
	unregister_chrdev_region(first, 1);
	return -1;
      }
	
      if (device_create(driver_class, NULL, first, NULL, "mydevice") == NULL) {
	class_destroy(driver_class);
	unregister_chrdev_region(first, 1);
	return -1;
      }
      
	cdev_init(&c_dev, &fops);
	
      if (cdev_add(&c_dev, first, 1) == -1) {
	device_destroy(driver_class, first);
	class_destroy(driver_class);
	unregister_chrdev_region(first, 1);
	return -1;
      }
      return 0;
}


     
    void device_exit() {
	cdev_del(&c_dev);
	device_destroy(driver_class,first);
        unregister_chrdev(device_major, DEVICE_NAME);
        printk(KERN_INFO "chardev: chrdev unloaded.\n");
    }
     
    
/*
 * call it when Our devce opens everytime 
 */
 static int device_open(struct inode *nd, struct file *fp) {
        //if(device_opend) return -EBUSY;
        //device_opend++;
       // buff_rptr = buff_wptr = device_buffer;
        //try_module_get(THIS_MODULE);
	prink(KERN_ALERT "It seems like u have opened your device !");
	return 0;
    }
     
     
/*
 * Say goodbye to ur device : )
 */
 static int device_release(struct inode *nd, struct file *fp) {
        //if(device_opend) device_opend--;
        //module_put(THIS_MODULE);
	printk(KERN_ALERT "Au Revoir!");
	return 0;
    }
     
     
/*
 * Lets do some rd and wr, with device 
 */
  static ssize_t device_read(struct file *fp, char *buff, size_t length, loff_t *offset) {
        //int bytes_read = strlen(buff_rptr);
        //if(bytes_read > length) bytes_read = length;
        printk(KERN_ALERT "I am reading it!");
	copy_to_user(buff, ker_buf, currLen);
        //buff_rptr += bytes_read;
        return currLen;
    }
  
/*
 * But first, we need to write something
 */
    static ssize_t device_write(struct file *fp, const char *buff, size_t length, loff_t *offset) {
        //int bytes_written = BUFFER_SIZE - (buff_wptr - device_buffer);
        //if(bytes_written > length) bytes_written = length;
        printk(KERN_ALERT "I am writing on it!");
	copy_from_user(buff_wptr, buff, bytes_written);
        ker_buf[length]=0;
	currlen=length;
	//buff_wptr += bytes_written;
        return length;
    }
     
    /*
    End of Source Code
    */
