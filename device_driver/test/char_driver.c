#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DIP");
MODULE_DESCRIPTION("MY CHAR DRIVER");

static char kernelBuffer[512];

dev_t deviceNo;/* for getting device number for our driver */
/*structure for file operations */
int driver_open(struct inode *ino, struct file *fp)
{/* it called when our device related driver file open in user space */
	printk(KERN_DEBUG " DRIver open section\n");
	return 0;
}
int driver_release(struct inode *ino, struct file *fp)
{
	printk(KERN_DEBUG "Driver close file section\n");
	return 0;
}
ssize_t driver_read(struct file *fp, char __user *ubuf, size_t len, loff_t *lof)
{
	int ret;
	ret = copy_to_user((char*)ubuf, (char*)kernelBuffer, sizeof(ubuf));
	if( ret < 0 ) {
		printk(KERN_DEBUG "failed to copy data to user space\n");
		return -EFAULT;
	}
	return ret;
}
ssize_t driver_write(struct file *fp, const char __user *ubuf, size_t len, loff_t *lof)
{
	int ret;
	ret = copy_from_user((char*)kernelBuffer, (char*)ubuf, sizeof(ubuf));
	if( ret < 0) {
		printk(KERN_DEBUG "failed to receive data from user space\n");
		return -EFAULT;
	}
	return ret;
}

static const struct file_operations fops = {
		.owner		= THIS_MODULE,
		.open		= driver_open,
		.release	= driver_release,
		.read		= driver_read,
		.write		= driver_write,
};

/* Driver init section */
static int __init driver_init(void)
{
	int ret;
	printk(KERN_DEBUG "this is driver init section\n");
/* alloc_chrdev_region= for dynamic major number allocation(dev_t *,minor number,count,name); */
	ret = alloc_chrdev_region(&deviceNo,10/*minor*/,1/*count*/,"dip_charDriver");
	if( ret < 0) {
		printk(KERN_DEBUG "dynamic major number allocation failure\n");
			return -EFAULT;
	}
	printk(KERN_DEBUG "majoNumber=%d minorNUmber=%d\n",MAJOR(deviceNo),MINOR(deviceNo));
/* registering our driver with kernel sub system */
/*	ret = register_chrdev_region(deviceNo,10//minor//,"dip_charDriver");
	if( ret < 0) {
		printk(KERN_DEBUG "driver registration failure\n");
		return -EFAULT;
	}*/
	return 0;
}
static void __exit driver_exit(void)
{
	printk(KERN_DEBUG "this is driver exit section\n");
	unregister_chrdev_region(deviceNo, 10/*minor*/);/* removing our driver from kernel sub system*/
}

module_init(driver_init);
module_exit(driver_exit);

