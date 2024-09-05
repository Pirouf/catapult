/* SPDX-License-Identifier: GPL-2.0+ */

#define BPF_NO_GLOBAL_DATA
#include "vmlinux_local.h"

#include <linux/version.h>
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

#include <bpf/bpf_core_read.h> /* CO-RE */
#include <bpf/bpf_tracing.h> /* BPF_KPROBE */
#include "bpf-defs.h"

#ifndef bpf_target_defined
#warning "Tracing need __TARGET_ARCH_xxx defines"
#endif

char _license[] SEC("license") = "GPL";
u32 _version SEC("version") = LINUX_VERSION_CODE;

extern int LINUX_KERNEL_VERSION __kconfig;

/* https://nakryiko.com/posts/bpf-ringbuf/ */
struct {
  __uint(type, BPF_MAP_TYPE_RINGBUF);
  __uint(max_entries, 256 * 1024);
} rb SEC(".maps");

/* https://nakryiko.com/posts/bpf-tips-printk/ */
/* define our own struct definition if our vmlinux.h is outdated */
struct trace_event_raw_bpf_trace_printk___x {};

/* don't rely on up-to-date vmlinux.h */
enum bpf_func_id___x { BPF_FUNC_snprintf___x = 42 /* avoid zero */ };

#define printk_is_powerful  \
  (bpf_core_enum_value_exists(enum bpf_func_id___x, BPF_FUNC_snprintf___x))
  
