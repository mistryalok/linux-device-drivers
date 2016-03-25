#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/kernel.h>       /* printk() */
#include <linux/slab.h>         /* kmalloc() */
#include <linux/fs.h>           /* everything... */
#include <linux/errno.h>        /* error codes */
#include <linux/types.h>        /* size_t */
#include <linux/proc_fs.h>
#include <linux/fcntl.h>        /* O_ACCMODE */
#include <linux/seq_file.h>
#include <linux/cdev.h>

#include <asm/uaccess.h>        /* copy_*_user */

//#include "scull.h"

int alok_major=60,alok_minor=0;
/*
 * write init module first 
 * To initialize majorand minor
 */
int alok_init_module(void)
{
        int result, i;
        dev_t dev = 0;  /* Dev_t is used to maintain Major and Minor \
                        of the devices */

/*
 * Get a range of minor numbers to work with, asking for a dynamic
 * major unless directed otherwise at load time.
 */
        if (0) {
                /* dev is to turn device dev_t from major and minor */
                dev = MKDEV(60, 1);
                /* static allocation of device */
                /* scull_nr_devs= number of device we want */
                /* scull= naem of device that we want */ 
                result = register_chrdev_region(dev, 1, "alok_dev");
        } else {
                /* allocate it dynamicallywith minor addition! */
                result = alloc_chrdev_region(&dev, alok_minor,1, 
                                "alok_dev");
                /* reverse. get major now! */
                alok_major = MAJOR(dev);
        }
        /* if the registration is failed then handle the erros. */
        if (result < 0) {
                printk(KERN_WARNING "scull: can't get major %d\n", alok_major);
                return result;
        }
	printk("\n Okey so, i have %d as major !, Just got initialized \n",alok_major);

}


/* Now lets clean everyting! */
void alok_exit_module(void)
{
        int i;
        dev_t devno = MKDEV(alok_major, alok_minor);

        /* Get rid of our char dev entries */
        /*if (scull_devices) {
                for (i = 0; i < scull_nr_devs; i++) {
                        scull_trim(scull_devices + i);
                        cdev_del(&scull_devices[i].cdev);
                }
                kfree(scull_devices);
        }*/

//#ifdef SCULL_DEBUG /* use proc only if debugging */
//        scull_remove_proc();
//#endif

        /* cleanup_module is never called if registering failed */
        unregister_chrdev_region(devno, 1);

        /* and call the cleanup functions for friend devices */
 //       scull_p_cleanup();
  //      scull_access_cleanup();
	printk("Good bye world!");

}

module_init(alok_init_module);
module_exit(alok_exit_module);

