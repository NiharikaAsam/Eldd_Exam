#include"chardriver_lowlevel.h"

#include"chardriver_lowlevel_ioctl.h"

#define SIZE 1024
#define NMINORS 2
#include"synch_semaphore.h"

#define LOCK 0
#define UNLOCK 1
#define NMINORS 1

MODULE_LICENSE("GPL");
struct cdev synch_semaphore_cdev;
struct class *synch_semaphore_class;
struct device *synch_semaphore_device;
/* Character driver's read function : Called when read() is called on the device node
 * The function returns the number of bytes actually written
 */
ssize_t synch_semaphore_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp)
{
	static unsigned int fake_val = 10;

	printk(" In the read function \n");

	/* Acquire the semaphore */
	if(down_interruptible(&sample_sem))
		return -ERESTARTSYS;
	/* Critical section starts here */

	// Let the current thread goto sleep//msleep(DELAY);

	/* Let the value received from the device is an increment of the previous */
	fake_val++;

	/* Send the data to the user */
	copy_to_user(Ubuff, &fake_val, sizeof(fake_val));

	/* Release the semaphore */
	up(&sample_sem);
	/* Critical section ends here */

	return sizeof(fake_val);
}


/* Define a device specific data */
typedef struct privatedata {
	int nMinor;
	char Kbuff[SIZE];
	struct cdev mycdev;

} chardriver_lowlevel_private;

chardriver_lowlevel_private devices[NMINORS];
int major;
int minor = 1;
dev_t deviceno;
struct class *chardriver_lowlevel_class;
struct device *chardriver_lowlevel_device;

/* Char driver open function called when open() is called on the device */
int chardriver_lowlevel_open(struct inode *inod, struct file *filp)
chardriver_lowlevel_private *dev = container_of(inod->i_cdev,chardriver_lowlevel_private, mycdev);
	filp->private_data = dev;

	printk("In char driver open() function device node : %d\n", dev->nMinor);

	return 0;
}

/* Char driver release function : Called when close() is called on the device */
int chardriver_lowlevel_release(struct inode *inod, struct file *filp)
{
	//get the device specific structure 
	chardriver_lowlevel_private *dev = filp->private_data;
	printk("In char driver release() function device node : %d\n", dev->nMinor);
	return 0;
}

/* Char driver read function : Called when read() is called on the device
  It is used to copy data to the user space.*/

ssize_t chardriver_lowlevel_read(struct file *filp, char __user *Ubuff, size_t count, loff_t *offp)
{
	chardriver_lowlevel_private *dev = filp->private_data;
	int res;

	printk("In char driver read() function\n");

	/* copy the data from kernel buffer to Us buffer */
	res = copy_to_user((char *)Ubuff , (char *)dev->Kbuff , strlen(dev->Kbuff) + 1);
	if(res == 0)
	{
		printk("data from kernel buffer to user buffer copied successfully with bytes : %d\n",
				strlen(dev->Kbuff));
		return strlen(dev->Kbuff);

	} else {

		printk("copy from kernel to user failed\n");
		return -1;
	}

	return 0;
}

ssize_t chardriver_lowlevel_write(struct file *filp, const char __user *Ubuff,
		size_t count, loff_t *offp)
{
	chardriver_lowlevel_private *dev = filp->private_data;
	int res;

	printk("In char driver write() function\n");

	/* Copy data from user space buffer to driver buffer */
	memset(dev->Kbuff,0,sizeof(dev->Kbuff));
	res = copy_from_user((char *)dev->Kbuff , (char *) Ubuff,count);
	if(res == 0)
	{
		printk("data from the user space : %s no of bytes : %d\n",dev->Kbuff, count);
		return count;
	} else {

		printk("copy from user space to kernel failed\n");
		return -EFAULT;
	}
	return 0;
}

/* Char driver unlocked_ioctl function : Called when ioctl() is called on the device*/
long chardriver_lowlevel_ioctl(struct file *f, unsigned int cmd,unsigned long)

//Define the file operations structure of the char driver 
struct file_operations chardriver_lowlevel_fops= {
	.owner 			= THIS_MODULE,
	.open			= chardriver_lowlevel_open,
	.release		= chardriver_lowlevel_release,
	.read			= chardriver_lowlevel_read,
	.write			= chardriver_lowlevel_write,
	.unlocked_ioctl = chardriver_lowlevel_ioctl
		.read=synch_semaphore_read
};

/* Initialisation function */
static int __init chardriver_lowlevel_init(void)
{
	int i;
	int res;

	printk("In init() function\n");

	/* Get the device number dynamically */
	res = alloc_chrdev_region(&deviceno , minor, NMINORS , DRIVER_NAME);
	if(res <0) {

		printk("register device no failed\n");
		return -1;
	}
	major = MAJOR(deviceno);

	//create a class file with the name DRIVER_NAME such that it  will appear in /sys/class/DRIVER_NAME
	 
	chardriver_lowlevel_class = class_create(THIS_MODULE , DRIVER_NAME);
	if(chardriver_lowlevel_class == NULL) {
		printk("class creation failed\n");
		return -1;
	}

	/* Create nMinors Device nodes , such users can access through nodes Ex :/dev/sample_cdev0 */
	for(i = 0; i < NMINORS; i++) {

		deviceno = MKDEV(major, minor + i);

		/* Attach file_operations to cdev and add the device to the linux kernel */
		cdev_init(&devices[i].mycdev , &chardriver_lowlevel_fops);
		cdev_add(&devices[i].mycdev, deviceno,1);

		/* Create the Device node in /dev/ directory */
		chardriver_lowlevel_device = device_create(chardriver_lowlevel_class ,
				NULL , deviceno , NULL ,"sample_cdev%d",i);
		if(chardriver_lowlevel_device == NULL) {

			class_destroy(chardriver_lowlevel_class);
			PERR("device creation failed\n");
			return -1;
		}

		devices[i].nMinor = minor + i;
	}

	return 0;
}

static int __init synch_semaphore_init(void)
{
	printk("In init() function\n");

	/* Create a character driver */
	alloc_chrdev_region(&deviceno , minor, NMINORS , DRIVER_NAME);
	major = MAJOR(deviceno);

	cdev_init(&synch_semaphore_cdev, &synch_semaphore_fops);
	cdev_add(&synch_semaphore_cdev , deviceno , 1);

	/* Create a node in /dev by the name "sample_cdev" */
	synch_semaphore_class = class_create(THIS_MODULE , DRIVER_NAME);
	synch_semaphore_device = device_create(synch_semaphore_class , NULL , deviceno , NULL , "sample_cdev");

	/* Initialise the semaphore */
	sema_init(&sample_sem ,UNLOCK);

	return 0;
}

/* Cleanup function */
static void __exit chardriver_lowlevel_exit(void)
{
	int i;

	PINFO("In exit() function\n");

	/* Remove cdev and device nodes with linux kernel */
	for(i = 0; i < NMINORS; i++) {

		deviceno = MKDEV(major, minor+i);
		cdev_del(&devices[i].mycdev);
		device_destroy(chardriver_lowlevel_class , deviceno);
	}

	/* Destroy the class we have created */
	class_destroy(chardriver_lowlevel_class);

	/* Unregister the device number from linux kernel */
	deviceno=MKDEV(major, minor);
	unregister_chrdev_region(deviceno, NMINORS);
}

module_init(chardriver_lowlevel_init)
module_exit(chardriver_lowlevel_exit)


