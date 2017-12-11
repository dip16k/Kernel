#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DIP");

int mul(int a,int b)
{
	printk( KERN_DEBUG " --> in multiplication function\n");
	printk( KERN_DEBUG " a=%d  b=%d \n",a,b);
	return a*b;
}

static int __init mul_module_init(void)
{
	printk( KERN_DEBUG " in mul_init module\n");
	return 0;
}

static void __exit mul_module_exit(void)
{
	printk( KERN_DEBUG " mul_exit module\n");
}
/* export multiplication function so other module can use it*/
EXPORT_SYMBOL(mul);

module_init(mul_module_init);
module_exit(mul_module_exit);
