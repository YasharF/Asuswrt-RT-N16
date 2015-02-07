/*
 * net/tipc/link.h: Include file for TIPC link code
 *
 * Copyright (c) 1995-2006, Ericsson AB
 * Copyright (c) 2004-2005, Wind River Systems
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the names of the copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * Alternatively, this software may be distributed under the terms of the
 * GNU General Public License ("GPL") version 2 as published by the Free
 * Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _TIPC_LINK_H
#define _TIPC_LINK_H

#include "dbg.h"
#include "msg.h"
#include "bearer.h"
#include "node.h"

#define PUSH_FAILED   1
#define PUSH_FINISHED 2

/*
 * Link states
 */

#define WORKING_WORKING 560810u
#define WORKING_UNKNOWN 560811u
#define RESET_UNKNOWN   560812u
#define RESET_RESET     560813u

/*
 * Starting value for maximum packet size negotiation on unicast links
 * (unless bearer MTU is less)
 */

#define MAX_PKT_DEFAULT 1500

/**
 * struct link - TIPC link data structure
 * @addr: network address of link's peer node
 * @name: link name character string
 * @media_addr: media address to use when sending messages over link
 * @timer: link timer
 * @owner: pointer to peer node
 * @link_list: adjacent links in bearer's list of links
 * @started: indicates if link has been started
 * @checkpoint: reference point for triggering link continuity checking
 * @peer_session: link session # being used by peer end of link
 * @peer_bearer_id: bearer id used by link's peer endpoint
 * @b_ptr: pointer to bearer used by link
 * @tolerance: minimum link continuity loss needed to reset link [in ms]
 * @continuity_interval: link continuity testing interval [in ms]
 * @abort_limit: # of unacknowledged continuity probes needed to reset link
 * @state: current state of link FSM
 * @blocked: indicates if link has been administratively blocked
 * @fsm_msg_cnt: # of protocol messages link FSM has sent in current state
 * @proto_msg: template for control messages generated by link
 * @pmsg: convenience pointer to "proto_msg" field
 * @priority: current link priority
 * @queue_limit: outbound message queue congestion thresholds (indexed by user)
 * @exp_msg_count: # of tunnelled messages expected during link changeover
 * @reset_checkpoint: seq # of last acknowledged message at time of link reset
 * @max_pkt: current maximum packet size for this link
 * @max_pkt_target: desired maximum packet size for this link
 * @max_pkt_probes: # of probes based on current (max_pkt, max_pkt_target)
 * @out_queue_size: # of messages in outbound message queue
 * @first_out: ptr to first outbound message in queue
 * @last_out: ptr to last outbound message in queue
 * @next_out_no: next sequence number to use for outbound messages
 * @last_retransmitted: sequence number of most recently retransmitted message
 * @stale_count: # of identical retransmit requests made by peer
 * @next_in_no: next sequence number to expect for inbound messages
 * @deferred_inqueue_sz: # of messages in inbound message queue
 * @oldest_deferred_in: ptr to first inbound message in queue
 * @newest_deferred_in: ptr to last inbound message in queue
 * @unacked_window: # of inbound messages rx'd without ack'ing back to peer
 * @proto_msg_queue: ptr to (single) outbound control message
 * @retransm_queue_size: number of messages to retransmit
 * @retransm_queue_head: sequence number of first message to retransmit
 * @next_out: ptr to first unsent outbound message in queue
 * @waiting_ports: linked list of ports waiting for link congestion to abate
 * @long_msg_seq_no: next identifier to use for outbound fragmented messages
 * @defragm_buf: list of partially reassembled inbound message fragments
 * @stats: collects statistics regarding link activity
 * @print_buf: print buffer used to log link activity
 */

struct link {
	u32 addr;
	char name[TIPC_MAX_LINK_NAME];
	struct tipc_media_addr media_addr;
	struct timer_list timer;
	struct node *owner;
	struct list_head link_list;

	/* Management and link supervision data */
	int started;
	u32 checkpoint;
	u32 peer_session;
	u32 peer_bearer_id;
	struct bearer *b_ptr;
	u32 tolerance;
	u32 continuity_interval;
	u32 abort_limit;
	int state;
	int blocked;
	u32 fsm_msg_cnt;
	struct {
		unchar hdr[INT_H_SIZE];
		unchar body[TIPC_MAX_IF_NAME];
	} proto_msg;
	struct tipc_msg *pmsg;
	u32 priority;
	u32 queue_limit[15];	/* queue_limit[0]==window limit */

	/* Changeover */
	u32 exp_msg_count;
	u32 reset_checkpoint;

	/* Max packet negotiation */
	u32 max_pkt;
	u32 max_pkt_target;
	u32 max_pkt_probes;

	/* Sending */
	u32 out_queue_size;
	struct sk_buff *first_out;
	struct sk_buff *last_out;
	u32 next_out_no;
	u32 last_retransmitted;
	u32 stale_count;

