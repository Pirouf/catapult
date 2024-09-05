/* SPDX-License-Identifier: GPL-2.0+ */

#define BPF_NO_GLOBAL_DATA
#include "vmlinux_local.h"

#include <linux/version.h>
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

#include <bpf/bpf_core_read.h> /* CO-RE */
#include <bpf/bpf_tracing.h> /* BPF_KPROBE */
#include "bpf-ipu6-defs.h"

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

struct ipu6_fw_state {
  enum ipu_fw_isys_send_type prev_send_t[IPU_ISYS_MAX_STREAMS];
  enum ipu_fw_isys_resp_type prev_resp_t[IPU_ISYS_MAX_STREAMS];
  unsigned int prev_source[IPU_ISYS_MAX_STREAMS];
  unsigned int prev_pid[IPU_ISYS_MAX_STREAMS];
  bool first;
} g_state = { .first = true };

/*
struct {
	__uint(type, BPF_MAP_TYPE_ARRAY);
	__uint(max_entries, IPU_ISYS_MAX_STREAMS);
	__uint(key_size, sizeof(int));
	__uint(value_size, sizeof(struct ipu6_state));
} ipu6_state_map SEC(".maps");
*/

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

#undef  bpf_vprintk  // See /usr/include/bpf/bpf_helpers.h
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
 * real kernel struct sk_buff. But only defining the members that the program
 * are interested in. The CO-RE infra-structure will remap the member offsets to
 * the correct offsets used by the kernel. This require adding the LLVM
 * attribute "preserve_access_index". Notice the triple-underscore after the
 * real struct name, which libbpf match and adjust prior to BPF-loading.
*/
SEC("kprobe/ipu_fw_isys_complex_cmd")
int BPF_KPROBE(ipu_fw_isys_complex_cmd_entry, struct ipu_isys___local *isys,
			const unsigned int stream_handle,
			void *cpu_mapped_buf,
			dma_addr_t dma_mapped_buf,
			size_t size, enum ipu_fw_isys_send_type send_type)
{
	u64 id;
	__u32 pid;
	//__u32 tid;
	//struct event *e;
	//struct task_struct___local *task;
	//u64 ts, *start_ts, start_time = 0;

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;
	//tid = (u32)id;

	// reserve sample from BPF ringbuf
	//e = bpf_ringbuf_reserve(&rb, sizeof(*e), 0);
	//if (!e)
	//  return 0;

	// fill out the sample with data
	//task = (struct task_struct___local *)bpf_get_current_task();
	//start_time = BPF_CORE_READ(task, start_time);
	//e->duration_ns = bpf_ktime_get_ns() - start_time;
	//e->pid = pid;
	//e->ppid = BPF_CORE_READ(task, real_parent, tgid);
	//bpf_get_current_comm(&e->comm, sizeof(e->comm));
	// send data to user-space for post-processing
	//bpf_ringbuf_submit(e, 0);

	// Find the kernels BTF_ID for struct ipu_isys
	//id = bpf_core_type_id_kernel(struct ipu_isys___local);

	//unsigned int stream_opened;
	//BPF_CORE_READ_INTO(&stream_opened,  isys, stream_opened);

	//struct ipu_bus_device___local *adev;
	//adev = (struct ipu_bus_device___local *)BPF_CORE_READ(isys, adev);
	//struct device___local dev;
	//BPF_CORE_READ_INTO(&dev,  adev, dev);

	//char driver_name[32];
	//BPF_CORE_READ_INTO(&driver_name,  media_dev, driver_name);

	//struct media_device___local media_dev;
	//BPF_CORE_READ_INTO(&media_dev,  isys, media_dev);

	//struct v4l2_device___local v4l2_dev;
	//BPF_CORE_READ_INTO(&v4l2_dev,  isys, v4l2_dev);

	struct ipu_isys_pipeline___local *pipes[IPU_ISYS_MAX_STREAMS];
	pipes[stream_handle] = (struct ipu_isys_pipeline___local *)BPF_CORE_READ(isys, pipes[stream_handle]);
	atomic_t sequence;
	int source;	/* SSI stream source */
	BPF_CORE_READ_INTO(&sequence,  pipes[stream_handle], sequence);
	BPF_CORE_READ_INTO(&source,  pipes[stream_handle], source);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {
	  bpf_vprintk("graph_ent func=ipu_fw_isys_complex_cmd args=state_req=%s;streamid=%d:%u;seq=%u",
		      send_msg_types[send_type], source, stream_handle, sequence.counter);
	} else {
	  bpf_printk("graph_ret func=ipu_fw_isys_complex_cmd args=state_req=%s;streamid=%d:%u",
		      send_msg_types[send_type], source, stream_handle);
	}

	for (u8 index=0; index < IPU_ISYS_MAX_STREAMS; index++) {
	  if (index == (u8) stream_handle) {
	    g_state.prev_send_t[index] = send_type;
	    g_state.prev_source[index] = source;
	    g_state.prev_pid[index] = pid;
	    break;
	  }
	}
	return 0;
}

