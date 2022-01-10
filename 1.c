#include<linux/init.h>
#include<linux/module.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>


int deviceno;

int desd_open (struct inode *,struct file *);
int desd_release (struct inode *,struct file *); 
ssize_t desd_read (struct file *,char __user *,size_t , loff_t *);
ssize_t desd_write (struct file *,const char __user *,size_t , loff_t *);

char kbuff[100]; 
struct cdev *desd_cdev;

struct file_operations fops=
{
.owner=THIS_MODULE,
.read=desd_read,
.write=desd_write,
.open=desd_open,
.release=desd_release,
};

struct stats{
	int size;char buff[];
	int r_w;
};


// start of init module------------------------------------------------------------------------------
 
static int __init chardriver_init(void)
{
	int result;
	deviceno=MKDEV(255,0);
	printk(KERN_INFO" i am char driver init function\t dev no=%d\n",deviceno);

	result = register_chrdev_region(deviceno,1,"desd"); 

	if(result < 0)	
	{
		printk(KERN_INFO "ERROR \n");
//		exit(-1);
	}

	desd_cdev = cdev_alloc();
	desd_cdev->ops = &fops;	

	result=cdev_add(desd_cdev,deviceno,1);
	
	if(result < 0)	
	{
		printk(KERN_INFO "ERROR \n");
		//exit(-1);
	}

	return 0;
}

static void __exit chardriver_exit(void)
{

	unregister_chrdev_region(deviceno,1);
	cdev_del(desd_cdev);
	printk(KERN_INFO" bye bye char driver ...\n");

	
}

int desd_open(struct inode *inode,struct file *filp )
{
	printk("I am in open function \n");
	return 0;

}

int desd_release(struct inode *inode,struct file *filp )
{
	printk("I am in relese function \n");
	return 0;

}

ssize_t desd_read(struct file *filp,char __user *Ubuff,size_t count , loff_t *fops)
{

	int result , retval;
	
	result= copy_to_user(Ubuff,kbuff,sizeof(kbuff));

	if(result>=0)
	{
		printk(KERN_INFO" completion of data ( inside read) \n");
		return count;
	}
	else
	{
		printk(KERN_ERR" error in read function , cannot copy to user \n");
		retval=-1;
		return retval;
	}

}
ssize_t desd_write(struct file *filp,const char __user *Ubuff,size_t count , loff_t *fops)
{

	int result , retval;
	
	result= copy_from_user(kbuff,Ubuff,count);
//	printk(KERN_INFO " the data from user is %s %d\n", kbuff,result);

	if(result==0)
	{
//		printk(KERN_INFO " completion of data ( inside ) %c \t count  %d", *Ubuff,count);
		printk(KERN_INFO " the data from user( inside ) %s\n", kbuff);
	
		return count;
	}
	else
	{
		printk(KERN_ERR" error in write function , cannot copy to user \n");
		retval=-1;
		return retval;
	}

}
		
module_init(chardriver_init);
module_exit(chardriver_exit);










































































