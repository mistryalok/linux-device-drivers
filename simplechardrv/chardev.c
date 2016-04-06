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
//    #include <asm/system.h>
    #include <asm/uaccess.h>
     
    #define DEVICE_NAME "simpdev"
    #define BUFFER_SIZE 1024
     
    MODULE_LICENSE("Dual BSD/GPL");
    MODULE_AUTHOR("Alok Mistry");
    MODULE_DESCRIPTION("A simple character device driver.");
    MODULE_SUPPORTED_DEVICE(DEVICE_NAME);
     
static	char buff_my[100];
    int device_init(void);
    void device_exit(void);
    static int device_open(struct inode *, struct file *);
    static int device_release(struct inode *, struct file *);
    static ssize_t device_read(struct file *, char *, size_t, loff_t *);
    static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
     
    module_init(device_init);
    module_exit(device_exit);
     

/* file operations that we can do */
    static struct file_operations fops = {
        .read = device_read,
        .write = device_write,
        .open = device_open,
        .release = device_release
    };

/* Mentioning major and minor here */     
    static int device_major = 60;
    static int device_opend = 0;
    static char device_buffer[BUFFER_SIZE];
    static char *buff_rptr;
    static char *buff_wptr;
     
    module_param(device_major, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    MODULE_PARM_DESC(device_major, DEVICE_NAME " major number");
     
    int device_init() {
        int ret;
        ret = register_chrdev(device_major, DEVICE_NAME, &fops);
        if(ret < 0) {
            printk(KERN_ALERT "chardev: cannot obtain major number %d.\n", device_major);
            return ret;
        }
       // memset(device_buffer, 0, BUFFER_SIZE);
       // printk(KERN_INFO "chardev: chrdev loaded.\n");
        return 0;
    }
     
    void device_exit() {
        unregister_chrdev(device_major, DEVICE_NAME);
        printk(KERN_INFO "chardev: chrdev unloaded.\n");
    }
     
    static int device_open(struct inode *nd, struct file *fp) {
      //  if(device_opend) return -EBUSY;
      //  device_opend++;
      //  buff_rptr = buff_wptr = device_buffer;
      //  try_module_get(THIS_MODULE);
      printk(KERN_INFO "We have opened device");
	  return 0;
    }
     
    static int device_release(struct inode *nd, struct file *fp) {
       // if(device_opend) device_opend--;
       // module_put(THIS_MODULE);
        printk(KERN_INFO "We have closed device");
	return 0;
    }
     
    static ssize_t device_read(struct file *fp, char *buff, size_t length, loff_t *offset) {
        copy_to_user(buff, buff_my,length);
       // buff_rptr += bytes_read;
        return length;
    }
     
    static ssize_t device_write(struct file *fp, const char *buff, size_t length, loff_t *offset) {
       // int bytes_written = BUFFER_SIZE - (buff_wptr - device_buffer);
      //  if(bytes_written > length) bytes_written = length;
        copy_from_user(buff_my, buff, length);
       // buff_wptr += bytes_written;
        return length;
    }
     
    /*
    End of Source Code
    */
