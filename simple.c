// insert libraries

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/slab.h>

//software license information
MODULE_LICENSE("GPL");
//module descripton
MODULE_DESCRIPTION("Simple Module - Project");
//Module Author
MODULE_AUTHOR("AAA");

/* Define structure for object 'birthday' */
struct Birthday {
	char name[100];
	int day;
	int month;
	int year;
	struct list_head list;
};

struct Birthday birthdayList;

//define module entry point
int simple_init(void)
{

	struct Birthday *aNewBirthday, *aBirthday;
	unsigned int i;

	//output message to kernel log buffer. to be read by 'dmesg' command
	printk(KERN_INFO "Loading Module\n");

	INIT_LIST_HEAD(&birthdayList.list);

	/*adding random elements to the birthday list */
	for(i=1; i<=5; ++i){
		aNewBirthday = kmalloc(sizeof(*aNewBirthday), GFP_KERNEL);
		sprintf(aNewBirthday->name, "user_%d", i);
		//strcpy(aNewBirthday->name, "user");
		aNewBirthday->day = 2*i;
		aNewBirthday->month = i;
		aNewBirthday->year = 2000 + i;
		INIT_LIST_HEAD(&aNewBirthday->list);
		//add item to the birthday list
		list_add_tail(&(aNewBirthday->list), &(birthdayList.list));
	}
	
	printk(KERN_INFO "Traversing birthday list \n");
	list_for_each_entry(aBirthday, &birthdayList.list, list) {
		//print entries of the list, each record as object aBirthday
		printk(KERN_INFO "Name: %s; Day: %d; Month: %d; Year: %d",aBirthday->name, aBirthday->day, aBirthday->month, aBirthday->year);
	}

	printk(KERN_INFO "\n");

	return 0;
}

//define module exit point
void simple_exit(void)
{
	struct Birthday *aBirthday, *tmp;
	printk(KERN_INFO "Removing Module\n");

	printk(KERN_INFO "delete records in the birthday list");

	list_for_each_entry_safe(aBirthday, tmp, &birthdayList.list, list){
	//delete each object of birthday record
	printk(KERN_INFO "node deleted for %s", aBirthday->name);
	list_del(&aBirthday->list);
	kfree(aBirthday);
	}
}

//register module entry and exit points with kernel
module_init(simple_init);
module_exit(simple_exit);


