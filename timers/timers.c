/*
 * file: timers.c
 * brief: Sample program demonstrating, generating a small
 * delay. Delay value need to be carefully chosen, otherwise
 * a soft lockup event will be triggered.
 */

#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("gmate.amit@gmail.com");
MODULE_VERSION("1.1");

static void delay(int sec)
{
	unsigned long timeout = jiffies + (sec * HZ);
	while (time_before(jiffies, timeout))
		cpu_relax();
}

static int __init hello_init(void)
{
	/*
	 * cat /proc/sys/kernel/watchdog_thresh = 10
	 * Any value greater than watchdog threshold value causes
	 * soft lockup.
	 * 
	 * TODO: read the watchdog threshold value from /proc
	 * interface and pass it as a module load time parameter
	 */
	int delay_sec = 9;

	pr_debug("ticks since system reboot: %ld\n", jiffies);
	delay(delay_sec);
	pr_debug("jiffies after 30 seconds: %ld\n", jiffies);

	return 0;
}

static void hello_exit(void)
{
	pr_debug("Goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);
