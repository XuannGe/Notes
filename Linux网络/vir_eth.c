#include <linux/module.h>
#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/etherdevice.h>

static struct net_device *g_vir_dev;

static void vir_dev_rcpacket(struct sk_buff *skb, struct net_device *dev)
{
    struct ethhdr *ethhdr = NULL;
    unsigned char tmp_dev_addr[ETH_ALEN] = {0};
    struct iphdr *ih = NULL;
    __be32 *saddr = NULL;
    __be32 *daddr = NULL;
    __be32 tmp = 0;
    unsigned char *type = NULL;
    struct sk_buff *rx_skb;

    pr_info("enter vir_dev_rcpacket func\n");
    
    /* 对调源/目MAC地址 */
    ethhdr = (struct ethhdr *)skb->data;
    memcpy(tmp_dev_addr, ethhdr->h_dest, ETH_ALEN);
    memcpy(ethhdr->h_dest, ethhdr->h_source, ETH_ALEN);
    memcpy(ethhdr->h_source, tmp_dev_addr, ETH_ALEN);

    /* 对调源/目IP地址 */
    ih = (struct iphdr *)(skb->data + sizeof(struct ethhdr));
    saddr = &ih->saddr;
    daddr = &ih->daddr;
    tmp = *saddr;
    *saddr = *daddr;
    *daddr = tmp;

    /* 调用ip_fast_csum()获取iphr结构体的校验码 */
    ih->check = 0;
    ih->check = ip_fast_csum((unsigned char *)ih, ih->ihl);

    /* 设置数据类型为0，表示接收ping包 */
    type = skb->data + sizeof(struct ethhdr) + sizeof(struct iphdr);
    *type = 0;

    /* 调用dev_alloc_skb()构造新的接收sk_buff */
    rx_skb = dev_alloc_skb(skb->len + 2);

    /* 调用skb_reserve()预留2字节的头部空间，16字节对齐，（以太网的协议头长度是14个字节） */
    skb_reserve(rx_skb, 2);

    /* 将skb_buff->data复制到新的sk_buff中 利用skb_put动态扩大数据区，避免溢出*/
    memcpy(skb_put(rx_skb, skb->len), skb->data, skb->len);

    /* 设置新的sk_buff的其他成员 */
    rx_skb->dev = dev;
    rx_skb->ip_summed = CHECKSUM_UNNECESSARY;

    /* 调用eth_type_trans()获取上层协议 */
    rx_skb->protocol = eth_type_trans(rx_skb, dev);

    /* 更新接收的统计信息，调用netif_rx()来传递sk_buff数据包 */
    dev->stats.rx_packets++;
    dev->stats.rx_bytes += skb->len;
    dev->last_rx = jiffies;

    netif_rx(rx_skb);
}


static int vir_dev_xmit(struct sk_buff *skb, struct net_device *dev)
{
    pr_info("enter vir_dev_xmit func\n");

    /* 调用netif_stop_queue()阻止上层向网络设备驱动层发送数据包 */
    netif_stop_queue(dev);

    /* 调用收包函数，伪造接收ping包 */
    vir_dev_rcpacket(skb, dev);

    /* 调用dev_kfree_skb()函数释放发送的sk_buff */
    dev_kfree_skb(skb);

    /* 更新发送的统计信息 */
    dev->stats.tx_packets++;
    dev->stats.tx_bytes += skb->len;
    dev->trans_start = jiffies;

    /* 调用netif_wake_queue()唤醒被阻塞的上层 */
    netif_wake_queue(dev);

    return 0;
}

static int vir_dev_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	pr_info("enter vir_dev_ioctl func\n");
	
	switch (cmd) 
	{
		case SIOCGIFHWADDR:
		{
			pr_info("enter SIOCGIFHWADDR cmd\n");
			
			if (!g_vir_dev->addr_len)
			{
				memset(ifr->ifr_hwaddr.sa_data, 0, sizeof(ifr->ifr_hwaddr.sa_data));
			}
			else
			{
				memcpy(ifr->ifr_hwaddr.sa_data, g_vir_dev->dev_addr, min(sizeof(ifr->ifr_hwaddr.sa_data), (size_t)g_vir_dev->addr_len));
			}
		}
	
		case SIOCGIFNETMASK:
		case SIOCSIFNETMASK:

		default:
			return -EOPNOTSUPP;
	}

	return 0;
}


static int vir_dev_open(struct net_device *dev)
{
    pr_info("enter vir_dev_open func\n");
    
    /* 启动接口传输队列 */
    netif_start_queue(dev);
    return 0;
}


static int vir_dev_stop(struct net_device *dev)
{
    pr_info("enter vir_dev_stop func\n");
    
    /* 停止接口传输队列 */
    netif_stop_queue(dev);
    return 0;
}


static const struct net_device_ops vir_dev_ops = {
    .ndo_open       = vir_dev_open,
    .ndo_stop       = vir_dev_stop,
    .ndo_start_xmit = vir_dev_xmit,
    .ndo_do_ioctl   = vir_dev_ioctl,
};


static int vir_dev_register(void)
{
    int ret = 0;

    pr_info("enter the vir_dev_register func\n");

    g_vir_dev = alloc_netdev(sizeof(struct net_device), "vir_dev", NET_NAME_UNKNOWN, ether_setup);
    if (IS_ERR(vir_dev)) 
    {
    	pr_err("alloc netdev fail\n");
        return -ENOMEM;
    }

	memset(g_vir_dev->name,0, sizeof(g_vir_dev->name));
    memcpy(g_vir_dev->name, "vir_eth", 8);

    /* 初始化MAC地址 */
    g_vir_dev->dev_addr[0] = 0x00;
    g_vir_dev->dev_addr[1] = 0x01;
    g_vir_dev->dev_addr[2] = 0x02;
    g_vir_dev->dev_addr[3] = 0x03;
    g_vir_dev->dev_addr[4] = 0x04;
    g_vir_dev->dev_addr[5] = 0x05;
    g_vir_dev->addr_len = 6;

    /* 设置操作函数 */
    g_vir_dev->netdev_ops = &vir_dev_ops;
    g_vir_dev->flags      |= IFF_NOARP;
    g_vir_dev->features   |= NETIF_F_HW_CSUM;

    /* 注册net_device结构体 */
    ret = register_netdev(g_vir_dev);
    if (ret) 
    {
    	pr_err("register netdev fail\n");
    	
        free_netdev(vir_dev);
        return ret;
    }

    return ret;
}


static void vir_dev_unregister(void)
{
	pr_info("enter the vir_dev_unregister func\n");
	
    unregister_netdev(g_vir_dev);
    free_netdev(g_vir_dev);
}


static int __init vir_init(void)
{
    vir_dev_register();

    return 0;
}


static void __exit vir_exit(void)
{
    vir_dev_unregister();
}


module_init(vir_init);
module_exit(vir_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("hyx");


