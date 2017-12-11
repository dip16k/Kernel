#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/usb.h>
#include<linux/fs.h>
#include<linux/uaccess.h>

#define MIN(a,b) (((a) <= (b)) ? (a) : (b))
#define BULK_OUT 0x02	//depend upon pendrive view in device file
#define BULK_IN 0x81
#define MAX_PKT_SIZE 512

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dileep singh Kushwaha");
MODULE_DESCRIPTION("USB PenDrive Device Driver");

static struct usb_device *device;
/* structure that controls an entire USB device*/
static struct usb_class_driver class;
/*USB driver wants to use the usb major number to communicate with user-space programs*/
static unsigned char bulk_buf[MAX_PKT_SIZE];

static int pen_open(struct inode* ino, struct file *fp)
{
	printk(KERN_DEBUG,"in open section-->>\n");
	return 0;
}
static int pen_close(struct inode *ino, struct file *fp)
{	
	printk(KERN_DEBUG,"in close section\n");
	return 0;
}
static ssize_t pen_read(struct file *fp,char __user *buf, size_t cnt, loff_t *off)
{
	int retval;
	int read_cnt;
	/*read the data from the bulk endpoint */
	retval = usb_bulk_msg(device, usb_rcvbulkpipe(device, BULK_IN), bulk_buf, MAX_PKT_SIZE, &read_cnt, 5000);
	if( retval ) {
		printk(KERN_ERR "bulk message returned %d\n", retval);
		return retval;
	}
	if( copy_to_user(buf, bulk_buf, MIN(cnt, read_cnt)) ) {
		return -EFAULT;
	}
	return MIN(cnt, read_cnt);
}
static ssize_t pen_write(struct file *fp, const char __user *buf, size_t cnt, loff_t *loff)
{
	int retval;
	int wrote_cnt = MIN(cnt, MAX_PKT_SIZE);
	if(copy_from_user( bulk_buf, buf, MIN(cnt, MAX_PKT_SIZE)) ) {
		return -EFAULT;
	}
	/* Write the data into the bulk endpoint */
	retval = usb_bulk_msg(device, usb_sndbulkpipe(device, BULK_OUT), bulk_buf, MIN(cnt, MAX_PKT_SIZE), &wrote_cnt, 5000);
	if( retval ) {
		printk(KERN_ERR "bulk message returned %d\n", retval);
		return retval;
	}
	return wrote_cnt;
}

static struct file_operations fops = {
		.owner		= THIS_MODULE,
		.open		= pen_open,
		.release	= pen_close,
		.read		= pen_read,
		.write		= pen_writem
};

static int pen_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	int retval;
	device = interface_to_usbdev(interface);
	class.name = "usb/pen%d";/* Dynamic cration of /dev/pen0. */
	class.fops = &fops;
	if((retval = usb_register_dev(interface, &class)) < 0)
	{
		printk(KERN_ERR "Not able to get a minor for this device .");
		return -EFAULT;
	}
	else {
		printk(KERN_INFO "minor obtained: %d\n",interface->minor);
	}
	return retval;
}
	
/* table of devices that work with this driver */
static struct usb_device_id pen_table[] = {
	{ USB_DEVICE(0x03f0/*vendor ID*/,0x5607/*product ID*/) },
	{ }/*terminating entery */
};
MODULE_DEVICE_TABLE (usb, pen_table);
/*enables auto loading of this driver*/

static void pen_disconnet( struct usb_interface *interface)
{
	usb_deregister_dev(interface, &class);
}

/* driver operation */
static struct usb_driver pen_driver = {
		.name		= "pen_driver",
		.probe		= pen_probe,
		.disconnect	= pen_disconnet,
		.id_table	= pen_table,
};

static int __init pen_init(void)
{
	int result;
	/*register this driver with the USB subsystem*/
	if((result = usb_register( &open_driver )))
	{
		printk(KERN_ERR "USB_register failed. ERRoR number %d\n", result);
		return -EFAULT;
	}
	return result;
}
static void __exit pen_exit(void)
{
	/*deregister  this driver with the USB subsystem */
	usb_deregister( &pen_driver );
}
module_init(pen_int);
module_exit(pen_exit);
