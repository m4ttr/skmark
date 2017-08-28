/*
 * Module for modifying the sock->sk_mark field
 * of the sock struct, for use by QoS reflection or
 * multihomed routing.
 * based on xt_socket
 *
 * Author: Matt Rivet 
 *
 * inspired from from xt_socket.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.*
 *
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netfilter_ipv4/ip_tables.h>
#include <linux/spinlock.h>
#include <net/sock.h>
#include <net/inet_sock.h>
#include <net/tcp.h>
#include <net/udp.h>

#include <linux/netfilter/x_tables.h>
#include <netfilter/xt_SKMARK.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matt Rivet");
MODULE_DESCRIPTION("x_tables set sk->sk_mark value on socket");
MODULE_ALIAS("ipt_skmark");

static struct sock *
skmark_tg_get_sock(struct sk_buff *skb, const struct net_device *in, struct net *net)
{
        const struct iphdr *iph = ip_hdr(skb);
        __be32 uninitialized_var(daddr), uninitialized_var(saddr);
        __be16 uninitialized_var(dport), uninitialized_var(sport);
        u8 uninitialized_var(protocol);

        if (iph->protocol == IPPROTO_UDP || iph->protocol == IPPROTO_TCP) {
                struct udphdr _hdr, *hp;
                hp = skb_header_pointer(skb, ip_hdrlen(skb), sizeof(_hdr), &_hdr);
                if (hp == NULL) {
                        return NULL;
                }

                protocol = iph->protocol;
                saddr = iph->saddr;
                sport = hp->source;
                daddr = iph->daddr;
                dport = hp->dest;

        } else {
                return NULL;
        }
        switch (protocol) {
        case IPPROTO_TCP:
                return __inet_lookup(net, &tcp_hashinfo, saddr, sport,
                                     daddr, dport, in->ifindex);
        case IPPROTO_UDP:
                return udp4_lib_lookup(net, saddr, sport, daddr, dport, in->ifindex);
    }
    return NULL;
}


static unsigned int
skmark_tg_v4(struct sk_buff *skb, const struct xt_action_param *par)
{
        const struct xt_skmark_tg_info *info = par->targinfo;
        struct sock *sk;

        sk = skb->sk;
<<<<<<< HEAD:kmod/ipt_SKMARK.c
        if (!sk){
                sk = skmark_tg_get_sock(skb, par->in, dev_net(skb->dev));
        }

        // avoid locking if sk_mark is already set
        if ((sk && sk->sk_mark == 0) && info->skmark != 0) {
                // http://lists.openwall.net/netdev/2009/10/13/153
                // lock_sock(sk);
                spin_lock_bh(&sk->sk_receive_queue.lock);
                sk->sk_mark = info->skmark;
                // release_sock(sk);
                spin_unlock_bh(&sk->sk_receive_queue.lock);
=======
        // If the SK is already set, just move along somehow
        if (sk) {
            sk->sk_mark = info->somark;
        } else {
            sk = somark_tg_get_sock(skb, par->in, dev_net(skb->dev));
            sk->sk_mark = info->somark;
>>>>>>> removing debug stuff:kmod/xt_SOMARK.c
        }
        return XT_CONTINUE;
}

static struct xt_target skmark_tg_reg __read_mostly = {
        .name       = "SKMARK",
        .revision   = 0,
        .family     = NFPROTO_UNSPEC,
        .target     = skmark_tg_v4,
        .targetsize = sizeof(struct xt_skmark_tg_info),
        .me         = THIS_MODULE,
};


static int __init skmark_tg_init(void)
{
        return xt_register_target(&skmark_tg_reg);
}

static void __exit skmark_tg_exit(void)
{
        xt_unregister_target(&skmark_tg_reg);
}

module_init(skmark_tg_init);
module_exit(skmark_tg_exit);
