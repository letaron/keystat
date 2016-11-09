#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/interrupt.h> // included for free_irq

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Le Taron");
MODULE_DESCRIPTION("A keystroke stats counter");

static int __init hello_init(void)
{
    printk(KERN_INFO "Keystat counter enabled!\n");
    free_irq(1, NULL);
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit hello_cleanup(void)
{
    printk(KERN_INFO "Keystat module unloaded.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);
