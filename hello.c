// SPDX-License-Identifier: Dual BSD/GPL
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Rubanyk Hlib <forkreros228@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel[MODULE]");
MODULE_LICENSE("Dual BSD/GPL");

static uint COUNTER = 1;

// 0444 = S_IRUGO - flag for read-only parameter access
module_param(COUNTER, uint, 0444);
MODULE_PARM_DESC(COUNTER, "Counter value");

struct myStruct {
	struct list_head list;
	ktime_t myTime;
};

// Static list head declaration
static LIST_HEAD(myList);
static int counter_param;

static int __init hello_init(void)
{
	if (COUNTER > 10) {
		pr_err("Error: COUNTER CANNOT BE GREATER THAN 10\n");
		return -EINVAL;
	}

	if (COUNTER == 0 || (COUNTER <= 10 && COUNTER >= 5))
		pr_emerg("!!!WARNING!!!: %u\n", COUNTER);

	counter_param = 0;
	while (counter_param != COUNTER) {
		struct myStruct *ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);

		if (!ptr) {
			pr_err("!!!ERROR!!!: Memory allocation trouble\n");
			return -ENOMEM;
		}

		ptr->myTime = ktime_get();
		list_add_tail(&ptr->list, &myList);
		pr_emerg("HELLO, WORLD!\n");
		counter_param++;
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct myStruct *ptr, *next;

	list_for_each_entry_safe(ptr, next, &myList, list) {
		pr_emerg("Freeing node at time: %lld\n", ptr->myTime);
		list_del(&ptr->list);
		kfree(ptr);
	}
}

module_init(hello_init);
module_exit(hello_exit);
