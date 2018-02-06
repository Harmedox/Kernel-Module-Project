# Kernel-Module-Project
Kernel Module Project

First commit: 
	1. Load and remove sample module, displaying notification on kernel log buffer.

Second commit: 
	1. Define the structure of the "birthday object"
	2. Edit the module entry point to create a list of 5 birthday objects
	3. Traverse through the list to print each object record on kernel log buffer.
	4. At the module exit module, remove each birthday object from the list

Third commit:
	1. Define birthday with struct hlist_node to be abke to link each element of struct birthday to others.
	2. Define the hashtable, with a name and number of bits from which bucket will be formed.
	3. Perform a check of birthday object creation.
	4. At the creation of each birthday object, add it to the hash table with 'name' as key.
	5. At the module exit point, delete each node in the hash table. Note this is not the same as object as more than one birthday object can be placed in a single hash table node.
