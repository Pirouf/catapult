/* SPDX-License-Identifier: GPL-2.0+ */
#include <asm-generic/errno-base.h>

#include "ipu6-isys_ktrace_kern.c"
/*
 * PSYS 
*/
static const char* get_ppg_index_by_state(enum ipu_psys_ppg_state _state)
{
        enum ipu_psys_ppg_type_state ret = PPG_TYPE_STATE_START;

	switch (_state) {
	      case PPG_STATE_START:
		ret = PPG_TYPE_STATE_START;
		break;
	      case PPG_STATE_STARTING:
		ret = PPG_TYPE_STATE_STARTING;
		break;
	      case PPG_STATE_STARTED:
		ret = PPG_TYPE_STATE_STARTED;
		break;
	      case PPG_STATE_RUNNING:
		ret = PPG_TYPE_STATE_RUNNING;
		break;
	      case PPG_STATE_SUSPEND:
		ret = PPG_TYPE_STATE_SUSPEND;
		break;
	      case PPG_STATE_SUSPENDING:
		ret = PPG_TYPE_STATE_SUSPENDING;
		break;
	      case PPG_STATE_SUSPENDED:
		ret = PPG_TYPE_STATE_SUSPENDED;
		break;
	      case PPG_STATE_RESUME:
		ret = PPG_TYPE_STATE_RESUME;
		break;
	      case PPG_STATE_RESUMING:
		ret = PPG_TYPE_STATE_RESUMING;
		break;
	      case PPG_STATE_RESUMED:
		ret = PPG_TYPE_STATE_RESUMED;
		break;
	      case PPG_STATE_STOP:
		ret = PPG_TYPE_STATE_STOP;
		break;
	      case PPG_STATE_STOPPING:
		ret = PPG_TYPE_STATE_STOPPING;
		break;
	      case PPG_STATE_STOPPED:
		ret = PPG_TYPE_STATE_STOPPED;
		break;
	      default:
		ret = PPG_TYPE_STATE_START;
	}

        return ipu_psys_ppg_state_str[ret];
}
/*
*/
SEC("kprobe/ipu_psys_kcmd_new")
int BPF_KPROBE(ipu_psys_kcmd_new_entry,struct ipu_psys_command___local *cmd, struct ipu_psys_fh___local *fh){

	u32 issue_id;
	BPF_CORE_READ_INTO(&issue_id,  cmd, issue_id);
	u32 pg_size;
	BPF_CORE_READ_INTO(&pg_size,  cmd, pg_manifest_size);
	u32 pg_buf_cnt;
	BPF_CORE_READ_INTO(&pg_buf_cnt,  cmd, bufcount);
	u32 min_psys_freq;
	BPF_CORE_READ_INTO(&min_psys_freq,  cmd, min_psys_freq);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {
	
	  bpf_vprintk("graph_ent func=ipu_psys_kcmd_new args=issue_id=%x;ppg_sz=%u;buf_cnt=%u;min_freq=%u",
		      issue_id, pg_size,
		      pg_buf_cnt,
		      min_psys_freq);
	} else { 
	  bpf_printk("graph_ent func=ipu_psys_kcmd_new args=issue_id=%x;ppg_sz=%u;buf_cnt=%u",
		      issue_id, pg_size,
		      pg_buf_cnt);
	}
	return 0;
}

SEC("kretprobe/ipu_psys_kcmd_new")
int BPF_KRETPROBE(ipu_psys_kcmd_new_exit, int ret)
{
	bpf_printk("graph_ret func=ipu_psys_kcmd_new ret=%d", ret);
	return 0;
}

/* BPF perfbuf map */
struct {
	__uint(type, BPF_MAP_TYPE_PERF_EVENT_ARRAY);
	__uint(max_entries, 64);
	__type(key, int);
	__type(value, u32);
} counters SEC(".maps");

