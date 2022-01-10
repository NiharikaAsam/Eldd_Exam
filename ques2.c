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

static void MY_MODULE_exit(void)
{
    dev_t dev;
    dev = MKDEV(MAJ_NUM, MIN_NUM1);

    unregister_chrdev_region(dev, 1);
    
    dev = MKDEV(MAJ_NUM, MIN_NUM2);

    unregister_chrdev_region(dev, 1);

    cdev_del(mycdev1);
    cdev_del(mycdev2);

    kfree(rwstats);

    PRINT("Unregisterd device with MAJ=%d,MIN=%d", MAJOR(dev), MINOR(dev))
}

module_init(MY_MODULE_init);
module_exit(MY_MODULE_exit);

int CHAR_DEV_open(struct inode *__inode, struct file *__file)
{
    printk("CHARDEV_open");
    return 0;
}
int CHAR_DEV_release(struct inode *__inode, struct file *__file)
{
    PRINT("CHARDEV_release");
    return 0;
}

ssize_t CHAR_DEV_read(struct file *__file, char *__ubuff, size_t __nbytes, loff_t *offset)
{
    int numtoread, copyres;
    down(mysemaphore);
    printk("Inside CHAR");
    numtoread= __nbytes > KBUFFSIZE ? KBUFFSIZE : __nbytes;
    copyres = copy_to_user(__ubuff, kbuff, numtoread);

    if (copyres == 0)
    {
        printk("Copied %d bytes to user space", numtoread);
        return numtoread;
    }
    printk("Inside CHAR_DEV_read()");
    up(mysemaphore);
    return 0;
}
ssize_t CHAR_DEV_write(struct file *__file, const char *__ubuff, size_t __nbytes, loff_t *offset)
{
    int numtowrite, copyres;
    down(mysemaphore);
    PRINT("Inside CHAR_DEV_write() start")

    numtowrite = __nbytes > KBUFFSIZE ? KBUFFSIZE : __nbytes;

    copyres = copy_from_user(kbuff, __ubuff, numtowrite);

    if (copyres == 0)
    {
        printk("Copied %d bytes from user space to kernel space", numtowrite);
        return numtowrite;
    }
    printk("Inside CHAR_DEV_write()");
    up(mysemaphore);
    return 0;
}