SEC("kretprobe/ipu_fw_isys_complex_cmd")
int BPF_KRETPROBE(ipu_fw_isys_complex_cmd_exit, int ret)
{
	//u64 id;
	//u32 pid, tid;
	//struct event *e;
	//struct task_struct___local *task;
	//u64 ts, *start_ts, start_time = 0;

	// get PID and TID of exiting thread/process
	//id = bpf_get_current_pid_tgid();
	//pid = id >> 32;
	//tid = (u32)id;

	// reserve sample from BPF ringbuf
	//e = bpf_ringbuf_reserve(&rb, sizeof(*e), 0);
	//if (!e)
	//  return 0;

	// fill out the sample with data
	//task = (struct task_struct___local *)bpf_get_current_task();
	//start_time = BPF_CORE_READ(task, start_time);
	//e->duration_ns = bpf_ktime_get_ns() - start_time;
	//e->pid = pid;
	//e->ppid = BPF_CORE_READ(task, real_parent, tgid);
	//bpf_get_current_comm(&e->comm, sizeof(e->comm));
	// send data to user-space for post-processing
	//bpf_ringbuf_submit(e, 0);

	bpf_printk("graph_ret func=ipu_fw_isys_complex_cmd ret=%d", ret);
	return 0;
}

/*
*/
SEC("kprobe/ipu_fw_isys_simple_cmd")
int BPF_KPROBE(ipu_fw_isys_simple_cmd_entry, struct ipu_isys___local *isys,
			const unsigned int stream_handle,
			enum ipu_fw_isys_send_type send_type)
{
	// get PID of exiting thread/process
	u64 id;
	__u32 pid;
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;

	struct ipu_isys_pipeline___local *pipes[IPU_ISYS_MAX_STREAMS];
	pipes[stream_handle] = (struct ipu_isys_pipeline___local *)BPF_CORE_READ(isys, pipes[stream_handle]);
	atomic_t sequence;
	int source;	/* SSI stream source */
	BPF_CORE_READ_INTO(&sequence,  pipes[stream_handle], sequence);
	BPF_CORE_READ_INTO(&source,  pipes[stream_handle], source);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {
	  bpf_vprintk("graph_ent func=ipu_fw_isys_simple_cmd args=state_req=%s;streamid=%d:%u;seq=%u",
		      send_msg_types[send_type],
		      source, stream_handle, sequence.counter);
	} else {
	  bpf_printk("graph_ent func=ipu_fw_isys_simple_cmd args=state_req=%s;streamid=%d:%u",
		      send_msg_types[send_type], source, stream_handle);
	}

	for (u8 index = 0; index < IPU_ISYS_MAX_STREAMS; index++) {
	  if (index == (u8) stream_handle) {
	    g_state.prev_send_t[index] = send_type;
	    g_state.prev_source[index] = source;
	    g_state.prev_pid[index] = pid;
	    break;
	  }
	}
	return 0;
}