SEC("kprobe/ipu_psys_kcmd_start")
int BPF_KPROBE(ipu_psys_kcmd_start_entry,struct ipu_psys___local *psys, struct ipu_psys_kcmd___local *kcmd){


	enum ipu_psys_cmd_state old_kcmd_state;
 	BPF_CORE_READ_INTO(&old_kcmd_state,  kcmd, state);

	struct ipu_psys_pg___local *kpg;
	kpg = (struct ipu_psys_pg___local *)BPF_CORE_READ(kcmd, kpg);
	size_t pg_size;
	BPF_CORE_READ_INTO(&pg_size,  kpg, pg_size);
	
	struct ipu_fw_psys_process_group___local *pg;
	pg = (struct ipu_fw_psys_process_group___local *)BPF_CORE_READ(kpg, pg);

	u32 ppg_id;
	BPF_CORE_READ_INTO(&ppg_id,  pg, ID);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {
	
	  bpf_vprintk("graph_ent func=ipu_psys_kcmd_start args=kcmd_st=%s;ppg_id=%u;ppg_sz=%u",
		      ipu_psys_cmd_state_types[old_kcmd_state],
		      ppg_id, pg_size);
	} else { 
	  bpf_printk("graph_ent func=ipu_psys_kcmd_start args=kcmd_st=%s;ppg_id=%u;ppg_sz=%u",
		     ipu_psys_cmd_state_types[old_kcmd_state],
		      ppg_id, pg_size);
	}
	u32 cpu = bpf_get_smp_processor_id();
	struct bpf_perf_event_value value_buf;
	int ret;

	ret = bpf_perf_event_read_value(&counters, cpu, &value_buf, sizeof(value_buf));
	if (ret)
		return 0;
	bpf_vprintk("C|CPU %u:perf;Kernel|%s|%llx|ipu6-trace", cpu, "counter", value_buf.counter);
	return 0;
}

SEC("kretprobe/ipu_psys_kcmd_start")
int BPF_KRETPROBE(ipu_psys_kcmd_start_exit, int ret)
{
	bpf_printk("graph_ret func=ipu_psys_kcmd_start ret=%d", ret);
	return 0;
}

SEC("kprobe/ipu_psys_kcmd_complete")
int BPF_KPROBE(ipu_psys_kcmd_complete_entry,struct ipu_psys_ppg___local *kppg,
	       struct ipu_psys_kcmd___local *kcmd, int error){

	enum ipu_psys_ppg_state old_ppg_state;
	BPF_CORE_READ_INTO(&old_ppg_state,  kppg, state);
	const char *ppg_state_str =  get_ppg_index_by_state(old_ppg_state);
	/*
	 * Respond kcmd when ppg is in stable state:
	 * STARTED/RESUMED/RUNNING/SUSPENDED/STOPPED
	 */

	struct ipu_psys_pg___local *kpg;
	kpg = (struct ipu_psys_pg___local *)BPF_CORE_READ(kppg, kpg);
	size_t pg_size;
	BPF_CORE_READ_INTO(&pg_size,  kpg, pg_size);
	
	struct ipu_fw_psys_process_group___local *pg;
	pg = (struct ipu_fw_psys_process_group___local *)BPF_CORE_READ(kpg, pg);

	u32 ppg_id;
	BPF_CORE_READ_INTO(&ppg_id,  pg, ID);

	enum ipu_psys_cmd_state old_kcmd_state;
 	BPF_CORE_READ_INTO(&old_kcmd_state,  kcmd, state);

	if (error < 0) {
	  bpf_printk("A|ipu_psys;/dev ipu6|%s|kcmd_st=%s;ppg_st=%s|ipu6-trace",
		     error == -EIO ? "-EIO" : "Unkown" ,
		     ipu_psys_cmd_state_types[old_kcmd_state],
		     ppg_state_str);
	}
	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {
	
	  bpf_vprintk("graph_ent func=ipu_psys_kcmd_complete args=kcmd_st=%s;ppg_id=%u;ppg_sz=%u;ppg_st=%s;error=%d",
		      ipu_psys_cmd_state_types[old_kcmd_state],
		      ppg_id, pg_size,
		      ppg_state_str,
		      error);
	} else { 
	  bpf_printk("graph_ent func=ipu_psys_kcmd_complete args=kcmd_st=%s;ppg_st=%s;error=%d",
		   ipu_psys_cmd_state_types[old_kcmd_state],
		   ppg_state_str,
		   error);
	}
	
	/*
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;
	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {
	
	  bpf_vprintk("S|%u|ipu_psys_scheduler|0|kcmd_st=%s;call=ipu_psys_kcmd_complete;ppg_st=%s|ipu6-trace",
		      pid,
		      ipu_psys_cmd_state_types[old_kcmd_state],
		      ipu_psys_ppg_state_str[ppg_type_state]);
	} else { 
	  bpf_printk("S|%u|ipu_psys_scheduler|0|kcmd_st=%s;call=ipu_psys_kcmd_complete;ppg_st=%s|ipu6-trace",
		      pid,
		      ipu_psys_cmd_state_types[old_kcmd_state],
		      ipu_psys_ppg_state_str[ppg_type_state]);
	}
	*/
	return 0;
}

