/* SPDX-License-Identifier: GPL-2.0+ */

#define BPF_NO_GLOBAL_DATA
#include "vmlinux_local.h"

#include <linux/version.h>
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

#include <bpf/bpf_core_read.h> /* CO-RE */
#include <bpf/bpf_tracing.h> /* BPF_KPROBE */
#include "bpf-ipu7-ipu6-defs.h"

#ifndef bpf_target_defined
#warning "Tracing need __TARGET_ARCH_xxx defines"
#endif

char _license[] SEC("license") = "GPL";
u32 _version SEC("version") = LINUX_VERSION_CODE;

extern int LINUX_KERNEL_VERSION __kconfig;

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
SEC("kprobe/ipu7_fw_isys_complex_cmd")
int BPF_KPROBE(ipu_fw_isys_complex_cmd_entry, 	struct ipu7_isys___local *isys,
						const unsigned int stream_handle,
						void *cpu_mapped_buf,
						dma_addr_t dma_mapped_buf,
						size_t size,
						u16 send_type)
{
	u64 id;
	__u32 pid;

	if (send_type >= N_IPU_INSYS_SEND_TYPE)
		return -EINVAL;

	enum ipu7_insys_send_type s_type = (enum ipu7_insys_send_type) send_type;

	// get PID and TID of exiting thread/process
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;

	struct ipu7_isys_stream___local stream[1];
	BPF_CORE_READ_INTO(&stream[0], isys, streams[stream_handle]);
	struct ipu7_isys_stream___local *_stream = &stream[0];

	atomic_t sequence;
	int source;	// SSI stream source
	BPF_CORE_READ_INTO(&sequence,  _stream, sequence);
	BPF_CORE_READ_INTO(&source,  _stream, stream_source);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {
	  bpf_vprintk("graph_ent func=ipu_fw_isys_complex_cmd args=state_req=%s;stream=%d:%u;seq=%u",
		      ipu7_send_msg_types[s_type], source, stream_handle, sequence.counter);
	} else {
	  bpf_printk("graph_ret func=ipu_fw_isys_complex_cmd args=state_req=%s;streamid=%d:%u",
		      ipu7_send_msg_types[send_type], source, stream_handle);
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

SEC("kretprobe/ipu7_fw_isys_complex_cmd")
int BPF_KRETPROBE(ipu_fw_isys_complex_cmd_exit, int ret)
{

	bpf_printk("graph_ret func=ipu_fw_isys_complex_cmd ret=%d", ret);
	return 0;
}

/*
*/
SEC("kprobe/ipu7_fw_isys_simple_cmd")
int BPF_KPROBE(ipu_fw_isys_simple_cmd_entry, 	struct ipu7_isys___local *isys,
						const unsigned int stream_handle,
						u16 send_type)
{
	// get PID of exiting thread/process
	u64 id;
	__u32 pid;
	id = bpf_get_current_pid_tgid();
	pid = id >> 32;

	if (send_type >= N_IPU_INSYS_SEND_TYPE)
		return -EINVAL;

	enum ipu7_insys_send_type s_type = (enum ipu7_insys_send_type) send_type;
	/*
	*/
	struct ipu7_isys_stream___local stream[1];
	BPF_CORE_READ_INTO(&stream[0], isys, streams[stream_handle]);
	struct ipu7_isys_stream___local *_stream = &stream[0];
	atomic_t sequence;
	int source;	// SSI stream source

	BPF_CORE_READ_INTO(&sequence,  _stream, sequence);
	BPF_CORE_READ_INTO(&source,  _stream, stream_source);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {
	  bpf_vprintk("graph_ent func=ipu_fw_isys_simple_cmd args=state_req=%s;streamid=%d:%u;seq=%u",
		      ipu7_send_msg_types[s_type], source, stream_handle, sequence.counter);
	} else {
	  bpf_printk("graph_ent func=ipu_fw_isys_simple_cmd args=state_req=%s;streamid=%d:%u",
		      ipu7_send_msg_types[send_type], source, stream_handle);
	}

	for (u8 index = 0; index < IPU_ISYS_MAX_STREAMS; index++) {
	  if (index == (u8) stream_handle) {
	    g_state.prev_send_t[index] = send_type;
	    g_state.prev_source[index] = source;
	    g_state.prev_pid[index] = pid;
	    break;
	  }
	}
	/*
	*/
	return 0;
}

SEC("kretprobe/ipu7_fw_isys_simple_cmd")
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

	struct ipu7_isys_queue___local *aq = container_of(vbq, struct ipu7_isys_queue___local, vbq);
	struct ipu7_isys_video___local *av = container_of(aq, struct ipu7_isys_video___local, aq);

	struct ipu7_isys___local *isys;
	isys = (struct ipu7_isys___local *)BPF_CORE_READ(av, isys);

	unsigned int reset;
	BPF_CORE_READ_INTO(&reset,  av, reset);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {

	  unsigned int streaming;
	  BPF_CORE_READ_INTO(&streaming,  av, streaming);

	  int stream_handle;
	  int source;	// SSI stream source
	  BPF_CORE_READ_INTO(&stream_handle, av, stream, stream_handle);
	  BPF_CORE_READ_INTO(&source,  av, stream, stream_source);

	  bpf_vprintk("S|%u|ipu_isys_queue|0|streamid=%u:%u;call=buf_queue;queue_idx=%u;streaming=%d;reset_needed=%s|ipu7-trace",
		      pid,
		      source, stream_handle,
		      index, streaming, reset ? "true" : "false");
	} else {
	  bpf_printk("S|%u|ipu_isys_queue|0|call=buf_queue;queue_idx=%u;reset_needed=%s|ipu7-trace",
		      pid,
		      index, reset ? "true" : "false");
	}
	return 0;
}


SEC("kretprobe/buf_queue")
int BPF_KRETPROBE(buf_queue_exit, int ret)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	bpf_printk("F|%d|ipu_isys_queue|0|ret=%d|ipu7-trace", pid, ret);
	return 0;
}


