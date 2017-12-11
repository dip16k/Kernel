#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include "char-ioctl.h"
#include <linux/spinlock.h>
#include <linux/string.h>
/* module parameter*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DIP");

static int minor = 0;
dev_t device_num;
/* member finction defination for file_op structure */
int dip_open(struct inode *ino, struct file *flip)
{
	printk(KERN_DEBUG"-->> open function\n");
	return 0;
}
int dip_release(struct inode *ino, struct file *flip)
{
	printk(KERN_DEBUG"-->> in release function\n");
	return 0;
}
ssize_t dip_read(struct file * flip, char __user *ubuff, size_t len, loff_t *lof)
{
	char kbuff[10];
	int ret;
	printk(KERN_DEBUG"-->in read section\n");
	ret = copy_to_user((char*)ubuff, (char*)kbuff, sizeof(kbuff));
	if (ret == 0){
		printk(KERN_DEBUG"data copied to user space \n");
		return sizeof(kbuff);
	}
	else{
		printk(KERN_ALERT"data copy failure\n");
		return -EFAULT;
	}
	return ret;
}
ssize_t dip_write(struct file *flip, const char __user *ubuff, size_t len, loff_t *lof)
{
	char kbuff[10];
	int ret;
	printk(KERN_DEBUG"-->> in write section\n");
	ret = copr_from_user((char *)kbuff, (char *)ubuff, sizeof(ubuff));
	if(ret == 0 ){
		printk(KERN_DEBUG"data copy user to kernel\n");
		return sizeof(ubuff);
	}
	else{
		printk(KERN_DEBUG"data reception failure\n");
		return -EFAULT;
	}
	return ret;
}
long dip_ioctl(struct file *flip, unsigned int cmd, unsigned long arg)
{
	int cur_baud = 115200;
	struct mystatistics user = {
				.Baud	= 9600,
				.nBits	= 8,
				.nr	= 0,
				.wr	= 0,
	};
	printk(KERN_DEBUG"--> in char driver ioctl \n");
	switch (cmd) {
		case FIOP :
				printk(KERN_DEBUG"character ioctl VECIOP\n");
				break;
		case FIOGETCONFIG:
				printk(KERN_DEBUG"char driver ioctl VECIOGET\n");
				put_user(cur_baud,(struct mystatistics *)arg);
				break;
		case FIOSETCONFIG:
				printk(KERN_DEBUG"--> setting baud rate\n");
				get_user((struct mystatistics*)data, arg); 
				break;
		case FIOCLEAR:
				printk(KERN_DEBUG"--> clear buffer\n");
				memset(user.buff,'\0',10);
		default:
				return -EFAULT;
				break;
	}
}
/* structure for file operation */
static const struct file_operations fops = {
	.owner	= THIS_MODULE,
	.open	= dip_open,
	.release= dip_release,
	.read	= dip_read,
	.write	= dip_write,
	.unlocked_ioctl = dip_ioctl
};
/* structure for driver registration */
struct cdev dip_cdev;
	/*initialization section */
static int __init driver_init(void)
{
	int major;
	int reg;
	reg = alloc_chrdev_region(&device_num, minor, 4, "dip_driver");
	if (reg < 0) {
		printk(KERN_DEBUG" device number allocation failure\n");
		return -EFAULT;
	}
	major = MAJOR(device_num);
	printk(KERN_DEBUG"--> major device number=%d\n", major);
	dip_cdev->ops = &fops;
	reg = cdev_add(dip_cdev, device_num, 4);
	if (reg < 0) {
		printk(KERN_DEBUG"device registration failure\n");
		return -EFAULT;
	}
	printk(KERN_DEBUG"driver init section\n");
	return 0;
}
/* driver exit section */
static void __exit driver_exit(void)
{	/*unregitering driver */
	unregister_chrdev_region(device_num, 4);
	/*deleting structure */
	cdev_del(dip_cdev);
	printk(KERN_DEBUG"module exit\n");
}

module_init(driver_init);
module_exit(driver_exit);
