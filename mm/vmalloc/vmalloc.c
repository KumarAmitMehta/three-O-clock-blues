#include <linux/module.h>
#include <linux/init.h>
#include <linux/vmalloc.h>

/*
 * @brief 	Test virtually contiguous page allocation routine
 * 
 * @details	Use vmalloc kernel facility to allocate multiple pages
 * 		and release it during the module unload. Observe /proc/meminfo
 * 		for change in memory stats.
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("gmate.amit@gmail.com");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("excercise vmalloc");

#define MAX_SZ		(PAGE_SIZE * 10)

static uint32_t *p = NULL;

static int __init hello_init(void)
{
	pr_debug("Allocating virtually contiguous memory!\n");

	p = vmalloc(MAX_SZ);
	if (!p) {
		pr_debug("Not enough memory");
		return -ENOMEM;
	}
	
	pr_debug("physical address: 0x%8.8lx\n", __pa(p));
	
	return 0;
}

static void hello_exit(void)
{
	pr_debug("Releasing memory, size(%ld)\n", MAX_SZ);
	if (p)
		vfree(p);
}

module_init(hello_init);
module_exit(hello_exit);