SEC("kretprobe/ipu_fw_isys_simple_cmd")
int BPF_KRETPROBE(ipu_fw_isys_simple_cmd_exit, int ret)
{
	bpf_printk("graph_ret func=ipu_fw_isys_simple_cmd ret=%d", ret);
	return 0;
}


/*
*/
SEC("kprobe/buf_queue")
int BPF_KPROBE(buf_queue_entry, struct vb2_buffer___local *vb)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	unsigned int index;
	BPF_CORE_READ_INTO(&index,  vb, index);

	struct vb2_queue___local *vbq;
	vbq = (struct vb2_queue___local *)BPF_CORE_READ(vb, vb2_queue);

	struct ipu_isys_queue___local *aq = container_of(vbq, struct ipu_isys_queue___local, vbq);
	struct ipu_isys_video___local *av = container_of(aq, struct ipu_isys_video___local, aq);

	struct ipu_isys___local *isys;
	isys = (struct ipu_isys___local *)BPF_CORE_READ(av, isys);

	bool reset_needed;
	BPF_CORE_READ_INTO(&reset_needed,  isys, reset_needed);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {

	  unsigned int reset;
	  BPF_CORE_READ_INTO(&reset,  av, reset);

	  struct ipu_isys_pipeline___local ip;
	  BPF_CORE_READ_INTO(&ip,  av, ip);

	  bpf_vprintk("S|%u|ipu_isys_queue|0|streamid=%u:%u;call=buf_queue;queue_idx=%u;nr_queues=%d;nr_streaming=%d;streaming=%d;reset_needed=%s|ipu6-trace",
		      pid,
		      ip.source, ip.stream_handle,
		      index, ip.nr_queues, ip.nr_streaming, ip.streaming,
		      reset_needed ? "true" : "false");
	} else {
	  bpf_printk("S|%u|ipu_isys_queue|0|call=buf_queue;queue_idx=%u;reset_needed=%s|ipu6-trace",
		      pid,
		      index,
		      reset_needed ? "true" : "false");
	}
	return 0;
}


SEC("kretprobe/buf_queue")
int BPF_KRETPROBE(buf_queue_exit, int ret)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	bpf_printk("F|%d|ipu_isys_queue|0|ret=%d|ipu6-trace", pid, ret);
	return 0;
}


/*
*/
SEC("kprobe/buffer_list_get")
int BPF_KPROBE(buffer_list_get_entry, struct ipu_isys_pipeline___local *ip,
	       struct ipu_isys_buffer_list___local *bl)
{
	unsigned int nbufs;
	BPF_CORE_READ_INTO(&nbufs,  bl, nbufs);

	atomic_t sequence;
	int source;	// SSI stream source
	int stream_handle;
	BPF_CORE_READ_INTO(&sequence,  ip, sequence);
	BPF_CORE_READ_INTO(&source,  ip, source);
	BPF_CORE_READ_INTO(&stream_handle,  ip, stream_handle);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {
	  bpf_vprintk("graph_ent func=buffer_list_get args=nbufs=%u;streamid=%d:%u;seq=%u",
		      nbufs,
		      source, stream_handle,
		      sequence.counter);
	} else {
	  bpf_printk("graph_ent func=buffer_list_get args=streamid=%d:%u;seq=%u",
		      source, stream_handle,
		      sequence.counter);
	}
	return 0;
}


SEC("kretprobe/buffer_list_get")
int BPF_KRETPROBE(buffer_list_get_exit, int ret)
{
	bpf_printk("graph_ret func=buffer_list_get ret=%d", ret);
	return 0;
}

/*
*/
SEC("kprobe/ipu_isys_buffer_list_queue")
int BPF_KPROBE(ipu_isys_buffer_list_queue_entry, struct ipu_isys_buffer_list___local *bl,
				unsigned long op_flags,
				enum vb2_buffer_state state)
{
	unsigned int nbufs;
	BPF_CORE_READ_INTO(&nbufs,  bl, nbufs);

