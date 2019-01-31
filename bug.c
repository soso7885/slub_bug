#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>

enum {
	BUG_OVERFLOW_REDZONE = 0,
	BUG_OVERFLOW_PADDING,
	BUG_DOUBLE_FREE,
	BUG_ACCESS_FREE,
};

static unsigned char *buf;
static int mode;

static void _bug_red(void)
{
	memset(buf, 0x12, 128);
}

static void _bug_pad(void)
{
	memset(buf-10, 0x12, 8);
}

static void _bug_double_free(void)
{
	memset(buf, 0x12, 32);
	kfree(buf);
	kfree(buf);
}

static void _bug_freed(void)
{
	memset(buf, 0x12, 32);
	kfree(buf);
	memset(buf, 0x12, 32);
}

static int __init slub_bug_init(void)
{
	pr_info("===== Slub BUG init =====\n");

	buf = kmalloc(32, GFP_KERNEL);
	if (!buf) {
		pr_err("Error, kmalloc failed!\n");
		return -ENOMEM;
	}

	switch (mode) {
	case BUG_OVERFLOW_REDZONE:
		pr_info("Create Slub BUG: Redzone overwritten...\n");
		_bug_red();
		break;
	case BUG_OVERFLOW_PADDING:
		pr_info("Create Slub BUG: Object padding overwritten...\n");
		_bug_pad();
		break;
	case BUG_DOUBLE_FREE:
		pr_info("Create Slub BUG: Duoble free...\n");
		_bug_double_free();
		break;
	case BUG_ACCESS_FREE:
		pr_info("Create Slub BUG: Access freed memory...\n");
		_bug_freed();
		break;
	default:
		pr_err("%s: Should not go here!\n", __func__);
	}

	return 0;
}

static void __exit slub_bug_exit(void)
{
	pr_info("===== Slub BUG exit =====\n");
	if (mode < BUG_OVERFLOW_PADDING)
		kfree(buf);
}

module_init(slub_bug_init);
module_exit(slub_bug_exit);
module_param(mode, int, S_IRUGO);

MODULE_LICENSE("GPL");
MODULE_PARM_DESC(mode,"to switch slub bug mode");

