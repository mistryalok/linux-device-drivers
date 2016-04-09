   /*
    AUTHOR: Alok Mistry
    PROGRAM: Character Device Driver
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
//    #include <asm/system.h>
    #include <asm/uaccess.h>
     
    #define DEVICE_NAME "mydevice"
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
    static char math_operation;
    static int operand_1=0;
    static int operand_2=0;
    static int result=0;
    static int resp_ready=0;
	/* to have some memory to write and rd */
    static char ker_buf[100];
    
    static dev_t first;
    static struct cdev c_dev;
    
    static struct class *driver_class=NULL;
    static struct device *ourDevice;
    /* our dev init and exit modules */
    static int device_init(void);
    static void device_exit(void);
    
    
    static int device_open(struct inode *, 	struct file *);
    static int device_release(struct inode *, struct file *);
    static ssize_t device_read(struct file *, char *, size_t, loff_t *);
    static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
     
    /* defining that they are actually ! */
    module_init(device_init);
    module_exit(device_exit);
    
    /*
     * Define some attribuites to do something
     */
    //static DEVICE_ATTR(parCrtl,S_IWUSR,NULL,writeSomeAttr);
    //static DEVICE_ATTR(isBusy,S_IRUGO,readSomeAttr,NULL);

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
    //static int device_major = 60;
    //static int device_opend = 0;
    //static char device_buffer[BUFFER_SIZE];
    //static char *buff_rptr;
    //static char *buff_wptr;
     
//    module_param(device_major, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
 //   MODULE_PARM_DESC(device_major, DEVICE_NAME " major number");
     
    
/*
 *Reading the file
 */

static ssize_t readSomeAttr(struct device *dev,
			    struct device_attribute *attr,char *buf)
{
  int isBusyResp=0;
  sprintf(buf,"Busy is %d", isBusyResp);
  return strlen(buf)+1;
}



/*
 * Writing the attribuites
 */
static ssize_t writeSomeAttr(struct device *dev,struct device_attribute *attr,
			     const char *buf,size_t count)
{
  printk(KERN_ALERT "we have got %d bytes",(int)count);
  if(count>0) {
      if(buf[0]=='=') {
	printk(KERN_ALERT "Doing calculation");
	resp_ready=1;
	switch (math_operation) {
	  case '+':
	  {
	    result=operand_1+operand_2;
	    break;
	  }
	  case '-':
	  {
	    result=operand_1-operand_2;
	    break;
	  }
	  default:
	     result=0;
	}
	printk(KERN_ALERT "The answer is %d",result);
      }
  }
  return count;
}
	  
    
static DEVICE_ATTR(parCrtl,S_IWUSR,NULL,writeSomeAttr);
static DEVICE_ATTR(isBusy,S_IRUGO,readSomeAttr,NULL);

/*
 * Registering my device with new method
 */
 static int __init device_init(void) /* Constructor */
{
      printk(KERN_ALERT "INIT Hi, Your device is registered");
      if (alloc_chrdev_region(&first, 0,1, "alok") < 0) {
	return -1;
      }
      
      if ((driver_class = class_create(THIS_MODULE, "myclass")) == NULL) {
	unregister_chrdev_region(first, 1);
	return -1;
      }
      ourDevice=device_create(driver_class,NULL,first,NULL,"mydevice");
	
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
      
      
      /*
       * Lets create some attribute on /sys/class/CLASSNAME/mydevice/
       */
      if(device_create_file(ourDevice,&dev_attr_parCrtl)<0) {
	printk(KERN_ALERT "Failed attr creation");
	return -1;
      }
      if(device_create_file(ourDevice,&dev_attr_isBusy)<0) {
	printk(KERN_ALERT "Failed attr creation");
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
	printk(KERN_ALERT "OPENED It seems like u have opened your device !");
	return 0;
    }
     
     
/*
 * Say goodbye to ur device : )
 */
 static int device_release(struct inode *nd, struct file *fp) {
        //if(device_opend) device_opend--;
        //module_put(THIS_MODULE);
	printk(KERN_ALERT "CLOSED Au Revoir!");
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
        printk(KERN_ALERT "REAR I am reading it!");
	int n;
	n=sprintf(ker_buf,"%d\n",result);
	copy_to_user(buff, ker_buf, n);
        //buff_rptr += bytes_read;
       return n;
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
        printk(KERN_ALERT "WRITE I am writing on it!");
	copy_from_user(ker_buf, buff, length);
        sscanf(ker_buf,"%d,%d,%c",&operand_1,&operand_2,&math_operation);
	ker_buf[length]=0;
	currLen=length;
	//buff_wptr += bytes_written;
        return length;
    }
     
//static DEVICE_ATTR(parCrtl,S_IWUSR,NULL,writeSomeAttr);/
//static DEVICE_ATTR(isBusy,S_IRUGO,readSomeAttr,NULL);

    /*
    End of Source Code
    */