SEC("kretprobe/ipu_psys_kcmd_complete")
int BPF_KRETPROBE(ipu_psys_kcmd_complete_exit, int ret)
{
	bpf_printk("graph_ret func=ipu_psys_kcmd_complete ret=%d", ret);
	/*
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;
	bpf_printk("F|%u|ipu_psys_scheduler|0|ret=%d|ipu6-trace", pid, ret);
	*/
	return 0;
}

SEC("kprobe/ipu_psys_handle_events")
int BPF_KPROBE(ipu_psys_handle_events_entry, struct ipu_psys___local *psys){

	// assume PPG_STATE_STOPPED initial state  
	if (g_state.ppg_first) {
	  for (u8 index = 0; index < IPU6_FW_PSYS_N_PSYS_CMD_QUEUE_ID; index++) {
	    g_state.prev_ppg_state[index] = PPG_STATE_STOPPED;
	    g_state.prev_ppg_id[index] = 0;
	  }
	}
	
	return 0;
}

SEC("kprobe/ipu_psys_ppg_resume")
void BPF_KPROBE(ipu_psys_ppg_resume_entry, struct ipu_psys_ppg___local *kppg){
  
	enum ipu_psys_ppg_state old_ppg_state;
 	BPF_CORE_READ_INTO(&old_ppg_state,  kppg, state);
	const char *ppg_state_str =  get_ppg_index_by_state(old_ppg_state);

	struct ipu_psys_pg___local *kpg;
	kpg = (struct ipu_psys_pg___local *)BPF_CORE_READ(kppg, kpg);
	size_t pg_size;
	BPF_CORE_READ_INTO(&pg_size,  kpg, pg_size);
	
	struct ipu_fw_psys_process_group___local *pg;
	pg = (struct ipu_fw_psys_process_group___local *)BPF_CORE_READ(kpg, pg);

	u32 ppg_id;
	BPF_CORE_READ_INTO(&ppg_id,  pg, ID);
	u8 base_queue_id;
	BPF_CORE_READ_INTO(&base_queue_id,  pg, base_queue_id);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {
	
	  bpf_vprintk("graph_ent func=ipu_psys_ppg_resume args=ppg_id=%u;ppg_sz=%u;ppg_st=%s",
		      ppg_id, pg_size,
		      ppg_state_str);
	} else { 
	  bpf_printk("graph_ent func=ipu_psys_ppg_resume args=ppg_id=%u;ppg_sz=%u;ppg_st=%s",
		      ppg_id, pg_size,
		      ppg_state_str);
	}

	if ( g_state.prev_ppg_state[base_queue_id] == old_ppg_state && 
	     g_state.prev_ppg_id[base_queue_id] == ppg_id)  return;

	enum ipu_psys_ppg_state prev_state;
	prev_state = g_state.prev_ppg_state[base_queue_id];
	const char *prev_ppg_state_str = get_ppg_index_by_state(prev_state);

	u32 prev_ppg_id;
	prev_ppg_id = g_state.prev_ppg_id[base_queue_id];
	
	if (!g_state.ppg_first) {
	    bpf_printk("E|ipu_fw_psys/handle_events;/dev ipu6|%s|ppg_id=%u;queue_id=%u|ipu6-trace",
		       prev_ppg_state_str,
		       prev_ppg_id,
		       base_queue_id);
	}
	bpf_printk("B|ipu_fw_psys/handle_events;/dev ipu6|%s|ppg_id=%u;queue_id=%u|ipu6-trace",
		   ppg_state_str,
		   ppg_id,
		   base_queue_id);

	if (g_state.ppg_first) g_state.ppg_first = false;
	g_state.prev_ppg_state[base_queue_id] = old_ppg_state;
	g_state.prev_ppg_id[base_queue_id] = ppg_id;
	
	return;
}