	/* Reception */
	u32 next_in_no;
	u32 deferred_inqueue_sz;
	struct sk_buff *oldest_deferred_in;
	struct sk_buff *newest_deferred_in;
	u32 unacked_window;

	/* Congestion handling */
	struct sk_buff *proto_msg_queue;
	u32 retransm_queue_size;
	u32 retransm_queue_head;
	struct sk_buff *next_out;
	struct list_head waiting_ports;

	/* Fragmentation/defragmentation */
	u32 long_msg_seq_no;
	struct sk_buff *defragm_buf;

	/* Statistics */
	struct {
		u32 sent_info;		/* used in counting # sent packets */
		u32 recv_info;		/* used in counting # recv'd packets */
		u32 sent_states;
		u32 recv_states;
		u32 sent_probes;
		u32 recv_probes;
		u32 sent_nacks;
		u32 recv_nacks;
		u32 sent_acks;
		u32 sent_bundled;
		u32 sent_bundles;
		u32 recv_bundled;
		u32 recv_bundles;
		u32 retransmitted;
		u32 sent_fragmented;
		u32 sent_fragments;
		u32 recv_fragmented;
		u32 recv_fragments;
		u32 link_congs;		/* # port sends blocked by congestion */
		u32 bearer_congs;
		u32 deferred_recv;
		u32 duplicates;

		/* for statistical profiling of send queue size */

		u32 max_queue_sz;
		u32 accu_queue_sz;
		u32 queue_sz_counts;

		/* for statistical profiling of message lengths */

		u32 msg_length_counts;
		u32 msg_lengths_total;
		u32 msg_length_profile[7];
	} stats;

	struct print_buf print_buf;
};

struct port;

struct link *tipc_link_create(struct bearer *b_ptr, const u32 peer,
			      const struct tipc_media_addr *media_addr);
void tipc_link_delete(struct link *l_ptr);
void tipc_link_changeover(struct link *l_ptr);
void tipc_link_send_duplicate(struct link *l_ptr, struct link *dest);
void tipc_link_reset_fragments(struct link *l_ptr);
int tipc_link_is_up(struct link *l_ptr);
int tipc_link_is_active(struct link *l_ptr);
void tipc_link_start(struct link *l_ptr);
u32 tipc_link_push_packet(struct link *l_ptr);
void tipc_link_stop(struct link *l_ptr);
struct sk_buff *tipc_link_cmd_config(const void *req_tlv_area, int req_tlv_space, u16 cmd);
struct sk_buff *tipc_link_cmd_show_stats(const void *req_tlv_area, int req_tlv_space);
struct sk_buff *tipc_link_cmd_reset_stats(const void *req_tlv_area, int req_tlv_space);
void tipc_link_reset(struct link *l_ptr);
int tipc_link_send(struct sk_buff *buf, u32 dest, u32 selector);
int tipc_link_send_buf(struct link *l_ptr, struct sk_buff *buf);
u32 tipc_link_get_max_pkt(u32 dest,u32 selector);
int tipc_link_send_sections_fast(struct port* sender,
				 struct iovec const *msg_sect,
				 const u32 num_sect,
				 u32 destnode);
int tipc_link_send_long_buf(struct link *l_ptr, struct sk_buff *buf);
void tipc_link_tunnel(struct link *l_ptr, struct tipc_msg *tnl_hdr,
		      struct tipc_msg *msg, u32 selector);
void tipc_link_recv_bundle(struct sk_buff *buf);
int  tipc_link_recv_fragment(struct sk_buff **pending,
			     struct sk_buff **fb,
			     struct tipc_msg **msg);
void tipc_link_send_proto_msg(struct link *l_ptr, u32 msg_typ, int prob, u32 gap,
			      u32 tolerance, u32 priority, u32 acked_mtu);
void tipc_link_push_queue(struct link *l_ptr);
u32 tipc_link_defer_pkt(struct sk_buff **head, struct sk_buff **tail,
		   struct sk_buff *buf);
void tipc_link_wakeup_ports(struct link *l_ptr, int all);
void tipc_link_set_queue_limits(struct link *l_ptr, u32 window);
void tipc_link_retransmit(struct link *l_ptr, struct sk_buff *start, u32 retransmits);

/*
 * Link sequence number manipulation routines (uses modulo 2**16 arithmetic)
 */

static inline u32 mod(u32 x)
{
	return x & 0xffffu;
}

static inline int between(u32 lower, u32 upper, u32 n)
{
	if ((lower < n) && (n < upper))
		return 1;
	if ((upper < lower) && ((n > lower) || (n < upper)))
		return 1;
	return 0;
}

static inline int less_eq(u32 left, u32 right)
{
	return (mod(right - left) < 32768u);
}

static inline int less(u32 left, u32 right)
{
	return (less_eq(left, right) && (mod(right) != mod(left)));
}

static inline u32 lesser(u32 left, u32 right)
{
	return less_eq(left, right) ? left : right;
}

#endif