/*
*/
SEC("kprobe/buffer_list_get")
int BPF_KPROBE(buffer_list_get_entry, struct ipu7_isys_stream___local *stream,
	       struct ipu7_isys_buffer_list___local *bl)
{
	unsigned int nbufs;
	BPF_CORE_READ_INTO(&nbufs,  bl, nbufs);

	atomic_t sequence;
	int source;	// SSI stream source
	int stream_handle;
	BPF_CORE_READ_INTO(&sequence,  stream, sequence);
	BPF_CORE_READ_INTO(&source,  stream, stream_source);
	BPF_CORE_READ_INTO(&stream_handle,  stream, stream_handle);

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

SEC("kprobe/return_buffers")
int BPF_KPROBE(return_buffers_entry, struct ipu7_isys_queue___local *aq,
				 enum vb2_buffer_state state)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	struct vb2_queue___local vbq;
	BPF_CORE_READ_INTO(&vbq,  aq, vbq);

	struct ipu7_isys_video___local *av = container_of(aq, struct ipu7_isys_video___local, aq);

	unsigned int reset;
	BPF_CORE_READ_INTO(&reset,  av, reset);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {

	  struct ipu7_isys_stream___local *stream;
	  stream = (struct ipu7_isys_stream___local *)BPF_CORE_READ(av, stream);
	  int source;	// SSI stream source
	  int stream_handle;

	  BPF_CORE_READ_INTO(&source,  stream, stream_source);
	  BPF_CORE_READ_INTO(&stream_handle,  stream, stream_handle);

	  bpf_vprintk("S|%u|ipu_isys_queue|0|streamid=%u:%u;call=return_buffers;used_qbuf=%u;uses_req=%u;state=%s|ipu7-trace",
		      pid,
		      source, stream_handle,
		      vbq.uses_qbuf,vbq.uses_requests,
		      vb2_buf_state_type[state]); // args
	} else {
	  bpf_printk("S|%u|ipu_isys_queue|0|call=return_buffers;used_qbuf=%u;state=%s|ipu7-trace",
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

	bpf_printk("F|%d|ipu_isys_queue|0|ret=%d|ipu7-trace", pid, ret);
	return 0;
}

/*
*/
SEC("kprobe/ipu7_isys_buffer_list_queue")
int BPF_KPROBE(ipu_isys_buffer_list_queue_entry, struct ipu7_isys_buffer_list___local *bl,
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


SEC("kretprobe/ipu7_isys_buffer_list_queue")
int BPF_KRETPROBE(ipu_isys_buffer_list_queue_exit, int ret)
{
	bpf_printk("graph_ret func=ipu_isys_buffer_list_queue ret=%d", ret);
	return 0;
}


/*
*/
SEC("kretprobe/ipu7_fw_isys_get_resp")
int BPF_KRETPROBE(ipu_fw_isys_get_resp_exit, struct ipu7_insys_resp___local *resp)
{
	struct ia_gofo_msg_err___local error_info;
	BPF_CORE_READ_INTO(&error_info,  resp, error_info);
	u64 buf_id;
	u8 pin_id;
	u8 stream_handle;
	u8 _type;
	enum ipu7_insys_resp_type type, prev_type;
	enum ipu7_insys_send_type send_type;
	const char *resp_error;
	BPF_CORE_READ_INTO(&stream_handle,  resp, stream_id);
	BPF_CORE_READ_INTO(&_type,  resp, type);
	BPF_CORE_READ_INTO(&buf_id,  resp, buf_id);
	BPF_CORE_READ_INTO(&pin_id,  resp, pin_id);

	type = (enum ipu7_insys_resp_type) _type;

	// assume IPU_FW_ISYS_IDLE initial state
	if (g_state.first) {
	  for (u8 index = 0; index < IPU_ISYS_MAX_STREAMS; index++)
	    g_state.state[index] = IPU_FW_ISYS_IDLE;
	}

	for (u8 index = 0; index < IPU_ISYS_MAX_STREAMS; index++) {
	  if (index == (u8) stream_handle) {
	    prev_type = (enum ipu7_insys_resp_type) g_state.prev_resp_t[index];
	    send_type = (enum ipu7_insys_send_type) g_state.prev_send_t[index];

	    u32 pid = g_state.prev_pid[index];
	    enum ipu_fw_isys_state _state = g_state.state[index];
	    enum ipu_fw_isys_state _state_next = g_state.state[index];
	    int _cmd_count_next = g_state.capture_cmd_count;
	    enum ipu7_insys_send_type _cmd_type = IPU_INSYS_SEND_TYPE_STREAM_CAPTURE;

	    unsigned int source = (unsigned int) g_state.prev_source[index];
	    if (prev_type != type) {
	      if (!g_state.first) {
		if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
		  bpf_vprintk("E|ipu_fw_isys;/dev ipu7|%s|ret=%s|ipu7-trace", ipu7_resp_msg_types[prev_type], "None");
		else
		  bpf_printk("E|ipu_fw_isys;/dev ipu7|%s|ret=%s|ipu7-trace", ipu7_resp_msg_types[prev_type], "None");
	      }

	      // IPU7 FW is single threaded state machine (FSM) :
	      // Can hold a maximum of 2 capture commands at any given point of time
	      // Keep a counter of concurrent IPU6 FW capture commands :
	      //    - increment by 1 from STREAM_START_AND_CAPTURE_ACK and STREAM_CAPTURE_ACK
	      //    - decrement by 1 from STREAM_START_AND_CAPTURE_DONE and STREAM_CAPTURE_DONE
	      //    - reset to 0 STREAM_FLUSH_ACK or STREAM_CLOSE_ACK
	      switch (type) {
	      case IPU_INSYS_RESP_TYPE_STREAM_CAPTURE_ACK:
	      case IPU_INSYS_RESP_TYPE_STREAM_START_AND_CAPTURE_ACK:
		_cmd_count_next++;
		_cmd_type = IPU_INSYS_SEND_TYPE_STREAM_CAPTURE;
		break;
	      case IPU_INSYS_RESP_TYPE_STREAM_CAPTURE_DONE:
	      case IPU_INSYS_RESP_TYPE_STREAM_START_AND_CAPTURE_DONE:
		_cmd_count_next--;
		_cmd_type = IPU_INSYS_SEND_TYPE_STREAM_CAPTURE;
		break;
	      case IPU_INSYS_RESP_TYPE_STREAM_ABORT_ACK:
	      case IPU_INSYS_RESP_TYPE_STREAM_FLUSH_ACK:
	      case IPU_INSYS_RESP_TYPE_STREAM_CLOSE_ACK:
		_cmd_count_next = 0;
		_cmd_type = IPU_INSYS_SEND_TYPE_STREAM_CAPTURE;
		break;
	      default:
		_cmd_count_next = g_state.capture_cmd_count;
	      }

	      resp_error = "NONE";
	      if (error_info.err_group < INSYS_MSG_ERR_GROUP_N) {
		switch (error_info.err_group) {
		case INSYS_MSG_ERR_GROUP_RESERVED:
			resp_error = "UNKNOWN";
			break;
		case INSYS_MSG_ERR_GROUP_GENERAL:
			if (error_info.err_code < IA_GOFO_MSG_ERR_GENERAL_N)
				resp_error = insys_msg_err_general_types[error_info.err_code];
			break;
		case INSYS_MSG_ERR_GROUP_STREAM:
			if (error_info.err_code < INSYS_MSG_ERR_STREAM_N)
				resp_error = insys_msg_err_stream_types[error_info.err_code];
			break;
		case INSYS_MSG_ERR_GROUP_CAPTURE:
			if (error_info.err_code < INSYS_MSG_ERR_CAPTURE_N)
				resp_error = insys_msg_err_capture_types[error_info.err_code];
			break;
		default:
			resp_error = "NONE";
		}
	      }

	      if (send_type < N_IPU_INSYS_SEND_TYPE) {
		if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
		  bpf_vprintk("B|ipu_fw_isys;/dev ipu7|%s|streamid=%u:%u;sent=%s;err=%s|ipu7-trace",
			      ipu7_resp_msg_types[type],
			      source, stream_handle,
			      ipu7_send_msg_types[send_type],
			      resp_error);
		else
		  bpf_printk("B|ipu_fw_isys;/dev ipu7|%s|streamid=%u|ipu7-trace",
			     ipu7_resp_msg_types[type],
			     stream_handle);

		if (error_info.err_code > 0) {
		  if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
		    bpf_vprintk("A|ipu_fw_isys;/dev ipu7|%s|streamid=%u:%u;sent=%s;resp=%s;code=%s|ipu7-trace",
				insys_msg_err_groups_types[error_info.err_group],
				source,stream_handle,
				ipu7_send_msg_types[send_type],
				ipu7_resp_msg_types[type],
				resp_error);
		  else
		    bpf_printk("A|ipu_fw_isys;/dev ipu7|%s|streamid=%u|ipu7-trace",
			       pid,
			       resp_error,
			       stream_handle);
		}
	      }

	      // mirror IPU7_ISYS internal state-machine based on ipu7 fw resp type
	      switch (_state) {
	      case IPU_FW_ISYS_UNINIT:
		if (type == IPU_INSYS_RESP_TYPE_STREAM_OPEN_DONE)
		  _state_next = IPU_FW_ISYS_IDLE;
		break;
	      case IPU_FW_ISYS_IDLE:
		if (type == IPU_INSYS_RESP_TYPE_STREAM_CAPTURE_ACK ||
		    type == IPU_INSYS_RESP_TYPE_STREAM_START_AND_CAPTURE_ACK)
		  _state_next = IPU_FW_ISYS_IN_TRANFER;
		else if (type == IPU_INSYS_RESP_TYPE_STREAM_CLOSE_ACK)
		  _state_next = IPU_FW_ISYS_UNINIT;
		break;
	      case IPU_FW_ISYS_IN_TRANFER:
		if (type == IPU_INSYS_RESP_TYPE_STREAM_FLUSH_ACK ||
		    type == IPU_INSYS_RESP_TYPE_STREAM_ABORT_ACK)
		  _state_next = IPU_FW_ISYS_IDLE;
		break;
	      default:
		_state_next = IPU_FW_ISYS_UNINIT;
	      }

	      if (_state != _state_next) {
		if (!g_state.first) {
		  bpf_printk("E|ipu_fw_isys/fsm%u;/dev ipu7|%s||ipu7-trace",
			      index,
			      ipu_isys_state_msg[_state]);
		}
		if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0))
		  bpf_vprintk("B|ipu_fw_isys/fsm%u;/dev ipu7|%s|streamid=%u:%u|ipu7-trace",
			      index,
			      ipu_isys_state_msg[_state_next],
			      source,stream_handle);
		else
		  bpf_vprintk("B|ipu_fw_isys/fsm%u;/dev ipu7|%s||ipu7-trace",
			      index,
			      ipu_isys_state_msg[_state_next]);

		g_state.state[index] = _state_next;
	      }

	      // mirror IPU7_ISYS internal state-machine ipu7 fw capture-cmd buffering
	      if (_cmd_count_next != g_state.capture_cmd_count) {
		bpf_printk("C|ipu_fw_isys;/dev ipu7|%s/count|%d|ipu7-trace",
			   ipu7_send_msg_types[_cmd_type],
			   _cmd_count_next);
		g_state.capture_cmd_count = _cmd_count_next;
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
SEC("kprobe/ipu7_isys_video_set_streaming")
int BPF_KPROBE(ipu_isys_video_set_streaming_entry, struct ipu7_isys_video___local *av,
				 unsigned int state,
				 struct ipu7_isys_buffer_list___local *bl)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {

	  struct ipu7_isys_stream___local *stream;
	  stream = (struct ipu7_isys_stream___local *)BPF_CORE_READ(av, stream);
	  int source;	// SSI stream source
	  int stream_handle;

	  BPF_CORE_READ_INTO(&source,  stream, stream_source);
	  BPF_CORE_READ_INTO(&stream_handle,  stream, stream_handle);

	  unsigned int nbufs;
	  BPF_CORE_READ_INTO(&nbufs,  bl, nbufs);

	  bpf_vprintk("S|%u|ipu_isys_video|0|streamid=%u:%u;call=ipu_isys_video_set_streaming;state=%u;nbuf=%d|ipu7-trace",
		      pid,
		      source,stream_handle,
		      state, nbufs);
	} else {
	  bpf_printk("S|%u|ipu_isys_video|0|state=%u;call=ipu_isys_video_set_streaming|ipu7-trace",
		      pid,
		      state);
	}
	return 0;
}

SEC("kretprobe/ipu7_isys_video_set_streaming")
int BPF_KRETPROBE(ipu_isys_video_set_streaming_exit, int ret)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	bpf_printk("F|%u|ipu_isys_video|0|ret=%d|ipu7-trace", pid, ret);
	return 0;
}

/*
*/
SEC("kprobe/start_stream_firmware")
int BPF_KPROBE(start_stream_firmware_entry, struct ipu7_isys_video___local *av,
				 struct ipu7_isys_buffer_list___local *bl)
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

	  struct ipu7_isys_stream___local *stream;
	  stream = (struct ipu7_isys_stream___local *)BPF_CORE_READ(av, stream);
	  int source;	// SSI stream source
	  int stream_handle;

	  BPF_CORE_READ_INTO(&source,  stream, stream_source);
	  BPF_CORE_READ_INTO(&stream_handle,  stream, stream_handle);

	  bpf_vprintk("S|%u|ipu_isys_video|0|streamid=%u:%u;call=start_stream_firmware;reset=%d|ipu7-trace",
		      pid,
		      source,stream_handle,reset); // args
	} else {
	  bpf_printk("S|%u|ipu_isys_video|0|call=start_stream_firmware;stream=%u;reset=%u|ipu7-trace",
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

	bpf_printk("F|%d|ipu_isys_video|0|ret=%d|ipu7-trace", pid, ret);
	return 0;
}


/*
*/
SEC("kprobe/stop_streaming_firmware")
int BPF_KPROBE(stop_stream_firmware_entry, struct ipu7_isys_video___local *av)
{
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	unsigned int streaming;
	unsigned int start_streaming;
	BPF_CORE_READ_INTO(&streaming,  av, streaming);
	BPF_CORE_READ_INTO(&start_streaming,  av, start_streaming);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {

	  struct ipu7_isys_stream___local *stream;
	  stream = (struct ipu7_isys_stream___local *)BPF_CORE_READ(av, stream);
	  int source;	// SSI stream source
	  int stream_handle;

	  BPF_CORE_READ_INTO(&source,  stream, stream_source);
	  BPF_CORE_READ_INTO(&stream_handle,  stream, stream_handle);

	  bpf_vprintk("S|%u|ipu_isys_video|0|streamid=%u:%u;call=stop_stream_firmware;active=%d;start=%d|ipu7-trace",
		      pid,
		      source, stream_handle,
		      streaming, start_streaming);

	} else {
	  bpf_printk("S|%u|ipu_isys_video|0|call=stop_stream_firmware;active=%d;started=%d|ipu7-trace",
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

	bpf_printk("F|%d|ipu_isys_video|0|ret=%d|ipu7-trace", pid, ret);
	return 0;
}

/*
*/
SEC("kprobe/close_streaming_firmware")
int BPF_KPROBE(close_stream_firmware_entry, struct ipu7_isys_video___local *av)
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

	  struct ipu7_isys_stream___local *stream;
	  stream = (struct ipu7_isys_stream___local *)BPF_CORE_READ(av, stream);
	  int source;	// SSI stream source
	  int stream_handle;

	  BPF_CORE_READ_INTO(&source,  stream, stream_source);
	  BPF_CORE_READ_INTO(&stream_handle,  stream, stream_handle);

	  bpf_vprintk("S|%u|ipu_isys_video|0|streamid=%u:%u;call=close_stream_firmware;active=%d;reset=%d;skip=%d|ipu7-trace",
		      pid,
		      source, stream_handle,
		      streaming, reset, skipframe);
	} else {

	  bpf_printk("S|%u|ipu_isys_video|0|call=close_stream_firmware;active=%d;reset=%d|ipu7-trace",
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

	bpf_printk("F|%d|ipu_isys_video|0|ret=%d|ipu7-trace", pid, ret);
	return 0;
}
