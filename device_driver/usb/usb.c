#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kdev_t.h>
#include <linux/usb.h>
#include <linux/uaccess.h>

#define BULK_IN 0x81
#define BULK_OUT 0x02
#define MAX_PKT_SIZE 512

/* MODULE PARAMETER */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DIP");
MODULE_DESCRIPTION("MY USB DRIVER");
/* structure for automatic device driver creation */
static struct usb_device *device;	
static struct usb_class_driver class;	/* automatic node creation*/
static unsigned char bulk_buf[MAX_PKT_SIZE];	/*data receiving from the device */

/* open function */
int usb_open( struct inode *ino, struct file *flip)
{
	printk(KERN_DEBUG"in usb open function\n");
	return 0;
}
int usb_release(struct inode *ino, struct file *flip)
{
	printk(KERN_DEBUG"in usb close function\n");
	return 0;
}
ssize_t usb_read(struct file *flip, char __user *ubuff, size_t len, loff_t *lof)
{	/*copy_to_user */
	int ret;
	int read_count;
	/*read the data from the bulk point */
	ret = usb_bulk_msg(device, usb_rcvbulkpipe(device, BULK_IN), bulk_buf, MAX_PKT_SIZE, &read_count, 5000/*time*/);
	if ( ret ) {
		printk(KERN_DEBUG "bulk message returned %d\n",ret);
		return ret;
	}
	ret = copy_to_user((char*)ubuff, (char*)bulk_buf, read_count);
	if (ret < 0) {
		printk(KERN_DEBUG"copy failure\n");
		return -EFAULT;
	}
	printk(KERN_DEBUG"in usb read section\n");
	return 0;
}
ssize_t usb_write(struct file *flip, const char __user *ubuff, size_t len, loff_t *lof)
{	/* copy_from_user */
	int write_count =  MAX_PKT_SIZE;
	int ret;
	printk(KERN_DEBUG"in usb write section\n");
	ret = copy_from_user((char*)bulk_buf, (char*)ubuff, MAX_PKT_SIZE);
	if(ret < 0) {
		printk(KERN_DEBUG"write section failure\n");
		return -EFAULT;
	}
	/* write the data into bulk endpoint */
	ret = usb_bulk_msg(device, usb_sndbulkpipe(device, BULK_OUT), bulk_buf,  MAX_PKT_SIZE, &write_count, 5000);
	if( ret ) {
		printk(KERN_DEBUG "bulk write message returned=%d\n",ret);
		return ret;
	}
	return 0;
}
struct file_operations fops = {
		.owner  	= THIS_MODULE,
		.open   	= usb_open,
		.release        = usb_release,
		.read		= usb_read,
		.write		= usb_write,
	};
/* USB device Id */
static struct usb_device_id usb_table[] = {
		{ USB_DEVICE(0x03f0/*vendoId*/,0x5607/*producId*/) },
		{ }
};
/* usb , usb array*/
MODULE_DEVICE_TABLE(usb,usb_table);

static int usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	int ret;
	device = interface_to_usbdev(interface);
	class.name = "MyUSB%d";/*auto node creation */
	class.fops = &fops;
	ret = usb_register_dev(interface, &class);
	if (ret < 0) {
		printk(KERN_DEBUG"usb_register_dev failure\n");
	}
	else {
		printk(KERN_DEBUG "minor number=%d\n", interface->minor);
	}
	printk(KERN_DEBUG "-->> in probe function\n");	
	printk(KERN_DEBUG "-->> vendoId=%d productId=%d\n",id->idVendor,id->idProduct);
	return 0;
}
static void usb_disconnect(struct usb_interface *interface)
{
	usb_deregister_dev(interface, &class);
	printk(KERN_DEBUG "--> device disconnected\n");
}
/* structure for connection management */
static struct usb_driver my_driver = {
		.name 	  = "my_USB_driver",
		.id_table = usb_table,
		.probe	  = usb_probe,
		.disconnect= usb_disconnect,
	}; 

/* module initializaton */
static int __init usb_init(void)
{
	int ret;
	/* registering our driver with the USB subsystem*/
	ret = usb_register(&my_driver);
	if (ret < 0) {
		printk(KERN_DEBUG "usb registration failure\n");
		return -EFAULT;
	}
	printk(KERN_DEBUG "## USB init ## section\n");
	return 0;
}
static void __exit usb_exit(void)
{
	/* Deregister this driver with the USB subsystem */
	usb_deregister(&my_driver);
	printk(KERN_DEBUG "## USB exit ## section\n");
}

/*loading and unloading module*/
module_init(usb_init);
module_exit(usb_exit);
