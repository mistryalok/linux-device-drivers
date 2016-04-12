   /*
    AUTHOR: Alok Mistry
    PROGRAM: Character Device Driver as an Adder
    VERSION: 4.0
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
//  #include <asm/system.h>
    #include <asm/uaccess.h>
     
    #define DEVICE_NAME "ALU"
    #define DEVICE_CLASS "chardevice"
    #define DRIVER_NAME "adder"
    //#define CLASS_NAME "AlokMistry"
    //#define BUFFER_SIZE 1024
     
   /*
    * License and other infos.
    */
    MODULE_LICENSE("GPL");
    MODULE_AUTHOR("Alok Mistry");
    MODULE_DESCRIPTION("An adder with some extra stuff!.");
    MODULE_SUPPORTED_DEVICE(DEVICE_NAME);
     
    /* Current len in read and write */
    static int currLen=0;
    static char math_operation;
    static int operand_1=0;
    static int operand_2=0;
    static int result=0;
    static int resp_ready=0;
    
    /* to have some memory to write and rd */
    static char ker_buf[100];
    static char mydata[10];
    
    static dev_t first;
    
    /* The chardevice struct in new way using CDev struct*/
    static struct cdev c_dev;
    
    /* Creating some class and device  */
    static struct class *driver_class=NULL;
    static struct device *ourDevice;
    
    /* our dev init and exit modules */
    static int device_init(void);
    static void device_exit(void);
    
    /* Some routine declarations */
    static int device_open(struct inode *,struct file *);
    static int device_release(struct inode *, struct file *);
    static ssize_t device_read(struct file *, char *, size_t, loff_t *);
    static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
     
    /* defining that they are actually ! */
    module_init(device_init);
    module_exit(device_exit);

    /* 
     *	file operations that we can do with our device
     */
    static struct file_operations fops = {
        .read = device_read,
        .write = device_write,
        .open = device_open,
        .release = device_release
    };

    
    /*
    *Reading the file
    */

    
	  
    
    
    
   
    /*
    * Registering my device with new method, kind of constructor
    */
    static int __init device_init(void) 
    {
	  printk(KERN_ALERT "device_init: Your device has got registered\n");
	  if (alloc_chrdev_region(&first, 0,1, DRIVER_NAME) < 0) {
	    return -1;
	  }
	  
	  if ((driver_class = class_create(THIS_MODULE, DEVICE_CLASS)) == NULL) {
	    unregister_chrdev_region(first, 1);
	    return -1;
	  }
	  ourDevice=device_create(driver_class,NULL,first,NULL,DEVICE_NAME);
	    
	  if (ourDevice == NULL) {
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


     
    static  void device_exit() {
	cdev_del(&c_dev);
	device_destroy(driver_class,first);
        class_destroy(driver_class);
//	unregister_chrdev(device_major, DEVICE_NAME);
        printk(KERN_ALERT "EXIT mydevice: mydevice is unloaded.\n");
    }
     
    
/*
 * call it when Our devce opens everytime 
 */
 static int device_open(struct inode *nd, struct file *fp) {
        //if(device_opend) return -EBUSY;
        //device_opend++;
       // buff_rptr = buff_wptr = device_buffer;
        //try_module_get(THIS_MODULE);
	printk(KERN_ALERT "OPENED It seems like u have opened your device !\n");
	return 0;
    }
     
     
/*
 * Say goodbye to ur device : )
 */
 static int device_release(struct inode *nd, struct file *fp) {
        //if(device_opend) device_opend--;
        //module_put(THIS_MODULE);
	printk(KERN_ALERT "CLOSED Au Revoir!\n");
	return 0;
    }
     
     
/*
 * Lets do some rd and wr, with device 
 */
  static ssize_t device_read(struct file *fp, char *buff, 
			     size_t length, loff_t *offset) {
      if(resp_ready) {
	resp_ready=0;
        //int bytes_read = strlen(buff_rptr);
        //if(bytes_read > length) bytes_read = length;
        printk(KERN_ALERT "READ I am reading it!\n");
	int n;
	//n=sprintf(ker_buf,"The results are %d\n",result);
	copy_to_user(buff, ker_buf, 10);
	
        //buff_rptr += bytes_read;
       return 10;
      }
     return 0;
    }
  
/*
 * But first, we need to write something
 */
    static ssize_t device_write(struct file *fp, const char *buff, 
				size_t length, loff_t *offset) {
        //int bytes_written = BUFFER_SIZE - (buff_wptr - device_buffer);
        //if(bytes_written > length) bytes_written = length;
        printk(KERN_ALERT "WRITE I am writing on it!\n");
	copy_from_user(ker_buf, buff, length);
        //sscanf(ker_buf,"%d,%d,%c",&operand_1,&operand_2,&math_operation);
	ker_buf[length]=0;
	resp_ready=1;
	//currLen=length;
	//buff_wptr += bytes_written;
        return length;
    }
     
//static DEVICE_ATTR(parCrtl,S_IWUSR,NULL,writeSomeAttr);/
//static DEVICE_ATTR(isBusy,S_IRUGO,readSomeAttr,NULL);

    /*
    End of Source Code
    */
