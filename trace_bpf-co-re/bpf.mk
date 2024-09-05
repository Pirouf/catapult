# $Id$

HAVE_LIBBPF_PERF_BUFFER__CONSUME:=y

BPF_CFLAGS := -Wno-visibility

# The BPF tracing header (/usr/include/bpf/bpf_tracing.h) need to know
# CPU architecture due to PT_REGS_PARM resolution of ASM call convention
#
ARCH := $(shell uname -m | sed 's/x86_64/x86/' | sed 's/aarch64/arm64/' | sed 's/ppc64le/powerpc/' | sed 's/mips.*/mips/')

BPF_CFLAGS += -D__TARGET_ARCH_$(ARCH)

# BPF-prog kern and userspace shares struct via header file:
KERN_USER_H ?= $(wildcard common_kern_user.h)

BPF_CFLAGS += -I$(top_builddir)/headers -I$(top_builddir)/include -I/usr/include/x86_64-linux-gnu

BPF_C = ${BPF_TARGETS:=.c}
BPF_OBJ = ${BPF_C:.c=.o}
BPF_SKEL = ${BPF_SKEL_OBJ:.o=.skel.h}

BPF_OBJ_INSTALL ?= $(BPF_OBJ)

all: $(BPF_OBJ) $(BPF_SKEL)

.PHONY: clean
clean:
	$(Q)rm -f $(BPF_OBJ) *.ll

$(BPF_OBJ): %.o: %.c $(KERN_USER_H) $(EXTRA_DEPS) $(BPF_INCLUDES) $(BPF_EXTRA_INCLUDES)
	$(QUIET_CLANG)$(CLANG) -S \
	    -target bpf \
	    -D __BPF_TRACING__ \
	    $(BPF_CFLAGS) \
	    -Wall \
	    -Wno-unused-value \
	    -Wno-pointer-sign \
	    -Wno-compare-distinct-pointer-types \
	    -O2 -emit-llvm -c -g -o ${@:.o=.ll} $<
	$(QUIET_LLC)$(LLC) -march=bpf -filetype=obj -o $@ ${@:.o=.ll}

$(BPF_SKEL): %.skel.h: %.o
	$(QUIET_GEN)$(BPFTOOL) gen skeleton ${@:.skel.h=.o} > $@
