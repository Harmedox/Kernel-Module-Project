// insert libraries

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>


//define module entry point
int simple_init(void)
{
	//output message to kernel log buffer. to be read by 'dmesg' command
	printk(KERN_INFO "Loading Module\n");

	return 0;
}

//define module exit point
void simple_exit(void)
{
	printk(KERN_INFO "Removing Module\n");
}

//register module entry and exit points with kernel
module_init(simple_init);
module_exit(simple_exit);

//software license information
MODULE_LICENSE("GPL");
//module descripton
MODULE_DESCRIPTION("Simple Module - Project");
//Module Author
MODULE_AUTHOR("AAA");
