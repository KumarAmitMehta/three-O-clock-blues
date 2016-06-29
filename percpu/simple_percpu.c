#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/percpu.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("gmate.amit@gmail.com");
MODULE_DESCRIPTION("An example on per-cpu variable");

static DEFINE_PER_CPU(long, counter);

void inc_count(void)
{
	get_cpu_var(counter)++;
	put_cpu_var(counter);
}

long read_count(void)
{
	long result = get_cpu_var(counter);
	return result;
}

static int __init my_init(void)
{
	int i;
	for (i = 0; i < 6000000; ++i)
		inc_count();

	pr_debug("counter value after incrementing: %ld\n", read_count());
	return 0;
}

static void __exit my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);
