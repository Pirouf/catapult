/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef BPF_DEFS_H
#define BPF_DEFS_H

#include <linux/net_tstamp.h>
#include <stdbool.h>

#define TASK_COMM_LEN 16
#define MAX_FILENAME_LEN 127
#define IGC_TXD_STAT_TS_STAT 0x00000002 /* DMA Timestamp in packet */

struct event {
  int pid;
  int ppid;
  unsigned exit_code;
  unsigned long long duration_ns;
  char comm[TASK_COMM_LEN];
};

/* local partial kernel struct definitions with just the members we need. 
 * These are deliberately *not* declared with the preserve_access_index, as
 * we'll read them with plan BPF_PROBE_READ() below; this is to make sure they
 * work even without module BTF, and the fields we need are only the first ones
 * of each struct which have been stable for a long time.
 */
struct task_struct___local {

	int pid;
	int tgid;

	/* Real parent process: */
	struct task_struct___local *real_parent;

        /* Monotonic time in nsecs: */
	u64 start_time;
} __attribute__((preserve_access_index));


struct sk_buff___local {
	unsigned int len;
	__u32 hash;
} __attribute__((preserve_access_index));


struct net {
	__u64 net_cookie;
} __attribute__((preserve_access_index));

typedef struct {
	__s64 counter;
} atomic64_t;

struct net___old {
	atomic64_t net_cookie;
} __attribute__((preserve_access_index));

struct device_type__local {
	const char *name;
} __attribute__((preserve_access_index));

struct device__local {
	struct device__local *parent;
	const char *init_name;
	const struct device_type__local *type;
} __attribute__((preserve_access_index));

struct net_device___local {
	int ifindex;
	struct {
		struct net *net;
	} nd_net;
} __attribute__((preserve_access_index));

struct napi_struct___local {
	struct net_device___local *dev;
	struct sk_buff___local *skb;
	int defer_hard_irqs_count;
	int rx_count; /* length of rx_list */
	unsigned intnapi_id;
} __attribute__((preserve_access_index));


struct xdp_desc___local {
	__u64 addr;
	__u32 len;
	__u32 options;
	__u64 txtime;
} __attribute__((preserve_access_index));

struct xdp_rxq_info__local {
	struct net_device__local *dev;
	u32 queue_index;
	u32 reg_state;
	unsigned int napi_id;
	u32 frag_size;
	long: 64;
	long: 64;
	long: 64;
	long: 64;
} __attribute__((preserve_access_index));

struct xdp_txq_info__local {
	struct net_device__local *dev;
} __attribute__((preserve_access_index));

struct xdp_buff__local {
	void *data;
	void *data_end;
	void *data_meta;
	void *data_hard_start;
	struct xdp_rxq_info__local *rxq;
	struct xdp_txq_info__local *txq;
	u32 frame_sz;
	u32 flags;
} __attribute__((preserve_access_index));
/*
struct xsk_buff_pool__local;

struct xdp_buff_xsk__local {
	struct xdp_buff__local xdp;
	struct xsk_buff_pool__local *pool;
} __attribute__((preserve_access_index));
*/

static inline int test_bit(int nr, const void *addr)
{
	const u32 *p = addr;
	return ((1UL << (nr & 31)) & (p[nr >> 5])) != 0 ? 1 : 0;
}

enum igc_state_t {
	__IGC_TESTING = 0,
	__IGC_RESETTING = 1,
	__IGC_DOWN = 2,
};

enum igc_ring_flags_t {
	IGC_RING_FLAG_RX_3K_BUFFER = 0,
	IGC_RING_FLAG_RX_BUILD_SKB_ENABLED = 1,
	IGC_RING_FLAG_RX_SCTP_CSUM = 2,
	IGC_RING_FLAG_RX_LB_VLAN_BSWAP = 3,
	IGC_RING_FLAG_TX_CTX_IDX = 4,
	IGC_RING_FLAG_TX_DETECT_HANG = 5,
	IGC_RING_FLAG_AF_XDP_ZC = 6,
};

struct xsk_buff_pool__local {
	struct net_device__local *netdev;
	u32 heads_cnt;
	u16 queue_id;
  //struct xdp_buff_xsk__local *heads;
  //struct xdp_desc__local *tx_descs;
} __attribute__((preserve_access_index));

struct xdp_frame___local {
	void *data;
	__u16 len;
	__u16 headroom;
	__u16 metasize;
} __attribute__((preserve_access_index));

struct wb {
  __le64 dma_tstamp;
  __le32 nxtseq_seed;
  __le32 status;
} __attribute__((preserve_access_index));

struct hwtstamp_config___local {
  int flags;
  int tx_type;
  int rx_filter;
} __attribute__((preserve_access_index));

/* Board specific private data structure */

