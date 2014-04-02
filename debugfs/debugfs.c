#include <linux/module.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include <linux/semaphore.h>

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("gmate.amit@gmail.com");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("sample module showing debugfs usage");

#define RWUSR (S_IRUSR|S_IWUSR)
#define RWGRP (S_IRGRP|S_IWGRP)
#define RWOTH (S_IROTH|S_IWOTH)

/* Name of the directory entry in debugfs file system */
#define DEBUGFS_NAME "debugfs-lkm"
/* debugfs-lkm directory entry */
static struct dentry *dfs_eud_entry;

/* Debugfs debugfs-lkm/id file node read/write buffer */
static const char text[] = "bitprolix";
static int len = sizeof(text);

/* Debugfs debugfs-lkm/foo file node read/write buffer */
static char foo_kbuff[PAGE_SIZE];

/* For foo debugfs file node read/write synchronization */
static struct semaphore sem;

static ssize_t id_read(struct file *filp, char __user *buf,
		size_t count, loff_t *f_ops)
{
	/* file offset past the text */
	if (*f_ops > len)
		return 0;

	/* Read as much we have */
	if (*f_ops + count > len)
		count = len - *f_ops;

	if (copy_to_user(buf, text + *f_ops, count)) {
		pr_debug("Failed to read the student id\n");
		return -EFAULT;
	}

	*f_ops += count;

	return count;
}

static ssize_t id_write(struct file *filp, const char __user *buf,
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

static ssize_t jiffies_read(struct file *filp, char __user *buf,
		size_t count, loff_t *f_ops)
{
	pr_debug("%s: current value of jiffies: %lu\n", __FUNCTION__, jiffies);
	return 0;
}

static ssize_t foo_read(struct file *filp, char __user *buf,
		size_t count, loff_t *f_ops)
{
	if (down_interruptible(&sem))
		return -ERESTARTSYS;

	/* file offset past the text */
	if (*f_ops > PAGE_SIZE) {
		up(&sem);
		return 0;
	}

	/* Read as much we have */
	if (*f_ops + count > PAGE_SIZE)
		count = PAGE_SIZE - *f_ops;

	if (copy_to_user(buf, foo_kbuff + *f_ops, count)) {
		pr_debug("Failed to read from foo\n");
		up(&sem);
		return -EFAULT;
	}

	*f_ops += count;

	up(&sem);
	return count;
}

static ssize_t foo_write(struct file *filp, const char __user *buf,
		size_t count, loff_t *f_ops)
{
	if (down_interruptible(&sem))
		return -ERESTARTSYS;

	if (copy_from_user(foo_kbuff, buf, count)) {
		pr_debug("Failed to write to foo\n");
		up(&sem);
		return -EFAULT;
	}

	up(&sem);
	return count;
}

static const struct file_operations debugfs_eud_id_fs_ops = {
	.read = id_read,
	.write = id_write,
};

static const struct file_operations debugfs_eud_jiffies_fs_ops = {
	.read = jiffies_read,
};

static const struct file_operations debugfs_eud_foo_fs_ops = {
	.read = foo_read,
	.write = foo_write,
};

static int __init debugfs-lkm_debug_fs_init(void)
{
	pr_debug("%s Init\n", __FUNCTION__);

	dfs_eud_entry = debugfs_create_dir("debugfs-lkm", NULL);
	if (!dfs_eud_entry) {
		pr_debug("Failed to create subdirectory in debugFS\n");
		return -ENODEV;
	}

	/* initialize locking mechanism for foo */
	sema_init(&sem, 1);

	/* id */
	if (!debugfs_create_file("id", RWUSR|RWGRP|RWOTH,
				dfs_eud_entry, NULL, &debugfs_eud_id_fs_ops)) {
		pr_debug("Failed to create id node\n");
		return -ENODEV;
	}

	/* jiffies */
	if (!debugfs_create_file("jiffies", S_IROTH,
					dfs_eud_entry, NULL, 
					&debugfs_eud_jiffies_fs_ops)) {
		pr_debug("Failed to create jiffies node\n");
		return -ENODEV;
	}

	/* foo */
	if (!debugfs_create_file("foo", S_IWUSR|S_IRGRP|S_IROTH,
				dfs_eud_entry, NULL, 
				&debugfs_eud_foo_fs_ops)) {
		pr_debug("Failed to create foo node\n");
		return -ENODEV;
	}

	return 0;
}

static void debugfs-lkm_debug_fs_exit(void)
{
	debugfs_remove_recursive(dfs_eud_entry);

	pr_debug("%s: Exiting\n", __FUNCTION__);
}

module_init(debugfs-lkm_debug_fs_init);
module_exit(debugfs-lkm_debug_fs_exit);