	bpf_printk("graph_ent func=ipu_isys_buffer_list_queue args=nbufs=%u;flags=%lu;state=%s",
		   nbufs,
		   op_flags,
		   vb2_buf_state_type[state]);
	return 0;
}


SEC("kretprobe/ipu_isys_buffer_list_queue")
int BPF_KRETPROBE(ipu_isys_buffer_list_queue_exit, int ret)
{
	bpf_printk("graph_ret func=ipu_isys_buffer_list_queue ret=%d", ret);
	return 0;
}

/*
*/
SEC("kretprobe/ipu_fw_isys_get_resp")
int BPF_KRETPROBE(ipu_fw_isys_get_resp_exit, struct ipu_fw_isys_resp_info_abi___local *resp)
{
	struct ipu_fw_isys_error_info_abi___local error_info;
	BPF_CORE_READ_INTO(&error_info,  resp, error_info);
	char stream_handle;	/* SSI stream source */

	int buf_id;
	enum ipu_fw_isys_resp_type type, prev_type;
	enum ipu_fw_isys_send_type send_type;
	enum ipu_fw_isys_error resp_error = error_info.error;
	BPF_CORE_READ_INTO(&stream_handle,  resp, stream_handle);
	BPF_CORE_READ_INTO(&type,  resp, type);
	BPF_CORE_READ_INTO(&buf_id,  resp, buf_id);



	for (u8 index = 0; index < IPU_ISYS_MAX_STREAMS; index++) {
	  if (index == (u8) stream_handle) {
	    prev_type = g_state.prev_resp_t[index];
	    send_type = g_state.prev_send_t[index];
	    u32 pid = g_state.prev_pid[index];

	    unsigned int source = (unsigned int) g_state.prev_source[index];
	    if (prev_type != type) {
	      if (!g_state.first) {
		if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
		  bpf_vprintk("E|ipu_fw_isys;%u|%s|ret=%s|ipu6-trace", pid, resp_msg_types[prev_type], "None");
		else
		  bpf_printk("E|ipu_fw_isys;%u|%s|ret=%s|ipu6-trace", pid, resp_msg_types[prev_type], "None");
	      }

	      if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
		bpf_vprintk("B|ipu_fw_isys;%u|%s|streamid=%u:%u;sent=%s;error=%s|ipu6-trace",
			    pid,
			    resp_msg_types[type],
			    source,stream_handle,
			    send_msg_types[send_type],
			    isys_error_types[resp_error]);
	      else
		bpf_printk("B|ipu_fw_isys;%u|%s|streamid=%u|ipu6-trace",
			    pid,
			    resp_msg_types[type],
			    stream_handle);

	      if (resp_error > 0) {
		if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
		  bpf_vprintk("A|ipu_fw_isys;%u|%s|streamid=%u:%u;sent=%s;resp=%s|ipu6-trace",
			      pid,
			      isys_error_types[resp_error],			  
			      source,stream_handle,
			      send_msg_types[send_type],
			      resp_msg_types[type]);
		else
		  bpf_printk("A|ipu_fw_isys;%u|%s|streamid=%u|ipu6-trace",
			      pid,
			      isys_error_types[resp_error],			  
			      stream_handle);
	      }
	      if (g_state.first) g_state.first = false;
	    }
	    g_state.prev_resp_t[index] = type;
	    break;
	  }
	}

	return 0;
}