SEC("kretprobe/ipu_psys_ppg_resume")
void BPF_KRETPROBE(ipu_psys_ppg_resume_exit, int ret)
{
	bpf_printk("graph_ret func=ipu_psys_ppg_resume ret=%d", ret);
}


SEC("kprobe/ipu_psys_ppg_start")
void BPF_KPROBE(ipu_psys_ppg_start_entry, struct ipu_psys_ppg___local *kppg){
  
	enum ipu_psys_ppg_state old_ppg_state;
 	BPF_CORE_READ_INTO(&old_ppg_state,  kppg, state);
	const char *ppg_state_str =  get_ppg_index_by_state(old_ppg_state);

	struct ipu_psys_pg___local *kpg;
	kpg = (struct ipu_psys_pg___local *)BPF_CORE_READ(kppg, kpg);
	size_t pg_size;
	BPF_CORE_READ_INTO(&pg_size,  kpg, pg_size);
	
	struct ipu_fw_psys_process_group___local *pg;
	pg = (struct ipu_fw_psys_process_group___local *)BPF_CORE_READ(kpg, pg);

	u32 ppg_id;
	BPF_CORE_READ_INTO(&ppg_id,  pg, ID);
	u8 base_queue_id;
	BPF_CORE_READ_INTO(&base_queue_id,  pg, base_queue_id);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {
	
	  bpf_vprintk("graph_ent func=ipu_psys_ppg_start args=ppg_id=%u;queue_id=%u;ppg_sz=%u;ppg_st=%s",
		      ppg_id, base_queue_id, pg_size,
		      ppg_state_str);
	} else { 
	  bpf_printk("graph_ent func=ipu_psys_ppg_start args=ppg_id=%u;queue_id=%u;ppg_st=%s",
		      ppg_id, base_queue_id,
		      ppg_state_str);
	}

	if ( g_state.prev_ppg_state[base_queue_id] == old_ppg_state && 
	     g_state.prev_ppg_id[base_queue_id] == ppg_id)  return;

	enum ipu_psys_ppg_state prev_state;
	prev_state = g_state.prev_ppg_state[base_queue_id];
	const char *prev_ppg_state_str = get_ppg_index_by_state(prev_state);

	u32 prev_ppg_id;
	prev_ppg_id = g_state.prev_ppg_id[base_queue_id];
	
	if (!g_state.ppg_first) {
	    bpf_printk("E|ipu_fw_psys/handle_events;/dev ipu6|%s|ppg_id=%u;queue_id=%u|ipu6-trace",
		       prev_ppg_state_str,
		       prev_ppg_id,
		       base_queue_id);
	}
	bpf_printk("B|ipu_fw_psys/handle_events;/dev ipu6|%s|ppg_id=%u;queue_id=%u|ipu6-trace",
		   ppg_state_str,
		   ppg_id,
		   base_queue_id);

	if (g_state.ppg_first) g_state.ppg_first = false;
	g_state.prev_ppg_state[base_queue_id] = old_ppg_state;
	g_state.prev_ppg_id[base_queue_id] = ppg_id;
	return;
}

SEC("kretprobe/ipu_psys_ppg_start")
void BPF_KRETPROBE(ipu_psys_ppg_start_exit, int ret)
{
	bpf_printk("graph_ret func=ipu_psys_ppg_start ret=%d", ret);
}


