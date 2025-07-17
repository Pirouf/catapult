// SPDX-License-Identifier: GPL-2.0+
// static const char *__doc__ = " Trivial bpf_trace_printk test";

#include <config.h>
#include <bpf/libbpf.h>
#include <linux/perf_event.h>
#include <stdlib.h>
#include <errno.h>

#include <stdio.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include "ethtool_utils.h"
#include "perf-sys.h"

#define pr_err(fmt, ...) \
	fprintf(stderr, "%s:%d - " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

#define DEBUGFS "/sys/kernel/debug/tracing/"
#define MAX_LINK 50
#define SAMPLE_PERIOD  0x7fffffffffffffffULL

struct perf_event_attr attr_cycles = {
	.freq = 0,
	.sample_period = SAMPLE_PERIOD,
	.inherit = 0,
	.type = PERF_TYPE_HARDWARE,
	.read_format = 0,
	.sample_type = 0,
	.config = PERF_COUNT_HW_CPU_CYCLES,
};
struct perf_event_attr attr_clock = {
	.freq = 0,
	.sample_period = SAMPLE_PERIOD,
	.inherit = 0,
	.type = PERF_TYPE_SOFTWARE,
	.read_format = 0,
	.sample_type = 0,
	.config = PERF_COUNT_SW_CPU_CLOCK,
};
struct perf_event_attr attr_raw = {
	.freq = 0,
	.sample_period = SAMPLE_PERIOD,
	.inherit = 0,
	.type = PERF_TYPE_RAW,
	.read_format = 0,
	.sample_type = 0,
	/* Intel Instruction Retired */
	.config = 0xc0,
};
struct perf_event_attr attr_l1d_load = {
	.freq = 0,
	.sample_period = SAMPLE_PERIOD,
	.inherit = 0,
	.type = PERF_TYPE_HW_CACHE,
	.read_format = 0,
	.sample_type = 0,
	.config =
		PERF_COUNT_HW_CACHE_L1D |
		(PERF_COUNT_HW_CACHE_OP_READ << 8) |
		(PERF_COUNT_HW_CACHE_RESULT_ACCESS << 16),
};
struct perf_event_attr attr_llc_miss = {
	.freq = 0,
	.sample_period = SAMPLE_PERIOD,
	.inherit = 0,
	.type = PERF_TYPE_HW_CACHE,
	.read_format = 0,
	.sample_type = 0,
	.config =
		PERF_COUNT_HW_CACHE_LL |
		(PERF_COUNT_HW_CACHE_OP_READ << 8) |
		(PERF_COUNT_HW_CACHE_RESULT_MISS << 16),
};
struct perf_event_attr attr_msr_tsc = {
	.freq = 0,
	.sample_period = 0,
	.inherit = 0,
	/* From /sys/bus/event_source/devices/msr/ */
	.type = 7,
	.read_format = 0,
	.sample_type = 0,
	.config = 0,
};
struct perf_event_attr attr_msr_smi = {
	.freq = 0,
	.sample_period = 0,
	.inherit = 0,
	/* From /sys/bus/event_source/devices/msr/ */
	.type = 7,
	.read_format = 0,
	.sample_type = 0,
	.config = 0,
};
#define SAMPLE_PERIOD  0x7fffffffffffffffULL

/* counters, values, values2 */
static int map_fd[3];

void read_trace_pipe(void)
{
	int trace_fd;

	trace_fd = open(DEBUGFS "trace_pipe", O_RDONLY, 0);
	if (trace_fd < 0)
		return;

	while (1) {
		static char buf[4096];
		ssize_t sz;

		sz = read(trace_fd, buf, sizeof(buf) - 1);
		if (sz > 0) {
			buf[sz] = 0;
			puts(buf);
		}
	}
}

int print_all_levels(enum libbpf_print_level level,
		     const char *format, va_list args)
{
	return vfprintf(stderr, format, args);
}

static const struct option long_options[] = {
	{ "debug",	no_argument,	NULL,	'd' },
	{ "ifname",	required_argument,	NULL,	'i' },
	{ "ipu6",	required_argument,	NULL,	'v' },
	{ 0, 0, NULL, 0 }
};

int main(int argc, char **argv)
{
	struct bpf_object *obj = NULL;
	struct bpf_link *links[MAX_LINK];
	struct bpf_program *prog;
	int opt, longindex = 0;
	char filename[512];
	char buf[100];
	int err;
	int rb_fd, j = 0;
	bool ipu6_fw_logs=false;
	const char *title;
	bool perf_evt_enabled=false;
	int pmu_fd=-1, error = 0;
	char bpf_co_re_drv[64] = "\0";

	/* Parse commands line args */
	while ((opt = getopt_long(argc, argv, "ik:dp",
				  long_options, &longindex)) != -1) {
		switch (opt) {
		case 'i':
		        if(!ethtool_get_drvinfo(optarg, bpf_co_re_drv, sizeof(bpf_co_re_drv))) {
			  fprintf(stderr, "%s NO driver Found!\n", optarg);
			  return EXIT_FAILURE;
			}
			break;
		case 'k':
		        strncpy(bpf_co_re_drv, optarg, sizeof(bpf_co_re_drv));
		        //ipu6_fw_logs = true;
			break;
		case 'd':
			libbpf_set_print(print_all_levels);
			// verifier_logs = true;
			break;
		case 'p':
			// verifier_logs = true;
			perf_evt_enabled=true;
			break;
		default:
			pr_err("Unrecognized option '%s'\n", argv[optind - 1]);
			return EXIT_FAILURE;
		}
	}

	if (!bpf_co_re_drv[0]) {
	  fprintf(stderr, "Usage: %s [-i <ifnet name>] | [-k <ipu6-isys | ipu6-isys-psys | ipu6-d4xx | igc-poll>] [-d]\n",
		  argv[0]);
	  return EXIT_FAILURE;
	}
	snprintf(filename, sizeof(filename), "/usr/share/catapult/%s_ktrace_kern.o", bpf_co_re_drv);
	argc -= optind;
	argv += optind;

	for (j=0; j < MAX_LINK ; j++) {
	  links[j] = NULL;
	}

	obj = bpf_object__open_file(filename, NULL);
	err = libbpf_get_error(obj);
	if (err) {
		libbpf_strerror(err, buf, sizeof(buf));
		pr_err("Error(%d) opening file: %s\n", err, buf);
		goto out;
	}

	err = bpf_object__load(obj);
	if (err) {
		libbpf_strerror(err, buf, sizeof(buf));
		pr_err("Error(%d) loading: %s\n", err, buf);
		goto out;
	}
#ifdef HAVE_LIBBPF0
	prog = bpf_program__next(NULL, obj);
#else
	prog = bpf_object__next_program(obj, NULL);
#endif
	if (!prog) {
		pr_err("No program!\n");
		err = -ENOENT;
		goto out;
	}
	printf("# tracer: nop\n"			\
	       "#\n"					\
	       "# Loaded BPF CO-RE program : %s\n"	\
	       "# ", filename);

	/* Open perf event and attach to the perf_event_array	*/

	printf("#\n"				\
	       "# sys_perf_event_open : \n"	\
	       "# ");
	pmu_fd = sys_perf_event_open(&attr_llc_miss, -1, 0, -1, 0);
	if (pmu_fd < 0) {
		printf("\n# ignored on CPU %d\n", 0);
	} else {
		printf("\n# added on CPU %d\n", 0);
		ioctl(pmu_fd, PERF_EVENT_IOC_ENABLE, 0);
	}
	bpf_object__for_each_program(prog, obj) {
#ifdef HAVE_LIBBPF0
	  title = bpf_program__title(prog, false);
#else
	  title = bpf_program__section_name(prog);
#endif
	  links[j] = bpf_program__attach(prog);
	  if (libbpf_get_error(links[j])) {
	    printf("\n# %s uavailable",title);
	    links[j] = NULL;
	  } else {
	    printf("\n# %s",title);
	  }
	  j++;
	}
	
	printf("\n" \
	       "# Attached kprobe and reading trace_pipe\n" \
	       "# - Press Ctrl-C to unload program again\n");
	read_trace_pipe();

out:
	for (j=0; j < MAX_LINK  ; j++) {
	  if (links[j] != NULL) { 
	    bpf_link__destroy(links[j]);
	    if (libbpf_get_error(links[j])) {
	      fprintf(stderr, "ERROR: bpf_link__destroy failed\n");
	    }
	    links[j] = NULL;
	  }
	}
	if (pmu_fd >= 0) {
	  ioctl(pmu_fd, PERF_EVENT_IOC_DISABLE, 0);
	  close(pmu_fd);
	}
	bpf_object__close(obj);
	if (err)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
