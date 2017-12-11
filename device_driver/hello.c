#include<linux/init.h>
#include<linux/module.h>
#include<linux/jiffies.h>
#include<linux/time.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DILEEP");

static int __init hello_init(void)
{
	int onesec;
	unsigned long a,b;
	time_before(a,b);
	onesec=jiffies+HZ;
	printk(KERN_DEBUG" onesec=%d\n",onesec);
	printk(KERN_DEBUG" hello world\n");
	return 0;
}
static void __exit hello_exit(void)
{
	printk(KERN_DEBUG" bye bye cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);

