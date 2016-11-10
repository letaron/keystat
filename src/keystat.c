#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

static int count = 0;

static int keystat_show(struct seq_file *m, void *v)
{
	seq_printf(m, "%d\n", ++count);
	return 0;
}

static int keystat_open(struct inode *inode, struct file *file)
{
	return single_open(file, keystat_show, NULL);
}

static const struct file_operations keystat_fops = {
	.owner      = THIS_MODULE,
	.open       = keystat_open,
	.read       = seq_read,
	.llseek     = seq_lseek,
	.release    = single_release,
};

static int __init keystat_init(void)
{
	printk(KERN_INFO "Loading keystat module, count = %d.\n", count);
	proc_create("keystat", 0, NULL, &keystat_fops);
	return 0;
}

static void __exit keystat_exit(void)
{
	remove_proc_entry("keystat", NULL);
	printk(KERN_INFO "Unloading keystat module.\n");
}

module_init(keystat_init);
module_exit(keystat_exit);

MODULE_LICENSE("GPL");
