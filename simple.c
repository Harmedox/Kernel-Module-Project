// insert libraries

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/hashtable.h>
#include <linux/slab.h>

//number of bits defining bucket size
#define BITS 3

//software license information
MODULE_LICENSE("GPL");
//module descripton
MODULE_DESCRIPTION("Kernel Module - Project");
//Module Author
MODULE_AUTHOR("AAA");

/* Define structure for object 'birthday' */
struct Birthday {
	char name[100];
	int day;
	int month;
	int year;
	struct hlist_node birthday_hash_list;
};

//create hash table 
DEFINE_HASHTABLE(birthday_hash, BITS);

//define module entry point
int simple_init(void)
{

	struct Birthday *aNewBirthday, *aBirthday;
	unsigned int i;

	//output message to kernel log buffer. to be read by 'dmesg' command
	printk(KERN_INFO "Loading Module\n");

	/*adding elements to the birthday data structure*/
	for(i=1; i<=8; ++i){
		aNewBirthday = kmalloc(sizeof(*aNewBirthday), GFP_KERNEL);
		//memory availability check
		if(!aNewBirthday){
			printk("cannot allocate memory");
			return -ENOMEM;
		}

		sprintf(aNewBirthday->name, "user_%d", i);
		aNewBirthday->day = 2*i;
		aNewBirthday->month = i;
		aNewBirthday->year = 2000 + i;

		//add item to the birthday hash
		hash_add(birthday_hash, &aNewBirthday->birthday_hash_list, aNewBirthday->name);
	}
	
	printk(KERN_INFO "Traversing birthday hash \n");
	hash_for_each(birthday_hash, i, aBirthday, birthday_hash_list) {
		//print entries of the hash table, each record as object aBirthday
		printk(KERN_INFO "Entry[%d]: Name: %s, Day: %d, Month: %d, Year: %d\n", i,aBirthday->name,aBirthday->day,aBirthday->month,aBirthday->year);
	}

	printk(KERN_INFO "\n");

	return 0;
}

//define module exit point
void simple_exit(void)
{
	struct Birthday *aBirthday;
	unsigned int j; //node pointer

	printk(KERN_INFO "Removing Module\n");

	printk(KERN_INFO "delete nodes in the birthday hash table");

	hash_for_each(birthday_hash, j, aBirthday, birthday_hash_list){
		//delete each node of birthday hash table
		printk(KERN_INFO "Entry[%d]: deleted", j);
		hash_del(&aBirthday->birthday_hash_list);
		kfree(aBirthday);
	}
	printk(KERN_INFO "\n");
}

//register module entry and exit points with kernel
module_init(simple_init);
module_exit(simple_exit);


