/*
 * Trivial example, showing usage of data structure, more specifically
 * link-list implementation mechanism provided by Linux kernel.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Kumar Amit Mehta");

struct identity {
	char name[20];
	int id;
	bool busy;
	struct list_head id_list;
};

static struct list_head superhero_list;

static int superhero_create(char *name, int id)
{
	struct identity *p;

	p = kmalloc(sizeof(struct identity), GFP_KERNEL);
	if (!p)
		return -ENOMEM;

	strcpy(p->name, name);
	p->id = id;
	p->busy = false;

	list_add(&p->id_list, &superhero_list);
	return 0;
}

static struct identity *superhero_find(int id)
{
	struct identity *p;

	if (!list_empty(&superhero_list)) {
		list_for_each_entry(p, &superhero_list, id_list) {
			if (p->id == id)
				return p;
		}
	}

	return NULL;
}

static void superhero_destroy(int id)
{
	struct identity *p, *q;

	if (!list_empty(&superhero_list)) {
		list_for_each_entry_safe(p, q, &superhero_list, id_list) {
			if (p->id == id) {
				list_del(&p->id_list);
				kfree(p);
				break;
			}
		}
	}
}

static int my_init(void)
{
	struct identity *temp;

	INIT_LIST_HEAD(&superhero_list);

	superhero_create("Superman", 1);
	superhero_create("Spiderman", 2);
	superhero_create("Batman", 3);
	superhero_create("Sherlock", 10);

	/* Spiderman, are you still there? */
	temp = superhero_find(2);
	pr_debug("Superhero #2 is %s\n", temp->name);

	temp = superhero_find(42);
	if (temp == NULL)
		pr_debug("Superhero #42 not found...\n");

	/* Aah! Me the mIghty */
	superhero_destroy(2);
	superhero_destroy(1);
	superhero_destroy(3);
	superhero_destroy(10);

	/* Negative test */
	superhero_destroy(42);
	superhero_destroy(10);

	return 0;
}

static void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);
