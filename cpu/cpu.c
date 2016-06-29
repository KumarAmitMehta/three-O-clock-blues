#include <linux/module.h>
#include <linux/init.h>
#include <linux/smp.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("gmate.amit@gmail.com");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("excercise each cpu");

void cb(void *info)
{
	uint32_t cpuid;
	cpuid = smp_processor_id();
        pr_debug("cpu id: %d\n", cpuid);
}

static int __init hello_init(void)
{
	on_each_cpu(cb, NULL, 1);

	return 0;
}

static void hello_exit(void)
{
	pr_debug("Exiting...\n");
}

module_init(hello_init);
module_exit(hello_exit);
