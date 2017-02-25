/*
 * Trivial example on fifo implementation in Linux kernel 
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kfifo.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Kumar Amit Mehta");
MODULE_DESCRIPTION("Exercise FIFO implementation in kernel");

struct identity {
	char name[20];
	int id;
	bool busy;
	struct list_head id_list;
};

static struct kfifo superhero_queue;

static int superhero_enqueue(char *name, int id)
{
	struct identity p;

	strcpy(p.name, name);
	p.id = id;
	p.busy = false;

	kfifo_in(&superhero_queue, &p, sizeof(struct identity));
	pr_debug("I'm %s with super id(%d)", p.name, p.id);

	return 0;
}

static int superhero_dequeue(void)
{
	struct identity p;
	int ret;

	while (kfifo_avail(&superhero_queue)) {
		/* read one super character at a time */
		ret = kfifo_out(&superhero_queue, &p, sizeof(p));
		if (ret != sizeof(p))
			return -EINVAL;

		pr_debug("Goodby from %s", p.name);
	}

	return 0;
}

static int my_init(void)
{
	int ret;

	ret = kfifo_alloc(&superhero_queue, PAGE_SIZE, GFP_KERNEL);
	if (ret) {
		pr_debug("Failed to create superhero queue");
		return ret;
	}

	superhero_enqueue("Superman", 1);
	superhero_enqueue("Spiderman", 2);
	superhero_enqueue("Batman", 3);
	superhero_enqueue("Sherlock", 10);

	return ret;
}

static void my_exit(void)
{
	superhero_dequeue();
	kfifo_free(&superhero_queue);
}

module_init(my_init);
module_exit(my_exit);