/*
*/
SEC("kprobe/ipu_isys_video_prepare_streaming")
int BPF_KPROBE(ipu_isys_video_prepare_streaming_entry, struct ipu_isys_video___local *av, unsigned int state)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {

	  struct ipu_isys_pipeline___local ip;
	  BPF_CORE_READ_INTO(&ip,  av, ip);

	  bpf_vprintk("S|%u|ipu_isys_video|0|streamid=%u:%u;call=ipu_isys_video_prepare_streaming;state=%u|ipu6-trace",
		      pid,
		      ip.source,
		      ip.stream_handle,
		      state); //args
	} else {
	    bpf_printk("S|%u|ipu_isys_video|0|call=ipu_isys_video_prepare_streaming;state=%u|ipu6-trace",
		       pid,
		       state); //args
	}
	return 0;
}

SEC("kretprobe/ipu_isys_video_prepare_streaming")
int BPF_KRETPROBE(ipu_isys_video_prepare_streaming_exit, int ret)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	bpf_printk("F|%u|ipu_isys_video|0|ret=%d|ipu6-trace", pid, ret);
	return 0;
}

/*
*/
SEC("kprobe/ipu_isys_video_set_streaming")
int BPF_KPROBE(ipu_isys_video_set_streaming_entry, struct ipu_isys_video___local *av,
				 unsigned int state,
				 struct ipu_isys_buffer_list___local *bl)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {

	  struct ipu_isys_pipeline___local ip;
	  BPF_CORE_READ_INTO(&ip,  av, ip);

	  unsigned int nbufs;
	  BPF_CORE_READ_INTO(&nbufs,  bl, nbufs);

	  bpf_vprintk("S|%u|ipu_isys_video|0|streamid=%u:%u;call=ipu_isys_video_set_streaming;state=%u;nbuf=%d|ipu6-trace",
		      pid,
		      ip.source,ip.stream_handle,
		      state, nbufs);
	} else { 
	  bpf_printk("S|%u|ipu_isys_video|0|state=%u;call=ipu_isys_video_set_streaming|ipu6-trace",
		      pid,
		      state);
	}
	return 0;
}

SEC("kretprobe/ipu_isys_video_set_streaming")
int BPF_KRETPROBE(ipu_isys_video_set_streaming_exit, int ret)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	bpf_printk("F|%u|ipu_isys_video|0|ret=%d|ipu6-trace", pid, ret);
	return 0;
}
/*
*/
SEC("kprobe/start_stream_firmware")
int BPF_KPROBE(start_stream_firmware_entry, struct ipu_isys_video___local *av,
				 struct ipu_isys_buffer_list___local *bl)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	unsigned int streaming;
	unsigned int reset;
	unsigned int skipframe;
	unsigned int start_streaming;
	BPF_CORE_READ_INTO(&streaming,  av, streaming);
	BPF_CORE_READ_INTO(&reset,  av, reset);
	BPF_CORE_READ_INTO(&skipframe,  av, skipframe);
	BPF_CORE_READ_INTO(&start_streaming,  av, start_streaming);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {

	  struct ipu_isys_pipeline___local ip;
	  BPF_CORE_READ_INTO(&ip,  av, ip);

	  bpf_vprintk("S|%u|ipu_isys_video|0|streamid=%u:%u;call=start_stream_firmware;reset=%d|ipu6-trace",
		      pid,
		      ip.source,ip.stream_handle,reset); // args
	} else {
	  bpf_printk("S|%u|ipu_isys_video|0|call=start_stream_firmware;stream=%u;reset=%u|ipu6-trace",
		      pid,
		      streaming,
		      reset); // args
	}
	return 0;
}

SEC("kretprobe/start_stream_firmware")
int BPF_KRETPROBE(start_stream_firmware_exit, int ret)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	bpf_printk("F|%d|ipu_isys_video|0|ret=%d|ipu6-trace", pid, ret);
	//bpf_printk("graph_ret func=start_stream_firmware ret=%d", ret);
	return 0;
}

