#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("gmate.amit@gmail.com");
MODULE_VERSION("1.1");

static int __init hello_init(void)
{
	pr_debug("Hello World!\n");
	return 0;
}

static void hello_exit(void)
{
	pr_debug("Goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);
