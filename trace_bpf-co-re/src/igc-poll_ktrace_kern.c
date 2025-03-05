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

/* https://nakryiko.com/posts/bpf-ringbuf/
struct {
  __uint(type, BPF_MAP_TYPE_RINGBUF);
  __uint(max_entries, 256 * 1024);
} rb SEC(".maps");
*/

#define IGC_MAX_POLL 16

struct igc_poll_state {
  long unsigned int prev_rx_flags[IGC_MAX_POLL];
  long unsigned int prev_tx_flags[IGC_MAX_POLL];
  unsigned int prev_pid[IGC_MAX_POLL];
  bool first;
} g_state = { .first = true };

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

/*
 * __bpf_vprintk wraps the bpf_trace_vprintk helper with variadic arguments
 * instead of an array of u64.
 */
#define bpf_vprintk(fmt, args...)				\
({								\
	static const char ___fmt[] = fmt;			\
	unsigned long long ___param[___bpf_narg(args)];		\
								\
	_Pragma("GCC diagnostic push")				\
	_Pragma("GCC diagnostic ignored \"-Wint-conversion\"")	\
	___bpf_fill(___param, args);				\
	_Pragma("GCC diagnostic pop")				\
								\
	bpf_trace_vprintk(___fmt, sizeof(___fmt),		\
			  ___param, sizeof(___param));		\
								\
})

/* Gotcha: bpf_core_type_exists() needs __builtin_preserve_type_info */

/* Example of using triple-underscore and preserve_access_index
 * --------------------------------------------------------------------------
 * This demonstrates how it is possible define a local struct that shadow the
 * real igc kernel module struct . But only defining the members that the program
 * are interested in. The CO-RE infra-structure will remap the member offsets to
 * the correct offsets used by the kernel. This require adding the LLVM
 * attribute "preserve_access_index". Notice the triple-underscore after the
 * real struct name, which libbpf match and adjust prior to BPF-loading.
*/
SEC("kprobe/igc_poll")
int BPF_KPROBE(igc_clean_tx_irq, struct napi_struct___local *napi, int budget)
{
	u64 id;
        __u32 status;
        __u64 tstamp;
	int ifindex;
	char name[25];
	__u32 pid, tid;	

	unsigned int len;
	__u32 h;
	struct event *e;
	struct task_struct___local *task;
	u64 ts, *start_ts, start_time = 0;	
	
	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;


	struct igc_q_vector___local *q_vector = container_of(napi,
						     struct igc_q_vector___local,
						     napi);
	BPF_CORE_READ_INTO(&ifindex,    napi, dev, ifindex);
	BPF_CORE_READ_STR_INTO(&name,    q_vector, name);
	bpf_printk("B|%d|igc_poll|irq=%s;ifindex=%u|bpftrace",
		   pid, name, ifindex);

	//struct igc_adapter___local *adapter;
	//adapter = (struct igc_adapter___local *) BPF_CORE_READ(q_vector, adapter);
	//BPF_CORE_READ_INTO(&ifindex,    adapter, netdev, ifindex);

	/* This example demonstrates how it is possible define igc_ring___local array struct
	 * that shadow the real kernel module igc_ring array  struct
	 * struct igc_ring___local *tx_ring[4];
	 * tx_ring[0] = (struct igc_ring___local *)BPF_CORE_READ(q_vector, adapter, tx_ring[0]);
	 * tx_ring[1] = (struct igc_ring___local *)BPF_CORE_READ(q_vector, adapter, tx_ring[1]);
	 * tx_ring[2] = (struct igc_ring___local *)BPF_CORE_READ(q_vector, adapter, tx_ring[2]);
	 * tx_ring[3] = (struct igc_ring___local *)BPF_CORE_READ(q_vector, adapter, tx_ring[3]);
	 */

	unsigned long state;
	unsigned int flags;
	unsigned int num_q_vectors;
	BPF_CORE_READ_INTO(&state,    q_vector, adapter, state);
	BPF_CORE_READ_INTO(&flags,    q_vector, adapter, flags);
	BPF_CORE_READ_INTO(&num_q_vectors,    q_vector, adapter, num_q_vectors);

	struct igc_ring_container___local rx;
	struct igc_ring_container___local tx;
	BPF_CORE_READ_INTO(&rx,    q_vector, rx);
	BPF_CORE_READ_INTO(&tx,    q_vector, tx);

	struct igc_ring___local *rx_ring;
	rx_ring = (struct igc_ring___local *) BPF_CORE_READ(q_vector, rx.ring);
	struct igc_ring___local *tx_ring;
	tx_ring = (struct igc_ring___local *) BPF_CORE_READ(q_vector, tx.ring);

	long unsigned int rx_flags;
	long unsigned int tx_flags;
	BPF_CORE_READ_INTO(&rx_flags, rx_ring, flags);
	BPF_CORE_READ_INTO(&tx_flags, tx_ring, flags);

	u8 queue_index;
	BPF_CORE_READ_INTO(&queue_index, tx_ring, queue_index);

	if (tx_ring) {
	  u32 xsk_frames = 0;
	  u16 next_to_clean;
	  BPF_CORE_READ_INTO(&next_to_clean, tx_ring, next_to_clean);
	  unsigned int i = next_to_clean;
	  struct igc_tx_buffer___local *tx_buffer_info;
	  tx_buffer_info = (struct igc_tx_buffer___local *) BPF_CORE_READ(tx_ring, tx_buffer_info);
	  struct igc_tx_buffer___local *tx_buffer;
	  tx_buffer = &tx_buffer_info[i];
	  if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
	    bpf_vprintk("B|%d|igc_clean_tx_irq|ifindex=%d;queue=%u;flags=%lu|bpftrace",
			pid, ifindex, queue_index, tx_flags);
	  else
	    bpf_printk("B|%d|igc_clean_tx_irq|ifindex=%d;flags=%lu|bpftrace",
		       pid, ifindex, tx_flags);
	}

	/*
	bpf_printk("B|%d|igc_poll|adapter=%p|rx_ring=%p|bpftrace",
		   pid, adapter, rx_ring);

	*/
	return 0;
}