SEC("kprobe/ipu_psys_ppg_stop")
void BPF_KPROBE(ipu_psys_ppg_stop_entry, struct ipu_psys_ppg___local *kppg){
  
	enum ipu_psys_ppg_state old_ppg_state;
 	BPF_CORE_READ_INTO(&old_ppg_state,  kppg, state);
	const char *ppg_state_str =  get_ppg_index_by_state(old_ppg_state);

	struct ipu_psys_pg___local *kpg;
	kpg = (struct ipu_psys_pg___local *)BPF_CORE_READ(kppg, kpg);
	size_t pg_size;
	BPF_CORE_READ_INTO(&pg_size,  kpg, pg_size);
	
	struct ipu_fw_psys_process_group___local *pg;
	pg = (struct ipu_fw_psys_process_group___local *)BPF_CORE_READ(kpg, pg);

	u32 ppg_id;
	BPF_CORE_READ_INTO(&ppg_id,  pg, ID);
	u8 base_queue_id;
	BPF_CORE_READ_INTO(&base_queue_id,  pg, base_queue_id);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {
	
	  bpf_vprintk("graph_ent func=ipu_psys_ppg_stop args=ppg_id=%u;queue_id=%u;ppg_sz=%u;ppg_st=%s",
		      ppg_id, base_queue_id, pg_size,
		      ppg_state_str);
	} else { 
	  bpf_printk("graph_ent func=ipu_psys_ppg_stop args=ppg_id=%u;queue_id=%u;ppg_st=%s",
		      ppg_id, base_queue_id,
		      ppg_state_str);
	}

	if ( g_state.prev_ppg_state[base_queue_id] == old_ppg_state && 
	     g_state.prev_ppg_id[base_queue_id] == ppg_id)  return;

	enum ipu_psys_ppg_state prev_state;
	prev_state = g_state.prev_ppg_state[base_queue_id];
	const char *prev_ppg_state_str = get_ppg_index_by_state(prev_state);

	u32 prev_ppg_id;
	prev_ppg_id = g_state.prev_ppg_id[base_queue_id];
	
	if (!g_state.ppg_first) {
	    bpf_printk("E|ipu_fw_psys/handle_events;/dev ipu6|%s|ppg_id=%u;queue_id=%u|ipu6-trace",
		       prev_ppg_state_str,
		       prev_ppg_id,
		       base_queue_id);
	}
	bpf_printk("B|ipu_fw_psys/handle_events;/dev ipu6|%s|ppg_id=%u;queue_id=%u|ipu6-trace",
		   ppg_state_str,
		   ppg_id,
		   base_queue_id);

	if (g_state.ppg_first) g_state.ppg_first = false;
	g_state.prev_ppg_state[base_queue_id] = old_ppg_state;
	g_state.prev_ppg_id[base_queue_id] = ppg_id;
	return;
}

SEC("kretprobe/ipu_psys_ppg_stop")
void BPF_KRETPROBE(ipu_psys_ppg_stop_exit, int ret)
{
	bpf_printk("graph_ret func=ipu_psys_ppg_stop ret=%d", ret);
}

SEC("kprobe/ipu_psys_ppg_suspend")
void BPF_KPROBE(ipu_psys_ppg_suspend_entry, struct ipu_psys_ppg___local *kppg){
  
	enum ipu_psys_ppg_state old_ppg_state;
 	BPF_CORE_READ_INTO(&old_ppg_state,  kppg, state);
	const char *ppg_state_str =  get_ppg_index_by_state(old_ppg_state);

	struct ipu_psys_pg___local *kpg;
	kpg = (struct ipu_psys_pg___local *)BPF_CORE_READ(kppg, kpg);
	size_t pg_size;
	BPF_CORE_READ_INTO(&pg_size,  kpg, pg_size);
	
	struct ipu_fw_psys_process_group___local *pg;
	pg = (struct ipu_fw_psys_process_group___local *)BPF_CORE_READ(kpg, pg);

	u32 ppg_id;
	BPF_CORE_READ_INTO(&ppg_id,  pg, ID);
	u8 base_queue_id;
	BPF_CORE_READ_INTO(&base_queue_id,  pg, base_queue_id);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {
	
	  bpf_vprintk("graph_ent func=ipu_psys_ppg_suspend args=ppg_id=%u;queue_id=%u;ppg_sz=%u;ppg_st=%s",
		      ppg_id, base_queue_id, pg_size,
		      ppg_state_str);
	} else { 
	  bpf_printk("graph_ent func=ipu_psys_ppg_suspend args=ppg_id=%u;queue_id=%u;ppg_st=%s",
		      ppg_id, base_queue_id,
		      ppg_state_str);
	}

	if ( g_state.prev_ppg_state[base_queue_id] == old_ppg_state && 
	     g_state.prev_ppg_id[base_queue_id] == ppg_id)  return;

	enum ipu_psys_ppg_state prev_state;
	prev_state = g_state.prev_ppg_state[base_queue_id];
	const char *prev_ppg_state_str = get_ppg_index_by_state(prev_state);

	u32 prev_ppg_id;
	prev_ppg_id = g_state.prev_ppg_id[base_queue_id];
	
	if (!g_state.ppg_first) {
	    bpf_printk("E|ipu_fw_psys/handle_events;/dev ipu6|%s|ppg_id=%u;queue_id=%u|ipu6-trace",
		       prev_ppg_state_str,
		       prev_ppg_id,
		       base_queue_id);
	}
	bpf_printk("B|ipu_fw_psys/handle_events;/dev ipu6|%s|ppg_id=%u;queue_id=%u|ipu6-trace",
		   ppg_state_str,
		   ppg_id,
		   base_queue_id);

	if (g_state.ppg_first) g_state.ppg_first = false;
	g_state.prev_ppg_state[base_queue_id] = old_ppg_state;
	g_state.prev_ppg_id[base_queue_id] = ppg_id;
	return;
}

