#+Title: Test using BPF CO-RE to define kprobe/kretprobe and bpf_trace_printk linux events

This is an `bpf_trace_printk` example that show how CO-RE came be utilised...
to learn CO-RE and BTF better.

* Compile BPF CO-RE trace kprobes/kretprobes for different CPU architectures...
* Add `bpf_trace_printk` user-events

Install Linux distro specific kernel-dbgsym (e.g. https://wiki.ubuntu.com/Debug%20Symbol%20Packages, https://access.redhat.com/solutions/9907, ...) and download the corresponding kernel-sources to define particular `kprobe+<offset>` o$

## BPF Co-RE tracing of Intel Ethernet TSN controller drivers functional entry/exit 

Check if a particular  function call in Intel Ethernet TSN controller linux module can be attached via kprobe and kretprobe 

```
# gdb -batch -ex 'file /usr/lib/debug/modules/6.5.0-1023-oem/kernel/drivers/net/ethernet/intel/igc/igc.ko' -ex 'disassemble /s igc_poll' -ex 'set disassembly-flavor intel' | less'
...
3035            tx_desc = IGC_TX_DESC(tx_ring, i);
   0x0000000000006396 <+166>:   add    %al,(%rax)
   0x0000000000006398 <+168>:   add    %al,(%rax)

3022            unsigned int total_bytes = 0, total_packets = 0;
   0x000000000000639a <+170>:   add    %al,(%rax)
   0x000000000000639c <+172>:   add    %al,(%rax)
   0x000000000000639e <+174>:   add    %al,(%rax)
   0x00000000000063a0 <+176>:   add    %al,(%rax)
   0x00000000000063a2 <+178>:   add    %al,(%rax)
   0x00000000000063a4 <+180>:   add    %al,(%rax)
   0x00000000000063a6 <+182>:   add    %al,(%rax)
   0x00000000000063a8 <+184>:   add    %al,(%rax)
   0x00000000000063aa <+186>:   add    %al,(%rax)

3032                    return true;
3033
3034            tx_buffer = &tx_ring->tx_buffer_info[i];
   0x00000000000063ac <+188>:   add    %al,(%rax)
   0x00000000000063ae <+190>:   add    %al,(%rax)

3036            i -= tx_ring->count;
   0x00000000000063b0 <+192>:   add    %al,(%rax)

3022            unsigned int total_bytes = 0, total_packets = 0;
   0x00000000000063b2 <+194>:   add    %al,(%rax)
   0x00000000000063b4 <+196>:   add    %al,(%rax)

3037
3038            do {
3039                    union igc_adv_tx_desc *eop_desc = tx_buffer->next_to_watch;
   0x00000000000063b6 <+198>:   add    %al,(%rax)

3040
3041                    /* if next_to_watch is not set then there is no work pending */
3042                    if (!eop_desc)
   0x00000000000063b8 <+200>:   add    %al,(%rax)
   0x00000000000063ba <+202>:   add    %al,(%rax)
   0x00000000000063bc <+204>:   add    %al,(%rax)
   0x00000000000063be <+206>:   add    %al,(%rax)
   0x00000000000063c0 <+208>:   add    %al,(%rax)

3043                            break;
3044
3045                    /* prevent any other reads prior to eop_desc */
...
```

Run the following test BPF CO-RE `bpf_trace_prink()` test on Intel Ethernet controllers <ifname> to attach I22x `[igc]` , I21x `[igb]` or TGL/EHL/ADL/RPL mGbE `[st_gmac]` user-defined BPF_KPROBE/BPF_RETKPROBE tracepoints: 
```
# ./bpf_trace_printk_test -i enp2s0
# tracer: nop
#
# Loaded BPF CO-RE file igc_ktrace_kern.o
# Attached kprobe and reading trace_pipe
# - Press Ctrl-C to unload program again
 irq/135-enp2s0--19778   [000] d..s313 697678.023239: bpf_trace_printk: B|19778|clean_tx_irq|ifindex=4;status=0|bpftrace
 irq/138-enp2s0--19781   [001] d..s313 697678.023242: bpf_trace_printk: B|19781|clean_tx_irq|ifindex=4;status=0|bpftrace
 irq/135-enp2s0--19778   [000] d..s313 697678.023246: bpf_trace_printk: E|ret=0|bpftrace
 irq/138-enp2s0--19781   [001] d..s313 697678.023247: bpf_trace_printk: E|ret=0|bpftrace
 irq/137-enp2s0--19780   [001] d..s313 697678.023250: bpf_trace_printk: B|19780|clean_tx_irq|ifindex=4;status=0|bpftrace
 irq/137-enp2s0--19780   [001] d..s313 697678.023251: bpf_trace_printk: E|ret=0|bpftrace
 irq/136-enp2s0--19779   [001] d..s313 697678.023254: bpf_trace_printk: B|19779|clean_tx_irq|ifindex=4;status=0|bpftrace
 irq/136-enp2s0--19779   [001] d..s313 697678.023257: bpf_trace_printk: E|ret=0|bpftrace

 irq/135-enp2s0--19778   [000] d..s313 697680.039227: bpf_trace_printk: B|19778|clean_tx_irq|ifindex=4;status=0|bpftrace
 irq/138-enp2s0--19781   [001] d..s313 697680.039230: bpf_trace_printk: B|19781|clean_tx_irq|ifindex=4;status=0|bpftrace
 irq/138-enp2s0--19781   [001] d..s313 697680.039233: bpf_trace_printk: E|ret=0|bpftrace
 irq/135-enp2s0--19778   [000] d..s313 697680.039233: bpf_trace_printk: E|ret=0|bpftrace
 irq/137-enp2s0--19780   [001] d..s313 697680.039236: bpf_trace_printk: B|19780|clean_tx_irq|ifindex=4;status=0|bpftrace
 irq/137-enp2s0--19780   [001] d..s313 697680.039237: bpf_trace_printk: E|ret=0|bpftrace
 irq/136-enp2s0--19779   [001] d..s313 697680.039240: bpf_trace_printk: B|19779|clean_tx_irq|ifindex=4;status=0|bpftrace
 irq/136-enp2s0--19779   [001] d..s313 697680.039240: bpf_trace_printk: E|ret=0|bpftrace
...
```

## BPF Co-RE tracing of Intel IPU6 drivers function-call entry/exit 

Check if a particular  function call in Intel Ethernet TSN controller linux module can be attached via kprobe and kretprobe 

```
gdb -batch -ex 'file /usr/lib/modules/6.5.0-1023-oem/updates/dkms/intel-ipu6-isys.ko'  -ex 'set disassembly-flavor intel' -ex 'set substitute-path /var/lib/dkms/ipu6-drivers/20240118+iotgipu6-0eci6/build /root/ipu6-drivers-20240118+iotgipu6' -ex 'disassemble /s ipu_fw_isys_simple_cmd'
Dump of assembler code for function ipu_fw_isys_simple_cmd:
/var/lib/dkms/ipu6-drivers/20240118+iotgipu6-0eci6/build/drivers/media/pci/intel/ipu6/../ipu-fw-isys.c:
203	{
   0x00000000000085d0 <+0>:	call   0x85d5 <ipu_fw_isys_simple_cmd+5>

170		struct ipu_fw_com_context *ctx = isys->fwcom;
   0x00000000000085d5 <+5>:	push   rbp
   0x00000000000085d6 <+6>:	mov    rbp,rsp
   0x00000000000085d9 <+9>:	push   r14
   0x00000000000085db <+11>:	push   r13
   0x00000000000085dd <+13>:	push   r12
   0x00000000000085df <+15>:	push   rbx
   0x00000000000085e0 <+16>:	sub    rsp,0x8
   0x00000000000085e4 <+20>:	mov    r12,QWORD PTR [rdi+0x3b0]

171		struct ipu_fw_send_queue_token *token;
172	
173		if (send_type >= N_IPU_FW_ISYS_SEND_TYPE)
   0x00000000000085eb <+27>:	cmp    edx,0x6
   0x00000000000085ee <+30>:	ja     0x867f <ipu_fw_isys_simple_cmd+175>
   0x00000000000085f4 <+36>:	mov    ebx,esi
   0x00000000000085f6 <+38>:	mov    r13d,edx

./arch/x86/include/asm/jump_label.h:
27		asm_volatile_goto("1:"
   0x00000000000085f9 <+41>:	xchg   ax,ax

/var/lib/dkms/ipu6-drivers/20240118+iotgipu6-0eci6/build/drivers/media/pci/intel/ipu6/../ipu-fw-isys.c:
186		token = ipu_send_get_token(ctx,
   0x00000000000085fb <+43>:	add    ebx,0x2
   0x00000000000085fe <+46>:	mov    rdi,r12
   0x0000000000008601 <+49>:	mov    esi,ebx
   0x0000000000008603 <+51>:	call   0x8608 <ipu_fw_isys_simple_cmd+56>

187					   stream_handle + IPU_BASE_MSG_SEND_QUEUES);
188		if (!token)
   0x0000000000008608 <+56>:	test   rax,rax
   0x000000000000860b <+59>:	je     0x8686 <ipu_fw_isys_simple_cmd+182>

190	
191		token->payload = dma_mapped_buf;
   0x000000000000860d <+61>:	mov    DWORD PTR [rax+0x8],0x0

192		token->buf_handle = (unsigned long)cpu_mapped_buf;
   0x0000000000008614 <+68>:	mov    esi,ebx
   0x0000000000008616 <+70>:	mov    rdi,r12
   0x0000000000008619 <+73>:	mov    QWORD PTR [rax],0x0

193		token->send_type = send_type;
   0x0000000000008620 <+80>:	mov    WORD PTR [rax+0xc],r13w

194	
195		ipu_send_put_token(ctx, stream_handle + IPU_BASE_MSG_SEND_QUEUES);
   0x0000000000008625 <+85>:	call   0x862a <ipu_fw_isys_simple_cmd+90>

196	
197		return 0;
   0x000000000000862a <+90>:	xor    eax,eax

205					       send_type);
206	}
   0x000000000000862c <+92>:	add    rsp,0x8
   0x0000000000008630 <+96>:	pop    rbx
   0x0000000000008631 <+97>:	pop    r12
   0x0000000000008633 <+99>:	pop    r13
   0x0000000000008635 <+101>:	pop    r14
   0x0000000000008637 <+103>:	pop    rbp
   0x0000000000008638 <+104>:	xor    edx,edx
   0x000000000000863a <+106>:	xor    ecx,ecx
   0x000000000000863c <+108>:	xor    esi,esi
   0x000000000000863e <+110>:	xor    edi,edi
   0x0000000000008640 <+112>:	xor    r8d,r8d
   0x0000000000008643 <+115>:	jmp    0x8648 <ipu_fw_isys_simple_cmd+120>
   0x0000000000008648 <+120>:	mov    r14,rdi

176		dev_dbg(&isys->adev->dev, "send_token: %s, stream_handle: %u\n",
   0x000000000000864b <+123>:	mov    ecx,edx
   0x000000000000864d <+125>:	cmp    edx,0x7
   0x0000000000008650 <+128>:	ja     0x868d <ipu_fw_isys_simple_cmd+189>
   0x0000000000008652 <+130>:	shl    rcx,0x5
   0x0000000000008656 <+134>:	mov    rsi,QWORD PTR [r14+0x310]
   0x000000000000865d <+141>:	mov    r8d,ebx
   0x0000000000008660 <+144>:	mov    rdx,0x0
   0x0000000000008667 <+151>:	add    rcx,0x0
   0x000000000000866e <+158>:	mov    rdi,0x0
   0x0000000000008675 <+165>:	call   0x867a <ipu_fw_isys_simple_cmd+170>
   0x000000000000867a <+170>:	jmp    0x85fb <ipu_fw_isys_simple_cmd+43>

174			return -EINVAL;
   0x000000000000867f <+175>:	mov    eax,0xffffffea
   0x0000000000008684 <+180>:	jmp    0x862c <ipu_fw_isys_simple_cmd+92>

189			return -EBUSY;
   0x0000000000008686 <+182>:	mov    eax,0xfffffff0

204		return ipu_fw_isys_complex_cmd(isys, stream_handle, NULL, 0, 0,
   0x000000000000868b <+187>:	jmp    0x862c <ipu_fw_isys_simple_cmd+92>

176		dev_dbg(&isys->adev->dev, "send_token: %s, stream_handle: %u\n",
   0x000000000000868d <+189>:	mov    rsi,rcx
   0x0000000000008690 <+192>:	mov    rdi,0x0
   0x0000000000008697 <+199>:	mov    QWORD PTR [rbp-0x28],rcx
   0x000000000000869b <+203>:	call   0x86a0 <ipu_fw_isys_simple_cmd+208>
   0x00000000000086a0 <+208>:	mov    rcx,QWORD PTR [rbp-0x28]
   0x00000000000086a4 <+212>:	jmp    0x8652 <ipu_fw_isys_simple_cmd+130>
End of assembler dump.
```

Run the following test BPF CO-RE `bpf_trace_prink()` test on Intel IPU6 nultimedia controllers <ipu6-isys | ipu6-psys> to attach  `[intel-ipu6-isys]` or psys `[intel-ipu6-psys]` user-defined BPF_KPROBE/BPF_RETKPROBE tracepoints: 
```
# ./bpf_trace_printk_test -v ipu6-isys
# tracer: nop
#
# Loaded BPF CO-RE file ipu6-isys_ktrace_kern.o
# Attached kprobe and reading trace_pipe
# - Press Ctrl-C to unload program again
```