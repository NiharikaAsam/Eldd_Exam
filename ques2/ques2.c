#include "my_driver.h"
struct cdev *mycdev1;
struct cdev *mycdev2;

static char *kbuff[KBUFFSIZE];

static struct semaphore *mysemaphore;

struct file_operations fops1 =
    {
        .owner = THIS_MODULE,
        .open = CHAR_DEV_open,
        .write = CHAR_DEV_write,
        .read = CHAR_DEV_read,
        .release = CHAR_DEV_release,
};
struct file_operations fops2 =
    {
        .owner = THIS_MODULE,
        .open = CHAR_DEV_open,
        .write = CHAR_DEV_write,
        .read = CHAR_DEV_read,
        .release = CHAR_DEV_release,
};

static int MY_MODULE_init(void)
{
    int res;
    dev_t dev1, dev2;
    printk("My Module init start");
    dev1 = MKDEV(MAJ_NUM, MIN_NUM1);
    dev2 = MKDEV(MAJ_NUM, MIN_NUM2);

    res = register_chrdev_region(dev1, 1, DEV_NAME);

    if (res != 0)
    {
        printk("couldnot register char device");
        return -1;
    }
    else
    {
        mycdev1 = cdev_alloc();
        mycdev1->ops = &fops;
        res = cdev_add(mycdev1, dev1, 1);
        if (res != 0)
        {
            unregister_chrdev_region(dev1, 1);
            printk("Could not add cdev")
            return -1;
        }
    }
    res = register_chrdev_region(dev2, 1, DEV_NAME);
    if (res != 0)
    {
        printk("Couldnot register char device");
        return -1;
    }
    else
    {
        mycdev2 = cdev_alloc();
        mycdev2->ops = &fops;
        res = cdev_add(mycdev2, dev2, 1);
        if (res != 0)
        {
            unregister_chrdev_region(dev2, 1);
            printk("Could not add cdev");
            return -1;
        }
    }
    sema_init(mysemaphore);

    PRINT("My Module init end");
    return 0;
}

            