SEC("kretprobe/ipu_psys_ppg_suspend")
void BPF_KRETPROBE(ipu_psys_ppg_suspend_exit, int ret)
{
	bpf_printk("graph_ret func=ipu_psys_ppg_suspend ret=%d", ret);
}

SEC("kprobe/ipu_psys_ppg_enqueue_bufsets")
void BPF_KPROBE(ipu_psys_ppg_enqueue_bufsets_entry, struct ipu_psys_ppg___local *kppg){
  
	enum ipu_psys_ppg_state old_ppg_state;
 	BPF_CORE_READ_INTO(&old_ppg_state,  kppg, state);
	const char *ppg_state_str =  get_ppg_index_by_state(old_ppg_state);

	struct ipu_psys_pg___local *kpg;
	kpg = (struct ipu_psys_pg___local *)BPF_CORE_READ(kppg, kpg);
	size_t pg_size;
	BPF_CORE_READ_INTO(&pg_size,  kpg, pg_size);
	
	struct ipu_fw_psys_process_group___local *pg;
	pg = (struct ipu_fw_psys_process_group___local *)BPF_CORE_READ(kpg, pg);

	u32 ppg_id;
	BPF_CORE_READ_INTO(&ppg_id,  pg, ID);
	u8 base_queue_id;
	BPF_CORE_READ_INTO(&base_queue_id,  pg, base_queue_id);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {
	
	  bpf_vprintk("graph_ent func=ipu_psys_ppg_enqueue_bufsets args=ppg_id=%u;queue_id=%u;ppg_sz=%u;ppg_st=%s",
		      ppg_id, base_queue_id, pg_size,
		      ppg_state_str);
	} else { 
	  bpf_printk("graph_ent func=ipu_psys_ppg_enqueue_bufsets args=ppg_id=%u;queue_id=%u;ppg_st=%s",
		      ppg_id, base_queue_id, 
		      ppg_state_str);
	}

	if ( g_state.prev_ppg_state[base_queue_id] == old_ppg_state && 
	     g_state.prev_ppg_id[base_queue_id] == ppg_id)  return;

	enum ipu_psys_ppg_state prev_state;
	prev_state = g_state.prev_ppg_state[base_queue_id];
	const char *prev_ppg_state_str = get_ppg_index_by_state(prev_state);

	u32 prev_ppg_id;
	prev_ppg_id = g_state.prev_ppg_id[base_queue_id];
	
	if (!g_state.ppg_first) {
	    bpf_printk("E|ipu_fw_psys/handle_events;/dev ipu6|%s|ppg_id=%u;queue_id=%u|ipu6-trace",
		       prev_ppg_state_str,
		       prev_ppg_id,
		       base_queue_id);
	}
	bpf_printk("B|ipu_fw_psys/handle_events;/dev ipu6|%s|ppg_id=%u;queue_id=%u|ipu6-trace",
		   ppg_state_str,
		   ppg_id,
		   base_queue_id);

	if (g_state.ppg_first) g_state.ppg_first = false;
	g_state.prev_ppg_state[base_queue_id] = old_ppg_state;
	g_state.prev_ppg_id[base_queue_id] = ppg_id;
	return;
}