/* Example of attaching kprobes mid-function_
 * --------------------------------------------------------------------------
 * This demonstrates how it is possible define a local struct that shadow the
 * real igc kernel module mid-function  ..
 *  https://blog.tohojo.dk/2023/04/netfilter-packet-drop-attribution-using-bpf.html
SEC("kprobe/igc_poll+1043")
SEC("kprobe/igc_poll+1024")
int BPF_KPROBE(igc_clean_rx_irq, struct napi_struct___local *napi, int budget)
{
	u64 id;
        __u32 status;
        __u64 tstamp;
	int ifindex;
	__u32 pid, tid;	

	unsigned int len;
	__u32 h;
	struct event *e;
	struct task_struct___local *task;
	u64 ts, *start_ts, start_time = 0;	
	
	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	if (!ctx->r13)
	  return 0;

	struct igc_ring___local *rx_ring;
	rx_ring = ((struct igc_ring___local *) ctx->r13);

	BPF_CORE_READ_INTO(&ifindex,    rx_ring, netdev, ifindex);
	
	long unsigned int rx_flags;
	BPF_CORE_READ_INTO(&rx_flags, rx_ring, flags);

	u8 queue_index;
	BPF_CORE_READ_INTO(&queue_index, rx_ring, queue_index);
	
	if (rx_ring) {

	  bpf_printk("E|%d|ret=%d|bpftrace", pid, 0);
	  
	  struct igc_rx_buffer___local *rx_buffer_info;
	  rx_buffer_info = (struct igc_rx_buffer___local *) BPF_CORE_READ(rx_ring, rx_buffer_info);
	  if (test_bit(IGC_RING_FLAG_AF_XDP_ZC, &rx_flags)) {
	    if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
	      bpf_vprintk("B|%d|igc_clean_rx_irq_zc|ifindex=%d;queue=%u;flags=%lu|bpftrace",
			  pid, ifindex, queue_index, rx_flags);
	    else
	      bpf_printk("B|%d|igc_clean_rx_irq_zc|ifindex=%d;flags=%lu|bpftrace",
			 pid, ifindex, rx_flags);
	  } else {
	    if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
	      bpf_vprintk("B|%d|igc_clean_rx_irq|ifindex=%d;queue=%u;flags=%lu|bpftrace",
			  pid, ifindex, queue_index, rx_flags);
	    else
	      bpf_printk("B|%d|igc_clean_rx_irq|ifindex=%d;flags=%lu|bpftrace",
			 pid, ifindex, rx_flags);
	  }
	}
	return 0;
}
*/

