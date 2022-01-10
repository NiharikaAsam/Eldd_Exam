#include <linux/init.h>// initialization header
#include <linux/module.h>// module header
#include <linux/fs.h>//file stack

#include <linux/uaccess.h>// header for user space access 
#include <linux/kdev_t.h> // kernel device 
#include <linux/cdev.h> //character device header
#include <linux/types.h> // permissions
#include <linux/semaphore.h>

#define PRINT(...){printk(KERN_INFO "my_driver: "__VA_ARGS__);}

#define DEV_NAME "my_device2"
#define MAJ_NUM 266 // common major number
#define MIN_NUM1 5// miner no. for dev 1 
#define MIN_NUM2 2 // miner no. for dev 2 
#define KBUFFSIZE 50 // data buffer size 

int CHAR_DEV_open(struct inode *__inode, struct file *__file);
int CHAR_DEV_release(struct inode *__inode, struct file *__file);
ssize_t CHAR_DEV_read(struct file *__file, char *__ubuff, size_t __nbytes, loff_t *offset);
ssize_t CHAR_DEV_write(struct file *__file, const char *__ubuff, size_t __nbytes, loff_t *offset);
~                               
