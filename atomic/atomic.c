#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("gmate.amit@gmail.com");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("Trivial atomic counter");

atomic_t counter  = ATOMIC_INIT(0);

static void inc_count(void)
{
	atomic_inc(&counter);
}

static long read_count(void)
{
	return atomic_read(&counter);
}

static int __init hello_init(void)
{
	int i;
	pr_debug("Start counting...!\n");

	for (i = 0; i < 10; ++i) 
		inc_count();

	return 0;
}

static void hello_exit(void)
{
	pr_debug("counter value at exit: %ld\n", read_count());
}

module_init(hello_init);
module_exit(hello_exit);