/* This BPF CO-RE tracepoints will remap the member offsets to
 * the tx_ring correct offsets used by the igc kernel module.
*/
SEC("kprobe/xsk_tx_completed")
bool BPF_KPROBE(xsk_tx_completed, struct xsk_buff_pool___local *pool,
	       u32 nb_entries)
{
	__u64 id;
	int ifindex;
	__u32 pid, tid;	

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	bpf_printk("E|%d|ret=%d|bpftrace", pid, 0);

	BPF_CORE_READ_INTO(&ifindex,    pool, netdev, ifindex);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
	  bpf_vprintk("B|%d|xsk_tx_completed|ifindex=%d|bpftrace",
		      pid, ifindex);
	else
	  bpf_printk("B|%d|xsk_tx_completed|ifindex=%d|bpftrace",
		     pid, ifindex);

	return 0;
}

SEC("kprobe/xsk_set_tx_need_wakeup")
bool BPF_KPROBE(xsk_set_tx_need_wakeup, struct xsk_buff_pool___local *pool)
{
	__u64 id;
	int ifindex;
	__u32 pid, tid;	

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	bpf_printk("E|%d|ret=%d|bpftrace", pid, 0);

	BPF_CORE_READ_INTO(&ifindex,    pool, netdev, ifindex);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
	  bpf_vprintk("B|%d|xsk_set_tx_need_wakeup|ifindex=%d|bpftrace",
		      pid, ifindex);
	else
	  bpf_printk("B|%d|xsk_set_tx_need_wakeup|ifindex=%d|bpftrace",
		     pid, ifindex);

	return 0;
}


SEC("kprobe/xsk_tx_peek_desc")
bool BPF_KPROBE(xsk_tx_peek_desc, struct xsk_buff_pool___local *pool,
	       struct xdp_desc___local *desc)
{
	__u64 id;
	int ifindex;
	__u32 pid, tid;	

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	bpf_printk("E|%d|ret=%d|bpftrace", pid, 0);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
	  bpf_vprintk("B|%d|igc_xdp_xmit_zc|bpftrace",
		      pid);
	else
	  bpf_printk("B|%d|igc_xdp_xmit_zc|bpftrace",
		     pid);

	return 0;
}

SEC("kprobe/igc_tx_ctxtdesc")
int BPF_KPROBE(igc_tx_ctxtdesc, struct igc_ring___local *tx_ring,
			    __le32 launch_time, bool first_flag,
			    u32 vlan_macip_lens, u32 type_tucmd,
			    u32 mss_l4len_idx)
{
	__u64 id;
	int ifindex;
	__u32 pid, tid;	

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	bpf_printk("E|%d|ret=%d|bpftrace", pid, 0);

	return 0;
}

SEC("kprobe/xsk_tx_release")
int BPF_KPROBE(xsk_tx_release, struct xsk_buff_pool___local *pool) 
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

	bpf_printk("E|%d|ret=%d|bpftrace", pid, 0);

	return 0;
}


SEC("kprobe/igc_xdp_xmit")
bool BPF_KPROBE(igc_xdp_xmit, struct net_device___local *dev,
		int num_frames,
		struct xdp_frame___local **frames,
		u32 flags)
{
	__u64 id;
	int ifindex;
	__u32 pid, tid;	

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	bpf_printk("E|%d|ret=%d|bpftrace", pid, 0);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
	  bpf_vprintk("B|%d|igc_xdp_xmit|bpftrace",
		      pid);
	else
	  bpf_printk("B|%d|igc_xdp_xmit|bpftrace",
		     pid);

	return 0;
}

SEC("kretprobe/igc_xdp_xmit")
int BPF_KRETPROBE(xdp_xmit_exit, int ret)
{
	int cur_pid = bpf_get_current_pid_tgid() >> 32;

	// debug returned file descriptor
	bpf_printk("E|%d|ret=%d|bpftrace", cur_pid, 0);

	return 0;
}


/*
*/
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

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {
	  bpf_vprintk("B|%d|igc_xsk_wakeup|ifindex=%d;queue_id=%d|bpftrace", pid, ifindex, queue_id);
	} else {
	  bpf_printk("B|%d|igc_xsk_wakeup|ifindex=%d;queue_id=%d|bpftrace", pid, ifindex, queue_id);
	}

	return 0;}



