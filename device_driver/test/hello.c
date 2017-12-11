#include<linux/init.h>
#include<linux/module.h>
#include<linux/jiffies.h>
#include<linux/time.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DIP");

static int __init hello_init(void)
{
	unsigned long a,b,j,sec;
	int time;
	time = time_after(a,b);
	printk(KERN_DEBUG"time=%d\n",time);
	j=jiffies;
	sec=j+HZ;
	printk(KERN_DEBUG" HELLO WORLD sec=%d\n",sec);
	return 0;
}
static void __exit hello_exit(void)
{
	printk(KERN_DEBUG" BYE WORLD\n");
}

module_init(hello_init);
module_exit(hello_exit);

