#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
/* here modulparam.h is used for EXPORT_SYMBOL*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DIP");

int add(int a,int b)
{
	printk( KERN_ALERT " in addtion function-->\n");
	printk( KERN_DEBUG " a=%d b=%d\n",a,b);
	return a+b;
}
static int __init add_module_init(void)
{
	printk( KERN_DEBUG " in initialization section of add module ##\n");
	return 0;
}

static void __exit add_module_exit(void)
{
	printk( KERN_DEBUG " module exiting \n");
}

EXPORT_SYMBOL(add);

module_init(add_module_init);
module_exit(add_module_exit);

