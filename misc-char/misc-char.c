#include <linux/module.h>
#include <linux/init.h>
#include <linux/major.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/miscdevice.h>

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("gmate.amit@gmail.com");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("misc char driver");
#define DRV_NAME "misc-char-lkm"

static const char text[] = "bitprolix";
static int len = sizeof(text);

static ssize_t misc-char-lkm_read(struct file *filp, char __user *buf,
		size_t count, loff_t *f_ops)
{
	/* file offset past the text */
	if (*f_ops > len)
		return 0;

	/* Read as much we have */
	if (*f_ops + count > len)
		count = len - *f_ops;

	if (copy_to_user(buf, text + *f_ops, count)) {
		pr_debug("Failed to read from the char driver\n");
		return -EFAULT;
	}

	*f_ops += count;

	return count;
}

static ssize_t misc-char-lkm_write(struct file *filp, const char __user *buf,
		size_t count, loff_t *f_ops)
{
	int max, bytes_to_write;

	char *kbuff = kmalloc(len, GFP_KERNEL);
	if (!kbuff) {
		pr_debug("Failed to allocate memory\n");
		return -ENOMEM;
	}
	max = len - *f_ops;

	if (max > count)
		bytes_to_write = count;
	else
		bytes_to_write = max;

	if (copy_from_user(kbuff + *f_ops, buf, bytes_to_write)) {
		pr_debug("Failed to write to the char driver\n");
		kfree(kbuff);
		return -EFAULT;
	}

	*f_ops += bytes_to_write;
	if (memcmp(kbuff, text, len - 1) != 0) {
		pr_debug("Invalid value\n");
		kfree(kbuff);
		return -EINVAL;
	}

	kfree(kbuff);
	return count;
}

static const struct file_operations misc-char-lkm_fops = {
	.read = misc-char-lkm_read,
	.write = misc-char-lkm_write,
};

static struct miscdevice misc-char-lkm_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DRV_NAME,
	.fops = &misc-char-lkm_fops
};

static int __init misc-char-lkm_init(void)
{
	int ret;

	ret = misc_register(&misc-char-lkm_misc_device);
	if (ret) {
		pr_debug("Failed to register misc-char-lkm char driver\n");
		return ret;
	}

	pr_debug("Registered /dev/%s with major: %d and minor: %d\n",
		DRV_NAME, MISC_MAJOR, misc-char-lkm_misc_device.minor);

	return 0;
}

static void misc-char-lkm_exit(void)
{
	misc_deregister(&misc-char-lkm_misc_device);
	pr_debug("Removed /dev/%s file node!!!\n", DRV_NAME);
}

module_init(misc-char-lkm_init);
module_exit(misc-char-lkm_exit);