#undef  bpf_printk  // See /usr/include/bpf/bpf_helpers.h
#define bpf_printk(fmt, ...)					\
 ({								\
	static char ____fmt[] = fmt "\0";				\
	if (bpf_core_type_exists( struct trace_event_raw_bpf_trace_printk___x)) { \
		bpf_trace_printk(____fmt, sizeof(____fmt) - 1, ##__VA_ARGS__); \
	} else {							\
		____fmt[sizeof(____fmt) - 2] = '\n';			\
		bpf_trace_printk(____fmt, sizeof(____fmt), ##__VA_ARGS__); \
	}								\
 })

/* Gotcha: bpf_core_type_exists() needs __builtin_preserve_type_info */

/* Example of using triple-underscore and preserve_access_index
 * --------------------------------------------------------------------------
 * This demonstrates how it is possible define a local struct that shadow the
 * real kernel struct sk_buff. But only defining the members that the program
 * are interested in. The CO-RE infra-structure will remap the member offsets to
 * the correct offsets used by the kernel. This require adding the LLVM
 * attribute "preserve_access_index". Notice the triple-underscore after the
 * real struct name, which libbpf match and adjust prior to BPF-loading.

SEC("kprobe/udp_send_skb")
int BPF_KPROBE(udp_send_skb, struct sk_buff___local *skb)
{

	unsigned int len;
	u64 id;
	__u32 h;
	struct event *e;
	struct task_struct___local *task;
	u64 ts, *start_ts, start_time = 0;	
	__u32 pid, tid;	

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	// reserve sample from BPF ringbuf
	e = bpf_ringbuf_reserve(&rb, sizeof(*e), 0);
	if (!e)
	  return 0;
	
	// fill out the sample with data
	task = (struct task_struct___local *)bpf_get_current_task();
	start_time = BPF_CORE_READ(task, start_time);
	e->duration_ns = bpf_ktime_get_ns() - start_time;
	e->pid = pid;
	e->ppid = BPF_CORE_READ(task, real_parent, tgid);
	bpf_get_current_comm(&e->comm, sizeof(e->comm));
	// send data to user-space for post-processing
	bpf_ringbuf_submit(e, 0);

	// Find the kernels BTF_ID for struct sk_buff
	id = bpf_core_type_id_kernel(struct sk_buff___local);

	BPF_CORE_READ_INTO(&h,    skb, hash);
	BPF_CORE_READ_INTO(&len,  skb, len);

	bpf_printk("B|%d|udp_send_skb|skb_hash=0x%x;len=%d|bpftrace", pid, h, len);

	return 0;
}

SEC("kretprobe/udp_send_skb")
int BPF_KRETPROBE(udp_send_skb_exit, int ret)
{
	u64 id;
	u32 pid, tid;
	struct event *e;
	struct task_struct___local *task;
	u64 ts, *start_ts, start_time = 0;	

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	// reserve sample from BPF ringbuf
 	e = bpf_ringbuf_reserve(&rb, sizeof(*e), 0);
	if (!e)
	  return 0;
	
	// fill out the sample with data
	task = (struct task_struct___local *)bpf_get_current_task();
	start_time = BPF_CORE_READ(task, start_time);
	e->duration_ns = bpf_ktime_get_ns() - start_time;
	e->pid = pid;
	e->ppid = BPF_CORE_READ(task, real_parent, tgid);
	bpf_get_current_comm(&e->comm, sizeof(e->comm));
	// send data to user-space for post-processing
	bpf_ringbuf_submit(e, 0);

	bpf_printk("E|ret=%d|bpftrace", ret);
	return 0;
}
*/

/*
SEC("kprobe/igc_poll")
int BPF_KPROBE(igc_poll, struct napi_struct___local *napi, int budget)
{
	u64 id;
        __u32 status, tx_flags, tstamp_config_tx_type;
        __u64 tstamp;
	int ifindex;
	__u32 pid, tid;	

	unsigned int len;
	__u32 h;
	struct event *e;
	struct task_struct___local *task;
	u64 ts, *start_ts, start_time = 0;	

	unsigned long state;
	unsigned int flags;
	unsigned int num_q_vectors;
	u16 tx_ring_count;
	u16 rx_ring_count;

	struct igc_ring_container__local rx;
	struct igc_ring_container__local tx;
	
	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	BPF_CORE_READ_INTO(&ifindex,    napi, dev, ifindex);

	struct igc_q_vector *q_vector = container_of(napi,
						     struct igc_q_vector,
						     napi);
	BPF_CORE_READ_INTO(&state,    q_vector, adapter, state);
	BPF_CORE_READ_INTO(&flags,    q_vector, adapter, flags);

	BPF_CORE_READ_INTO(&num_q_vectors,    q_vector, adapter, num_q_vectors);

	BPF_CORE_READ_INTO(&tx_ring_count,    q_vector, adapter, tx_ring_count);
	BPF_CORE_READ_INTO(&rx_ring_count,    q_vector, adapter, rx_ring_count);

	BPF_CORE_READ_INTO(&rx,    q_vector, rx);
	BPF_CORE_READ_INTO(&tx,    q_vector, tx);

	struct igc_ring__local *rx_ring; // q_vector->rx.ring;
	struct igc_ring__local *tx_ring; // q_vector->tx.ring;

	bpf_core_read(&rx_ring, 8, &q_vector->rx.ring);
	bpf_core_read(&tx_ring, 8, &q_vector->tx.ring);

	unsigned int tx_count = (unsigned int) tx_ring->count;
	if (bpf_core_field_exists(rx_ring->count))
	  bpf_printk("B|%d|igc_poll|ifindex=%d|bpftrace", pid, ifindex);
	  
	//BPF_CORE_READ_INTO(&tx_count,    tx_ring, count);
	//bpf_core_read(&tx_count, 8, tx_ring->count);
	//bpf_printk("B|%d|igc_poll|ifindex=%d|count=%x|bpftrace", pid, ifindex, (int) tx_count);
	
	//struct igc_ring__local *rx_ring = (igc_ring__local *) (BPF_CORE_READ(q_vector, rx)).ring; // q_vector->tx.ring;
	//struct igc_ring__local *tx_ring = (igc_ring__local *) (BPF_CORE_READ(q_vector, tx)).ring; // q_vector->tx.ring;
	
	//struct xsk_buff_pool__local *xsk_pool = BPF_CORE_READ(rx_ring, xsk_pool); 
	//struct xsk_buff_pool__local *xsk_pool = rx_ring->xsk_pool;

	struct igc_tx_buffer___local *tx_buffer = (struct igc_tx_buffer___local *)tx_ring->tx_buffer_info;
	union igc_adv_tx_desc___local *tx_desc = (union igc_adv_tx_desc___local *)tx_ring->desc;
	enum igc_tx_buffer_type__local type;

	if (tx_ring) {
	  u16 tx_count = 0;
	  if (bpf_core_field_exists(tx_ring->count)) {
	    tx_count = tx_ring->count;
	    if (tx_count > 0) {

	      bpf_printk("B|%d|clean_tx_irq|ifindex=%d|tx_count=%d|bpftrace", pid, ifindex, tx_count);

	      // reserve sample from BPF ringbuf
	      e = bpf_ringbuf_reserve(&rb, sizeof(*e), 0);
	      if (!e)
		return 0;
	
	      // fill out the sample with data
	      task = (struct task_struct___local *)bpf_get_current_task();
	      start_time = BPF_CORE_READ(task, start_time);
	      e->duration_ns = bpf_ktime_get_ns() - start_time;
	      e->pid = pid;
	      e->ppid = BPF_CORE_READ(task, real_parent, tgid);
	      bpf_get_current_comm(&e->comm, sizeof(e->comm));

	      // send data to user-space for post-processing
	      bpf_ringbuf_submit(e, 0);
	    }
	  }
	}
	
	if (rx_ring) {
	  unsigned int rx_count = 0;
	  if (bpf_core_field_exists(rx_ring->count)) {
	    rx_count = (unsigned int)rx_ring->count;

	    if (rx_count > 0) {
	      if (test_bit(IGC_RING_FLAG_AF_XDP_ZC, &flags))
		bpf_printk("B|%d|clean_rx_irq_zc|ifindex=%d|rx_count=%d|bpftrace", pid, ifindex, rx_count);
	      else
		bpf_printk("B|%d|clean_rx_irq|ifindex=%d|rx_count=%d|bpftrace", pid, ifindex, rx_count);

	      // reserve sample from BPF ringbuf
	      e = bpf_ringbuf_reserve(&rb, sizeof(*e), 0);
	      if (!e)
		return 0;
	
	      // fill out the sample with data
	      task = (struct task_struct___local *)bpf_get_current_task();
	      start_time = BPF_CORE_READ(task, start_time);
	      e->duration_ns = bpf_ktime_get_ns() - start_time;
	      e->pid = pid;
	      e->ppid = BPF_CORE_READ(task, real_parent, tgid);
	      bpf_get_current_comm(&e->comm, sizeof(e->comm));

	      // send data to user-space for post-processing
	      bpf_ringbuf_submit(e, 0);

	    }
	  }
	}


	struct igc_tx_buffer___local *tx_buffer = (struct igc_tx_buffer___local *)ctx->bx;
	union igc_adv_tx_desc___local *tx_desc = (union igc_adv_tx_desc___local *)ctx->r15;
	enum igc_tx_buffer_type__local type;

	BPF_CORE_READ_INTO(&tx_flags, tx_buffer, tx_flags);
	BPF_CORE_READ_INTO(&status, tx_desc, wb.status);
	BPF_CORE_READ_INTO(&tstamp, tx_desc, wb.dma_tstamp);
	tstamp_config_tx_type = (BPF_CORE_READ(adapter, tstamp_config)).tx_type;

	bpf_printk("B|%d|clean_tx_irq|ifindex=%d;status=%u|bpftrace", pid, ifindex, status);
	if (tstamp_config_tx_type == HWTSTAMP_TX_ON)
	  bpf_printk("TX HW TS %lld", tstamp);
	return 0;
}

SEC("kretprobe/igc_poll")
int BPF_KRETPROBE(poll_exit, int ret)
{

	long avail_data = 0;
	long ring_size = 0;
	long cons_pos = 0;
	long prod_pos = 0;

	int cur_pid = bpf_get_current_pid_tgid() >> 32;
	struct event *e;

	avail_data = bpf_ringbuf_query(&rb, BPF_RB_AVAIL_DATA);
	if (!avail_data)
		return 0;

	//if (cur_pid != pid)
	//	return 0;
	// debug returned file descriptor

	ring_size = bpf_ringbuf_query(&rb, BPF_RB_RING_SIZE);
	cons_pos = bpf_ringbuf_query(&rb, BPF_RB_CONS_POS);
	prod_pos = bpf_ringbuf_query(&rb, BPF_RB_PROD_POS);

	bpf_printk("E|ret=%d|bpftrace", ret);
	return 0;
}

SEC("kprobe/igc_xdp_xmit")
int BPF_KPROBE(xdp_xmit, struct net_device___local *dev, int num_frames,
	       struct xdp_frame___local **frames, u32 flags)
{
	__u16 len;
	__u16 metasize;
	__u64 id;
	int ifindex;
	__u32 pid, tid;	

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	BPF_CORE_READ_INTO(&ifindex,  dev, ifindex);
	//BPF_CORE_READ_INTO(&len,  frames[0], len);
	//BPF_CORE_READ_INTO(&metasize,  frames[0], metasize);

	bpf_printk("B|%d|xdp_xmit|ifindex=%d|bpftrace", pid, ifindex);

	return 0;
}

SEC("kretprobe/igc_xdp_xmit")
int BPF_KRETPROBE(xdp_xmit_exit, int ret)
{
	// debug returned file descriptor
	bpf_printk("E|ret=%d|bpftrace", ret);
	return 0;
}
	*/

/*
SEC("kprobe/igc_xsk_wakeup")
int BPF_KPROBE(xsk_wakeup, struct net_device___local *dev,
	       u32 queue_id, u32 flags)
{
	u64 id;
	int ifindex;
	__u32 pid, tid;	

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	BPF_CORE_READ_INTO(&ifindex,  dev, ifindex);

	bpf_printk("B|%d|xsk_wakeup|ifindex=%d;queue_id=%d|bpftrace", pid, ifindex, queue_id);

	return 0;
}


SEC("kretprobe/igc_xsk_wakeup")
int BPF_KRETPROBE(xsk_wakeup_exit, int ret)
{
	// debug returned file descriptor
	bpf_printk("E|ret=%d|bpftrace", ret);
	return 0;
}
*/