SEC("kretprobe/igc_xsk_wakeup")
int BPF_KRETPROBE(xsk_wakeup_exit, int ret)
{
	int cur_pid = bpf_get_current_pid_tgid() >> 32;

	// debug returned file descriptor
	bpf_printk("E|%d|ret=%d|bpftrace", cur_pid, 0);
}

/* This BPF CO-RE tracepoints will remap the member offsets to
 * the rx_ring correct offsets used by the igc kernel module.
*/
SEC("kprobe/igc_alloc_rx_buffers_zc")
bool BPF_KPROBE(igc_alloc_rx_buffers_zc, struct igc_ring___local *ring, u16 count)
{
	__u64 id;
	int ifindex;
	__u32 pid, tid;	

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	bpf_printk("E|%d|ret=%d|bpftrace", pid, 0);

	BPF_CORE_READ_INTO(&ifindex,    ring, netdev, ifindex);

	u8 queue_index;
	BPF_CORE_READ_INTO(&queue_index, ring, queue_index);


	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
	  bpf_vprintk("B|%d|igc_alloc_rx_buffers_zc|ifindex=%d;queue=%d|bpftrace",
		      pid, ifindex,queue_index);
	else
	  bpf_printk("B|%d|igc_alloc_rx_buffers_zc|ifindex=%d;queue=%d|bpftrace",
		     pid, ifindex,queue_index);

	return 0;
}

SEC("kprobe/igc_alloc_rx_buffers")
int BPF_KPROBE(igc_alloc_rx_buffers, struct igc_ring___local *rx_ring, u16 cleaned_count)
{
	__u64 id;
	int ifindex;
	__u32 pid, tid;	

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	bpf_printk("E|%d|ret=%d|bpftrace", pid, 0);

	BPF_CORE_READ_INTO(&ifindex, rx_ring, netdev, ifindex);

	u8 queue_index;
	BPF_CORE_READ_INTO(&queue_index, rx_ring, queue_index);


	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
	  bpf_vprintk("B|%d|igc_alloc_rx_buffers|ifindex=%d;queue=%d|bpftrace",
		      pid, ifindex,queue_index);
	else
	  bpf_printk("B|%d|igc_alloc_rx_buffers|ifindex=%d;queue=%d|bpftrace",
		     pid, ifindex,queue_index);

	return 0;
}

SEC("kprobe/xsk_set_rx_need_wakeup")
bool BPF_KPROBE(xsk_set_rx_need_wakeup, struct xsk_buff_pool___local *pool)
{
	__u64 id;
	int ifindex;
	__u32 pid, tid;

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	bpf_printk("E|%d|ret=%d|bpftrace", pid, 0);

	BPF_CORE_READ_INTO(&ifindex,    pool, netdev, ifindex);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
	  bpf_vprintk("B|%d|xsk_set_rx_need_wakeup|ifindex=%d|bpftrace",
		      pid, ifindex);
	else
	  bpf_printk("B|%d|xsk_set_rx_need_wakeup|ifindex=%d|bpftrace",
		     pid, ifindex);

	return 0;
}

SEC("kprobe/napi_complete_done")
bool BPF_KPROBE(napi_complete_done, struct napi_struct___local *n, int work_done)
{
	__u64 id;
	int ifindex;
	__u32 pid, tid;	

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	bpf_printk("E|%d|ret=%d|bpftrace", pid, 0);

	return 0;
}

/*
SEC("kprobe/__rcu_read_lock")
void BPF_KPROBE(__rcu_read_lock, void)
{
	__u64 id;
	int ifindex;
	__u32 pid, tid;	

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	return 0;
}

SEC("kretprobe/__rcu_read_unlock")
void BPF_KPROBE(__rcu_read_lock, void)
{
	__u64 id;
	int ifindex;
	__u32 pid, tid;	

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	tid = (u32)id;

	return 0;
}
*/

SEC("kretprobe/igc_poll")
int BPF_KRETPROBE(poll_exit, int ret)
{

	int cur_pid = bpf_get_current_pid_tgid() >> 32;

	bpf_printk("E|%d|ret=%d|bpftrace", cur_pid, 0);

	return 0;
}

