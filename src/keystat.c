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
		printk(KERN_INFO "len = %u, data_len = %d, addr data %pa, data tail %pa\n", skb->len, skb->data_len, skb->data, skb->tail);

		print_hex_dump(KERN_ALERT, "mem: ", DUMP_PREFIX_ADDRESS, 16, 1, &skb->data, skb->len, 0);


		//return NF_DROP;
	}

	return NF_ACCEPT;
}

static int minitel_show(struct seq_file *m, void *v)
{
	seq_printf(m, "Last UDP port knocked %d\n", portnum);
	return 0;
}

static int minitel_open(struct inode *inode, struct file *file)
{
	return single_open(file, minitel_show, NULL);
}

static const struct file_operations minitel_fops = {
	.owner      = THIS_MODULE,
	.open       = minitel_open,
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

static int __init minitel_init(void)
{
	printk(KERN_INFO "Loading minitel module, port = %d.\n", PORT);
	proc_create("minitel", 0, NULL, &minitel_fops);
	nf_register_hook(&nfho);
	return 0;
}

static void __exit minitel_exit(void)
{
	remove_proc_entry("minitel", NULL);
	nf_unregister_hook(&nfho);
	printk(KERN_INFO "Unloading minitel module.\n");
}

module_init(minitel_init);
module_exit(minitel_exit);

MODULE_LICENSE("GPL");
