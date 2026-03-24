# SPDX-License-Identifier: (GPL-2.0 OR BSD-2-Clause)

bin_PROGRAMS = bpf_ktrace_co_re

BPF_DIR_MNT ?=/sys/fs/bpf
BPF_OBJECT_DIR ?=./
DEFINES = -DBPF_DIR_MNT=\"$(BPF_DIR_MNT)\" -DBPF_OBJECT_PATH=\"$(BPF_OBJECT_DIR)\"

bpf_ktrace_co_re_SOURCES = bpf_ktrace_co_re.c ethtool_utils.c
bpf_ktrace_co_re_CFLAGS =  -I../ -I./include -I./headers $(libbpf_CFLAGS) $(libelf_CFLAGS) $(DEFINES)
bpf_ktrace_co_re_LDADD = $(libbpf_LIBS) $(libelf_LIBS) -lpthread -lrt -lnuma

# It is expected that you define the variables BPF_TARGETS
# as a space-separated list
BPF_TARGETS = ipu7-ipu6-isys_ktrace_kern ipu6-d4xx_ktrace_kern ipu6-isys_ktrace_kern ipu6-isys-psys_ktrace_kern igc-poll_ktrace_kern

@BPF_TOOLCHAIN_MAKE_RULES@

@BPF_PROG_MAKE_RULES@
