/*
 * Network configuration layer (Linux)
 *
 * Copyright (C) 2008, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: netconf_linux.h,v 1.13 2008/10/02 02:38:56 Exp $
 */

#ifndef _netconf_linux_h_
#define _netconf_linux_h_
#include <linux/version.h>

/* Debug malloc() */
#ifdef DMALLOC
#include <dmalloc.h>
#endif /* DMALLOC */

/* iptables definitions */
#include <libiptc/libiptc.h>
#include <iptables.h>
#include <linux/netfilter_ipv4/ip_tables.h>

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 22) )
#include <linux/netfilter_ipv4/ip_nat_rule.h>
#else
#include <linux/netfilter/nf_nat.h>
#include <linux/netfilter/nf_conntrack_common.h>
#endif
#define ETH_ALEN ETHER_ADDR_LEN
#include <linux/netfilter_ipv4/ipt_mac.h>
#include <linux/netfilter_ipv4/ipt_state.h>
#include <linux/netfilter_ipv4/ipt_time.h>
#include <linux/netfilter_ipv4/ipt_TCPMSS.h>
#include <linux/netfilter_ipv4/ipt_LOG.h>
#include <linux/netfilter_ipv4/ip_autofw.h>

/* ipt_entry alignment attribute */
#define IPT_ALIGNED ((aligned (__alignof__ (struct ipt_entry))))

/* TCP flags */
#define	TH_FIN	0x01
#define	TH_SYN	0x02
#define	TH_RST	0x04
#define	TH_PUSH	0x08
#define	TH_ACK	0x10
#define	TH_URG	0x20

#endif /* _netconf_linux_h_ */
