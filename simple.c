#include <linux/module.h>    // for all kernel modules
#include <linux/kernel.h>    // for KERN_INFO
#include <linux/init.h>      // for __init and __exit macros
#include <linux/list.h>		// for list operations
#include <linux/types.h>	// for hlist and list 
#include <linux/slab.h>
  
// given birthday struct
struct birthday
{
	int day;
	int month;
	int year;
	char name[100];
	struct hlist_node birthday_hash_list;
};

struct h_table
{
	struct hlist_head *head_list;
	unsigned int size;
};

static struct h_table birth_day_table;

// traverse all elements in hash table
static void traverse_hash_table(void) {
	unsigned int i;
	struct hlist_node *p;
	for (i = 0; i < birth_day_table.size; i++) {
		hlist_for_each(p, &(birth_day_table.head_list[i])) {
    	struct birthday *birth_day = hlist_entry(p, struct birthday, birthday_hash_list);
    	printk("Entry[%d]: Name: %s, Day: %d, Month: %d, Year: %d\n",
    		i, birth_day->name, birth_day->day, birth_day->month, birth_day->year);

		}
	}
}
// hash function for hash index - using lose lose hashing algorithm
unsigned int hash_func(const char *str)
{
	unsigned long hash = 0;
	int c;
	while ( (c = *str++ ))
	    hash += c;

	return hash % birth_day_table.size;
}

// get birth_day element by name 
struct birthday *get_birth_day_by_name(const char *name) {
	struct hlist_node *p;
	unsigned int hash_index = hash_func(name);
	hlist_for_each(p, &(birth_day_table.head_list[hash_index])) 
	{
		struct birthday *birth_day = hlist_entry(p, struct birthday, birthday_hash_list);
		if (!strncmp(birth_day->name, name, 101))
			return birth_day;  
	}
	return NULL;
}

// Initialize a new hash table
static void init_new_table(unsigned int n_birth_days)
{  	
	unsigned int i;
	struct hlist_head *heads;
	heads = kmalloc(sizeof(*heads) * n_birth_days, GFP_KERNEL);
  	if (heads != NULL)
    	for (i = 0; i < n_birth_days; i++)
      		INIT_HLIST_HEAD(&heads[i]);
    birth_day_table.head_list = heads;
    birth_day_table.size = n_birth_days;
}


static void hash_add(struct birthday birth_day)
{
	unsigned int hash_index = hash_func(birth_day.name);
	hlist_add_head(&birth_day.birthday_hash_list, &(birth_day_table.head_list[hash_index]));
}

// Kernel Module initializer.
static int __init simple_init(void)
{
	const unsigned int num_birth_days = 5;

	struct birthday birth_day1 = {
	    .day = 1,
	    .month = 1,
	    .year = 1990,
	    .name = "abc xyz",
	};
	struct birthday birth_day2 = {
	    .day = 2,
	    .month = 2,
	    .year = 1990,
	    .name = "def xyz",
	};
	struct birthday birth_day3 = { 
	    .day = 3,
	    .month = 3,
	    .year = 1990,
	    .name = "ghi xyz",
	};
	struct birthday birth_day4 = {
	    .day = 4,
	    .month = 4,
	    .year = 1990,
	    .name = "jkl xyz",
	};
	struct birthday birth_day5 = {
	    .day = 5,
	    .month = 5,
	    .year = 1990,
	    .name = "mno xyz",
	};

	struct birthday *tmp;

    printk(KERN_INFO "Starting birthday hash table module!\n\n");
    init_new_table(num_birth_days);

    printk(KERN_INFO "Adding 5 birthday objects to hash table.\n\n");

	hash_add(birth_day1);
	hash_add(birth_day2);
	hash_add(birth_day3);
	hash_add(birth_day4);
	hash_add(birth_day5);

	printk(KERN_INFO "Traversing all elements in the hash table.\n\n");
	traverse_hash_table();

	tmp = get_birth_day_by_name("Alfredo Past");
	  if (tmp) {
	    printk("%s, %d\n", tmp->name, tmp->day);
	}
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

// Kernel Module destructor.
static void __exit simple_exit(void)
{
	printk(KERN_INFO "Removing Module\n");
	kfree(birth_day_table.head_list);
}

module_init(simple_init);
module_exit(simple_exit);