SEC("kretprobe/ipu_psys_ppg_enqueue_bufsets")
void BPF_KRETPROBE(ipu_psys_ppg_enqueue_bufsets_exit, int ret)
{
	bpf_printk("graph_ret func=ipu_psys_ppg_enqueue_bufsets ret=%d", ret);
}

SEC("kprobe/ipu_psys_ppg_complete")
void BPF_KPROBE(ipu_psys_ppg_complete_entry,struct ipu_psys___local *psys, struct ipu_psys_ppg___local *kppg){
  
	enum ipu_psys_ppg_state old_ppg_state;
 	BPF_CORE_READ_INTO(&old_ppg_state,  kppg, state);
	const char *ppg_state_str =  get_ppg_index_by_state(old_ppg_state);

	struct ipu_psys_pg___local *kpg;
	kpg = (struct ipu_psys_pg___local *)BPF_CORE_READ(kppg, kpg);
	size_t pg_size;
	BPF_CORE_READ_INTO(&pg_size,  kpg, pg_size);
	
	struct ipu_fw_psys_process_group___local *pg;
	pg = (struct ipu_fw_psys_process_group___local *)BPF_CORE_READ(kpg, pg);

	u32 ppg_id;
	BPF_CORE_READ_INTO(&ppg_id,  pg, ID);
	u8 base_queue_id;
	BPF_CORE_READ_INTO(&base_queue_id,  pg, base_queue_id);

	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {
	
	  bpf_vprintk("graph_ent func=ipu_psys_ppg_complete args=ppg_id=%u;queue_id=%u;ppg_sz=%u;ppg_st=%s",
		      ppg_id, base_queue_id, pg_size,
		      ppg_state_str);
	} else { 
	  bpf_printk("graph_ent func=ipu_psys_ppg_complete args=ppg_id=%u;queue_id=%u;ppg_st=%s",
		      ppg_id, base_queue_id,
		      ppg_state_str);
	}

	if ( g_state.prev_ppg_state[base_queue_id] == old_ppg_state && 
	     g_state.prev_ppg_id[base_queue_id] == ppg_id)  return;

	enum ipu_psys_ppg_state prev_state;
	prev_state = g_state.prev_ppg_state[base_queue_id];
	const char *prev_ppg_state_str = get_ppg_index_by_state(prev_state);

	u32 prev_ppg_id;
	prev_ppg_id = g_state.prev_ppg_id[base_queue_id];
	
	if (!g_state.ppg_first) {
	    bpf_printk("E|ipu_fw_psys/handle_events;/dev ipu6|%s|ppg_id=%u;queue_id=%u|ipu6-trace",
		       prev_ppg_state_str,
		       prev_ppg_id,
		       base_queue_id);
	}
	bpf_printk("B|ipu_fw_psys/handle_events;/dev ipu6|%s|ppg_id=%u;queue_id=%u|ipu6-trace",
		   ppg_state_str,
		   ppg_id,
		   base_queue_id);

	if (g_state.ppg_first) g_state.ppg_first = false;
	g_state.prev_ppg_state[base_queue_id] = old_ppg_state;
	g_state.prev_ppg_id[base_queue_id] = ppg_id;
	/*	
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;
	if (LINUX_KERNEL_VERSION > KERNEL_VERSION(5, 19, 0)) {

	  bpf_vprintk("S|%u|ipu_psys_scheduler|0|ppg_st=%s=ipu_psys_ppg_complete|ipu6-trace",
		      pid,
		      ipu_psys_cmd_state_types[ppg_type_state]);
	} else { 
	  bpf_printk("S|%u|ipu_psys_scheduler|0|ppg_st=%s;kcmd_st=%s;call=ipu_psys_ppg_complete|ipu6-trace",
		     pid,
		     ipu_psys_cmd_state_types[ppg_type_state]);
	}
	*/
	return;
}

SEC("kretprobe/ipu_psys_ppg_complete")
void BPF_KRETPROBE(ipu_psys_ppg_complete_exit, int ret)
{
	bpf_printk("graph_ret func=ipu_psys_ppg_complete ret=%d", ret);
/*
	// get PID of exiting thread/process
	u32 pid = bpf_get_current_pid_tgid()  >> 32;

	bpf_printk("F|%u|ipu_psys_scheduler|0|ret=%d|ipu6-trace", pid, ret);
*/
}
