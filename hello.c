#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>

MODULE_AUTHOR("Dima Bryzgalov <Bryzgalov174@ukr.net>");
MODULE_DESCRIPTION("Module for lab 3");
MODULE_LICENSE("Dual BSD/GPL");


static uint cnt = 1;
MODULE_PARM_DESC(cnt,"this parameter shows us, how many times Hello world! will be outputted");
module_param(cnt, uint, S_IRUGO);

struct list {
	struct list_head l;
	ktime_t t;
} *ptr;
static LIST_HEAD(head);


static int __init hello_init(void)
{
	/*init function*/
	
	if( (cnt == 0) || (cnt > 4 && cnt < 11) ) {
		printk(KERN_WARNING "Warning!\n");
		
		int i = 0;
		for(i = 0;i < cnt;i++){
			ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);
			ptr->t = ktime_get();
			list_add_tail(&ptr->l,&head);
			printk(KERN_EMERG "Hello, world!\n");
		}
		
}
	if (cnt >= 11) {
		printk(KERN_ERR "Error! cnt must be <=10");
		return -EINVAL;
}
	return 0;
}

static void __exit hello_exit(void)
{
	struct list *cur, *next;
	list_for_each_entry_safe(cur, next, &head, l) {
	
	long long Event_time = ktime_to_ns(cur->t);
	printk(KERN_INFO "Event time: %lld\n", Event_time);
	list_del(&cur->l);
	kfree(cur);

	}
	BUG_ON(!list_empty(&head));

}

module_init(hello_init);
module_exit(hello_exit);
