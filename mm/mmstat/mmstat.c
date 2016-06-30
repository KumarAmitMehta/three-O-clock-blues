#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>

/*
 * @brief 	Get memory map of process address space
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("gmate.amit@gmail.com");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("Example on process address space");

#define MAX_SZ		(PAGE_SIZE * 10)

static uint32_t *p = NULL;

static int __init hello_init(void)
{
	unsigned long addr;
	int i;
	pr_debug("Allocating virtually contiguous memory!\n");

	p = vmalloc(MAX_SZ);
	if (!p) {
		pr_debug("Not enough memory");
		return -ENOMEM;
	}
	
	pr_debug("---process memory map---\n");

	pr_debug("virt: start_code: 0x%lx, end_code: 0x%lx\n",current->mm->start_code, 
		current->mm->end_code);
	pr_debug("virt: start_data: 0x%lx, end_data: 0x%lx\n",current->mm->start_data, 
		current->mm->end_data);
	pr_debug("virt: start_heap: 0x%lx, end_heap: 0x%lx\n",current->mm->start_brk, 
		current->mm->brk);
	pr_debug("phy: number of pages: %d\n", current->mm->map_count);
	for (addr = current->mm->mmap->vm_start, i = 0; addr <= current->mm->mmap->vm_end; 
		addr += PAGE_SIZE, ++i) {
		pr_debug("vma: vpn[%d]: 0x%lx, pfn[%d]: 0x%lx\n", i, addr, i, __pa(addr));
	}
	pr_debug("---process memory map---\n");
	
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
