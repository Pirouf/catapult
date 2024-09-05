// SPDX-License-Identifier: GPL-2.0+
// static const char *__doc__ = " Trivial bpf_trace_printk test";

#include <config.h>
#include <bpf/libbpf.h>
#include <stdlib.h>
#include <errno.h>

#include <stdio.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include "ethtool_utils.h"

#define pr_err(fmt, ...) \
	fprintf(stderr, "%s:%d - " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

#define DEBUGFS "/sys/kernel/debug/tracing/"

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
	struct bpf_link *links[10];
	struct bpf_program *prog;
	int opt, longindex = 0;
	char filename[512];
	char buf[100];
	int err;
	int rb_fd, j = 0;
	bool ipu6_fw_logs=false;

	char bpf_co_re_drv[64] = "\0";

	/* Parse commands line args */
	while ((opt = getopt_long(argc, argv, "ik:d",
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
		default:
			pr_err("Unrecognized option '%s'\n", argv[optind - 1]);
			return EXIT_FAILURE;
		}
	}

	if (!bpf_co_re_drv[0]) {
	  fprintf(stderr, "Usage: %s [-i <ifnet name>] | [-k <ipu6-isys | ipu6-d4xx | igc>] [-d]\n",
		  argv[0]);
	  return EXIT_FAILURE;
	}
	snprintf(filename, sizeof(filename), "/usr/share/catapult/%s_ktrace_kern.o", bpf_co_re_drv);
	argc -= optind;
	argv += optind;

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
	       "# Loaded BPF CO-RE file %s\n"		\
	       , filename);

	bpf_object__for_each_program(prog, obj) {
	  links[j] = bpf_program__attach(prog);
	  if (libbpf_get_error(links[j])) {
	    fprintf(stderr, "ERROR: bpf_program__attach failed\n");
	    links[j] = NULL;
	    goto out;
	  }
	  j++;
	}

	printf("# Attached kprobe and reading trace_pipe\n");
	printf("# - Press Ctrl-C to unload program again\n");
	read_trace_pipe();

out:
	bpf_link__destroy(link);
	bpf_object__close(obj);
	if (err)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
