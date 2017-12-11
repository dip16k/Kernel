#include<linux/init.h>
#include<linux/module.h>
#include<linux/usb.h>
#include<linux/fs.h>		/* for file operation */
#include<linux/cdev.h>
#include<linux/kdev_t.h>
#include<linux/types.h>		/* size_t */
#include<linux/uaccess.h> 	/* copy to user /copy from user */
#include<asm/system.h>	/* cli(), _flags */
#include<linux/slab.h>		/* kmalloc () */
#include<asm/io.h>	/*inb , outb */
//#include<asm/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DIP");

dev_t device_num;

int port_open(struct inode *ino, struct file *fp)
{
	printk(KERN_DEBUG " port open section\n");
	return 0;
}
int port_release(struct inode *ino, struct file *fp)
{
	printk(KERN_DEBUG "port close section\n");
	return 0;
}
ssize_t port_read(struct file *fp, char __user *user_buffer, size_t len, loff_t lof)
{
	char kernel_buffer[100];
	int ret;
	printk(KERN_DEBUG "port read section \n");
	return 0
}
ssize_t port_write(struct file *fp, const char __user *user_buffer,size_t len, loff_t lof)
{
	char kernel_buffer[100];
	int ret;
	printk(KERN_DEBUG "port write section\n");
	return 0;
}
/* structure for read write operation on device */
struct file_operations fops = {
	.owner		= THIS_MODULE,
	.open		= port_open,
	.release	= port_close,
	.read		= port_read,
	.write		= port_write,
	};

/*module init and registration section */
static int __init port_init(void)
{
	int reg;
	printk(KERN_DEBUG "Module init section-->\n");
	reg = alloc_chrdev_region(&device_num/*major number*/, 10/*minor number*/, 1/*count*/,"ParPort"/*name*/);
	if( reg < 0) {
		printk(KERN_DEBUG "device registration failure\n");
	}
	printk(KERN_DEBUG "major number=%d, minor=10 count=1 --> ParPort -->\n",device_number); 
	reg = check_region(0x378/*parport addr*/, 1);
	if( reg < 0) {/* chaecking for avalabilty of port address */
		printk(KERN_DEBUG "parlelport: cannot be reserved 0x378\n");
	}
	request_region(0x378, 1 "ParPort");
	
	return 0;
}
static void __exit port_exit(void)
{	
	printk(KERN_DEBUG " MODULE EXIT SECTION\n");
	unregister_chrdev_region(device_num, 1/*count*/, "ParPort"/*name*/);
	release_region(0x378, 1);
}
module_init(port_init);
module_exit(port_exit);
