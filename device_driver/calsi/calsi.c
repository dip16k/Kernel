#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DIP");

static int a;
static int b;

module_param(a,int,S_IRUGO);
module_param(b,int,S_IRUGO);
/* function prototype declaration*/
int add(int,int);
int mul(int,int);

static int __init calsi_init(void)
{
	int z;
	printk( KERN_DEBUG " calsi module init \n");
	z=add(a,b);
	printk( KERN_DEBUG " add result: %d\n",z);
	z=mul(a,b);
	printk( KERN_DEBUG " mul result: %d\n",z);
	return 0;
}

static void __exit calsi_exit(void)
{
	printk( KERN_DEBUG " #calsi module exit \n");
}

module_init(calsi_init);
module_exit(calsi_exit);

