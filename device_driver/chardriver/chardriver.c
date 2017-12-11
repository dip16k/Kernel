/* fs.h -> file ooperation register_chderv_region function
		alloc_chardev_region
   kde_t.h -> to get major number and register device driver wtih major number
	      and register that device number with the kernel
		dev_t MKDEV( major ,minor ) static allocation
		alloc_chrdev_region(& dev_t,minor,count,"name");
   uaccess.h for copy_to_user and copy_from_user function
   cdev.h -> register devise to kernel
   types.h -> permission related issues*/
#include<linux/init.h>
#include<linux/module.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/uaccess.h>
/* module parameter */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DIP");
/* driver major and minor number */
int major ;
int minor = 1;
dev_t devno ; /* driver number*/
/* function for structure intialization */
int dip_open(struct inode * ino, struct file * fop)
{
	printk( KERN_DEBUG "-> dip_open function\n");
	return 0;
}
ssize_t dip_write(struct file * flip, const char __user * ubuff, size_t len, loff_t * file_offset)
{
	printk( KERN_DEBUG "-> dip_write function \n");
	return 0;
}
ssize_t dip_read(struct file *flip, char __user *ubuff ,size_t count,loff_t * file_offset)
{
	ssize_t ret;
	char kbuff[]=" --> hi this is kernel\n";
	printk( KERN_DEBUG "-> dip_read function \n");

	ret = copy_to_user((char *)ubuff , (char *)kbuff , sizeof(kbuff) );
	if(ret==0){
		printk( "data copied \n");
		return sizeof(kbuff);
	}
	else{
		printk( " ERROR in DATA COPY " );
	return -EFAULT;/* return number of bytes successfully copied*/
	}
	return ret;
}
int dip_release(struct inode *ino ,struct file * flip)
{
	printk( KERN_DEBUG "-> dip_release function\n");
	return 0;
} 	
/* structure intialization for file operation*/
struct file_operations fops = {
				.owner   = THIS_MODULE,
				.open    = dip_open,
				.read    = dip_read,
				.write   = dip_write,
				.release = dip_release,
};
/*structure pointer for registration our module for communicatio*/
struct cdev * dip_cdev;

static int __init char_driver_init(void)
{
	int reg = 0;
	printk( KERN_DEBUG " my char driver \n");
	/* dynamic alocation of major number */
	reg = alloc_chrdev_region(&devno,minor,1,"dip");
	major = MAJOR( devno );
	printk( "major number=%d  minor=%d\n",major,minor);
	if(reg<0){
		printk( KERN_DEBUG " major number allocation failure \n");
		return -EFAULT;
	}
/*	reg = register_chrdev_region( devno , 1 , "dip" );*/
	/* memory allocation for cdev structure */
	dip_cdev=cdev_alloc();
	dip_cdev->ops = &fops;
	reg = cdev_add(dip_cdev , devno , 1 );
	if(reg<0){
		printk( "cdev device registration failed  \n");
		return -EFAULT;
	}
	return 0;
}

static void __exit char_driver_exit(void)
{
	printk( KERN_DEBUG " char driver exit \n");
	cdev_del(dip_cdev);
	unregister_chrdev_region(devno , 1 );
}

module_init(char_driver_init);
module_exit(char_driver_exit);