/*
*/
SEC("kprobe/stop_streaming_firmware")
int BPF_KPROBE(stop_stream_firmware_entry, struct ipu_isys_video___local *av)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	unsigned int streaming;
	unsigned int start_streaming;
	BPF_CORE_READ_INTO(&streaming,  av, streaming);
	BPF_CORE_READ_INTO(&start_streaming,  av, start_streaming);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {

	  struct ipu_isys_pipeline___local ip;
	  BPF_CORE_READ_INTO(&ip,  av, ip);
	  bpf_vprintk("S|%u|ipu_isys_video|0|streamid=%u:%u;call=stop_stream_firmware;active=%d;start=%d|ipu6-trace",
		      pid,
		      ip.source, ip.stream_handle,
		      streaming, start_streaming);

	} else {
	  bpf_printk("S|%u|ipu_isys_video|0|call=stop_stream_firmware;active=%d;started=%d|ipu6-trace",
		     pid,
		     streaming,
		     start_streaming);
	}
	return 0;
}

SEC("kretprobe/stop_streaming_firmware")
int BPF_KRETPROBE(stop_stream_firmware_exit, int ret)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	bpf_printk("F|%d|ipu_isys_video|0|ret=%d|ipu6-trace", pid, ret);
	return 0;
}

/*
*/
SEC("kprobe/close_streaming_firmware")
int BPF_KPROBE(close_stream_firmware_entry, struct ipu_isys_video___local *av)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	unsigned int streaming;
	unsigned int reset;
	unsigned int skipframe;
	BPF_CORE_READ_INTO(&streaming,  av, streaming);
	BPF_CORE_READ_INTO(&reset,  av, reset);
	BPF_CORE_READ_INTO(&skipframe,  av, skipframe);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {

	  struct ipu_isys_pipeline___local ip;
	  BPF_CORE_READ_INTO(&ip,  av, ip);

	  bpf_vprintk("S|%u|ipu_isys_video|0|streamid=%u:%u;call=close_stream_firmware;active=%d;reset=%d;skip=%d|ipu6-trace",
		      pid,
		      ip.source, ip.stream_handle,
		      streaming, reset, skipframe);
	} else {

	  bpf_printk("S|%u|ipu_isys_video|0|call=close_stream_firmware;active=%d;reset=%d|ipu6-trace",
		      pid,
		      streaming, reset);
	}
	return 0;
}

SEC("kretprobe/close_streaming_firmware")
int BPF_KRETPROBE(close_stream_firmware_exit, int ret)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	bpf_printk("F|%d|ipu_isys_video|0|ret=%d|ipu6-trace", pid, ret);
	return 0;
}

/*
*/
SEC("kprobe/return_buffers")
int BPF_KPROBE(return_buffers_entry, struct ipu_isys_queue___local *aq,
				 enum vb2_buffer_state state)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	struct vb2_queue___local vbq;
	BPF_CORE_READ_INTO(&vbq,  aq, vbq);

	struct ipu_isys_video___local *av = container_of(aq, struct ipu_isys_video___local, aq);

	unsigned int reset;
	BPF_CORE_READ_INTO(&reset,  av, reset);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {

	  struct ipu_isys_pipeline___local ip;
	  BPF_CORE_READ_INTO(&ip,  av, ip);

	  bpf_vprintk("S|%u|ipu_isys_queue|0|streamid=%u:%u;call=return_buffers;used_qbuf=%u;uses_req=%u;state=%s|ipu6-trace",
		    pid,
		    ip.source, ip.stream_handle,
		    vbq.uses_qbuf,vbq.uses_requests,
		    vb2_buf_state_type[state]); // args
	} else {
	  bpf_printk("S|%u|ipu_isys_queue|0|call=return_buffers;used_qbuf=%u;state=%s|ipu6-trace",
		    pid,
		    vbq.uses_qbuf,
		    vb2_buf_state_type[state]); // args
	}
	return 0;
}

SEC("kretprobe/return_buffers")
int BPF_KRETPROBE(return_buffers_exit, int ret)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	bpf_printk("F|%d|ipu_isys_queue|0|ret=%d|ipu6-trace", pid, ret);
	//bpf_printk("graph_ret func=start_stream_firmware ret=%d", ret);
	return 0;
}
