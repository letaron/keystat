#include <linux/init.h>
#include <linux/ip.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/skbuff.h>
#include <linux/udp.h>

#define PORT 3615

static unsigned short int portnum = 0;

/* This function to be called by hook. */
static unsigned int hook_func(unsigned int hooknum, struct sk_buff *skb, const struct net_device *in, const struct net_device *out, int (*okfn) (struct sk_buff *))
{
	struct udphdr *udp_header;
	struct iphdr *ip_header = (struct iphdr *)skb_network_header(skb);    

	if (ip_header->protocol == IPPROTO_UDP) {
		
		udp_header = (struct udphdr *)skb_transport_header(skb);
		portnum = ntohs(udp_header->dest);

		if (portnum != PORT) {
			return NF_ACCEPT;
		}

		printk(KERN_INFO "src = %d, dest = %d, converted dest %u.\n", udp_header->source, udp_header->dest, portnum);
		printk(KERN_INFO "len = %u, data_len = %d\n", skb->len, skb->data_len);

		print_hex_dump(KERN_ALERT, "mem: ", DUMP_PREFIX_ADDRESS, 16, 1, &skb->data, skb->len, 1);


		//return NF_DROP;
	}

	return NF_ACCEPT;
}

static int keystat_show(struct seq_file *m, void *v)
{
	seq_printf(m, "Last UDP port knocked %d\n", portnum);
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

/* Netfilter hook */
static struct nf_hook_ops nfho = {
	.hook       = hook_func,
	.hooknum    = 1, /* NF_IP_LOCAL_IN */
	.pf         = PF_INET,
	.priority   = NF_IP_PRI_FIRST,
};

static int __init keystat_init(void)
{
	printk(KERN_INFO "Loading keystat module, portnum = %d.\n", portnum);
	proc_create("keystat", 0, NULL, &keystat_fops);
	nf_register_hook(&nfho);
	return 0;
}

static void __exit keystat_exit(void)
{
	remove_proc_entry("keystat", NULL);
	nf_unregister_hook(&nfho);
	printk(KERN_INFO "Unloading keystat module.\n");
}

module_init(keystat_init);
module_exit(keystat_exit);

MODULE_LICENSE("GPL");