struct callback_head {
	struct callback_head *next;
	void (*func)(struct callback_head *);
};

struct igc_adapter___local {
  struct net_device___local *netdev;

  unsigned long state;
  unsigned int flags;
  unsigned int num_q_vectors;
  u16 tx_ring_count;
  u16 rx_ring_count;

  u32 tx_hwtstamp_timeouts;
  u32 tx_hwtstamp_skipped;
  u32 rx_hwtstamp_cleared;

  struct hwtstamp_config___local tstamp_config;

} __attribute__((preserve_access_index));

struct igc_q_vector__local;
struct igc_tx_buffer__local;
struct igc_rx_buffer__local;

typedef u64 dma_addr_t;

struct igc_ring__local {
	struct igc_q_vector__local *q_vector;
	struct net_device__local *netdev;
	struct device__local *dev;
	union {
		struct igc_tx_buffer__local *tx_buffer_info;
		struct igc_rx_buffer__local *rx_buffer_info;
	};
	void *desc;
	long unsigned int flags;
	void *tail;
	dma_addr_t dma;
	unsigned int size;
	u16 count;
	u8 queue_index;
	u8 reg_idx;
	bool launchtime_enable;
	bool preemptible;
	ktime_t last_tx_cycle;
	ktime_t last_ff_cycle;
	u32 start_time;
	u32 end_time;
	bool cbs_enable;
	s32 idleslope;
	s32 sendslope;
	s32 hicredit;
	s32 locredit;
	u16 next_to_clean;
	u16 next_to_use;
	u16 next_to_alloc;
} __attribute__((preserve_access_index));


struct igc_ring_container__local {
	struct igc_ring__local *ring;
	unsigned int total_bytes;
	unsigned int total_packets;
	u16 work_limit;
	u8 count;
	u8 itr;
} __attribute__((preserve_access_index));

struct igc_q_vector {
	struct igc_adapter___local *adapter;
	void *itr_register;
	u32 eims_value;
	u16 itr_val;
	u8 set_itr;
	struct igc_ring_container__local rx;
	struct igc_ring_container__local tx;
	struct napi_struct___local napi;
	struct callback_head rcu;
	char name[25];
	long: 64;
	long: 64;
	long: 64;
	long: 64;
	long: 64;
	long: 64;
	long: 64;
};

/* Transmit Descriptor - Advanced */
union igc_adv_tx_desc___local {
  struct {
    __le64 buffer_addr;    /* Address of descriptor's data buf */
    __le32 cmd_type_len;
    __le32 olinfo_status;
  } read;
  struct {
    __le64 dma_tstamp;
    __le32 nxtseq_seed;
    __le32 status;
  } wb;
} __attribute__((preserve_access_index));

enum igc_tx_buffer_type__local {
  IGC_TX_BUFFER_TYPE_SKB,
  IGC_TX_BUFFER_TYPE_XDP,
  IGC_TX_BUFFER_TYPE_XSK,
};

/* wrapper around a pointer to a socket buffer,
 * so a DMA handle can be stored along with the buffer
 */
struct igc_tx_buffer___local {
  union igc_adv_tx_desc___local *next_to_watch;
  unsigned long time_stamp;
  enum igc_tx_buffer_type__local type;
  union {
    struct sk_buff___local *skb;
    struct xdp_frame___local *xdpf;
  };
  u32 tx_flags;
} __attribute__((preserve_access_index));

/* Context descriptors */
struct igc_adv_tx_context_desc___local {
  __le32 launch_time;
} __attribute__((preserve_access_index));


/* Receive Descriptor - Advanced */
typedef u64 dma_addr_t;

union igc_adv_rx_desc___local {
  struct {
    __le64 pkt_addr; /* Packet buffer address */
    __le64 hdr_addr; /* Header buffer address */
  } read;
  struct {
    struct {
      union {
	__le32 data;
	struct {
	  __le16 pkt_info; /*RSS type, Pkt type*/
	  /* Split Header, header buffer len */
	  __le16 hdr_info;
	} hs_rss;
      } lo_dword;
      union {
	__le32 rss; /* RSS Hash */
	struct {
	  __le16 ip_id; /* IP id */
	  __le16 csum; /* Packet Checksum */
	} csum_ip;
      } hi_dword;
    } lower;
    struct {
      __le32 status_error; /* ext status/error */
      __le16 length; /* Packet length */
      __le16 vlan; /* VLAN tag */
    } upper;
  } wb;  /* writeback */
} __attribute__((preserve_access_index));

struct igc_rx_buffer___local {
  dma_addr_t dma;
  __u16 pagecnt_bias;
} __attribute__((preserve_access_index));

struct filename___local {
	const char *name;/* pointer to actual string */
} __attribute__((preserve_access_index));

#endif
