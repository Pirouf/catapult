/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef BPF_DEFS_H
#define BPF_DEFS_H

#include <stdbool.h>

//#include <pthread.h> removed to avoid <stub32.h> i686 deps 
//#include <linux/spinlock_types_raw.h>

#include <asm-generic/posix_types.h>
typedef __kernel_size_t		size_t;

typedef struct {
	volatile unsigned int slock;
} arch_spinlock_t;

typedef struct raw_spinlock___local {
	arch_spinlock_t raw_lock;
} raw_spinlock_t;

/* Non PREEMPT_RT kernels map spinlock to raw_spinlock */
typedef struct spinlock___local {
	union {
		struct raw_spinlock___local rlock;
	};
} spinlock_t;

//#include <linux/v4l2-controls.h>  removed to avoid <stub32.h> i686 deps

/* Control classes */
#define V4L2_CTRL_CLASS_USER		0x00980000	/* Old-style 'user' controls */
#define V4L2_CTRL_CLASS_CODEC		0x00990000	/* Stateful codec controls */
#define V4L2_CTRL_CLASS_CAMERA		0x009a0000	/* Camera class controls */
#define V4L2_CTRL_CLASS_FM_TX		0x009b0000	/* FM Modulator controls */
#define V4L2_CTRL_CLASS_FLASH		0x009c0000	/* Camera flash controls */
#define V4L2_CTRL_CLASS_JPEG		0x009d0000	/* JPEG-compression controls */
#define V4L2_CTRL_CLASS_IMAGE_SOURCE	0x009e0000	/* Image source controls */
#define V4L2_CTRL_CLASS_IMAGE_PROC	0x009f0000	/* Image processing controls */
#define V4L2_CTRL_CLASS_DV		0x00a00000	/* Digital Video controls */
#define V4L2_CTRL_CLASS_FM_RX		0x00a10000	/* FM Receiver controls */
#define V4L2_CTRL_CLASS_RF_TUNER	0x00a20000	/* RF tuner controls */
#define V4L2_CTRL_CLASS_DETECT		0x00a30000	/* Detection controls */
#define V4L2_CTRL_CLASS_CODEC_STATELESS 0x00a40000	/* Stateless codecs controls */
#define V4L2_CTRL_CLASS_COLORIMETRY	0x00a50000	/* Colorimetry controls */

/* User-class control IDs */
#define V4L2_CID_BASE                   (V4L2_CTRL_CLASS_USER | 0x900)
#define V4L2_CID_USER_BASE              V4L2_CID_BASE
#define V4L2_CID_USER_CLASS             (V4L2_CTRL_CLASS_USER | 1)
#define V4L2_CID_BRIGHTNESS             (V4L2_CID_BASE+0)
#define V4L2_CID_CONTRAST               (V4L2_CID_BASE+1)
#define V4L2_CID_SATURATION             (V4L2_CID_BASE+2)
#define V4L2_CID_HUE                    (V4L2_CID_BASE+3)
#define V4L2_CID_AUDIO_VOLUME           (V4L2_CID_BASE+5)
#define V4L2_CID_AUDIO_BALANCE          (V4L2_CID_BASE+6)
#define V4L2_CID_AUDIO_BASS             (V4L2_CID_BASE+7)
#define V4L2_CID_AUDIO_TREBLE           (V4L2_CID_BASE+8)
#define V4L2_CID_AUDIO_MUTE             (V4L2_CID_BASE+9)
#define V4L2_CID_AUDIO_LOUDNESS         (V4L2_CID_BASE+10)
#define V4L2_CID_BLACK_LEVEL            (V4L2_CID_BASE+11) /* Deprecated */
#define V4L2_CID_AUTO_WHITE_BALANCE     (V4L2_CID_BASE+12)
#define V4L2_CID_DO_WHITE_BALANCE       (V4L2_CID_BASE+13)
#define V4L2_CID_RED_BALANCE            (V4L2_CID_BASE+14)
#define V4L2_CID_BLUE_BALANCE           (V4L2_CID_BASE+15)
#define V4L2_CID_GAMMA                  (V4L2_CID_BASE+16)
#define V4L2_CID_WHITENESS              (V4L2_CID_GAMMA) /* Deprecated */
#define V4L2_CID_EXPOSURE               (V4L2_CID_BASE+17)
#define V4L2_CID_AUTOGAIN               (V4L2_CID_BASE+18)
#define V4L2_CID_GAIN                   (V4L2_CID_BASE+19)
#define V4L2_CID_HFLIP                  (V4L2_CID_BASE+20)
#define V4L2_CID_VFLIP                  (V4L2_CID_BASE+21)


#define V4L2_CID_IPU_BASE	(V4L2_CID_USER_BASE + 0x1080)

//#include <linux/v4l2-mediabus.h>  removed to avoid <stub32.h> i686 deps

/**
 * struct v4l2_mbus_framefmt - frame format on the media bus
 * @width:	image width
 * @height:	image height
 * @code:	data format code (from enum v4l2_mbus_pixelcode)
 * @field:	used interlacing type (from enum v4l2_field), zero for metadata
 *		mbus codes
 * @colorspace:	colorspace of the data (from enum v4l2_colorspace), zero on
 *		metadata mbus codes
 * @ycbcr_enc:	YCbCr encoding of the data (from enum v4l2_ycbcr_encoding), zero
 *		for metadata mbus codes
 * @hsv_enc:	HSV encoding of the data (from enum v4l2_hsv_encoding), zero for
 *		metadata mbus codes
 * @quantization: quantization of the data (from enum v4l2_quantization), zero
 *		for metadata mbus codes
 * @xfer_func:  transfer function of the data (from enum v4l2_xfer_func), zero
 *		for metadata mbus codes
 * @flags:	flags (V4L2_MBUS_FRAMEFMT_*)
 * @reserved:  reserved bytes that can be later used
 */
struct v4l2_mbus_framefmt___local {
	__u32			width;
	__u32			height;
	__u32			code;
	__u32			field;
	__u32			colorspace;
	union {
		/* enum v4l2_ycbcr_encoding */
		__u16			ycbcr_enc;
		/* enum v4l2_hsv_encoding */
		__u16			hsv_enc;
	};
	__u16			quantization;
	__u16			xfer_func;
	__u16			flags;
	__u16			reserved[10];
} __attribute__((preserve_access_index));

enum ipu_isys_v4l2_cid_type {
	IPU_STORE_CSI2_HEADER = 2,
	IPU_ISYS_COMPRESSION,
	IPU_QUERY_SUB_STREAM,
	IPU_SET_SUB_STREAM,
	IPU_ENUMERATE_LINK,
	N_IPU_ISYS_V4L2_CID_TYPE
};

static const char ipu_isys_v4l2_cid_types[N_IPU_ISYS_V4L2_CID_TYPE][32] = {
	"IPU_UNKNOWN_V4L2_CID",
	"IPU_UNKNOWN_V4L2_CID",
        "IPU_STORE_CSI2_HEADER",
	"IPU_ISYS_COMPRESSION",
	"IPU_QUERY_SUB_STREAM",
	"IPU_SET_SUB_STREAM",
	"IPU_ENUMERATE_LINK",
};

#define TASK_COMM_LEN 16
#define MAX_FILENAME_LEN 127

#define IPU_ISYS_MAX_STREAMS 16
#define IPU_ISYS_MAX_PARALLEL_SOF 2

#define VIDEO_MAX_PLANES               8

struct event {
  int pid;
  int ppid;
  unsigned exit_code;
  unsigned long long duration_ns;
  char comm[TASK_COMM_LEN];
};

typedef u64 dma_addr_t;

struct kobject___local {
	const char		*name;
	struct list_head	entry;
	struct kobject___local	*parent;
} __attribute__((preserve_access_index));

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

struct filename___local {
	const char *name;/* pointer to actual string */
} __attribute__((preserve_access_index));

typedef struct {
  long counter;
} atomic_long_t;

/*
 * Simple, straightforward mutexes with strict semantics:
 */
struct mutex___local {
  atomic_long_t           owner;
  raw_spinlock_t          wait_lock;
} __attribute__((preserve_access_index));

/**
 * enum ipu_fw_isys_resp_type
 */
enum ipu_fw_isys_resp_type {
	IPU_FW_ISYS_RESP_TYPE_STREAM_OPEN_DONE = 0,
	IPU_FW_ISYS_RESP_TYPE_STREAM_START_ACK,
	IPU_FW_ISYS_RESP_TYPE_STREAM_START_AND_CAPTURE_ACK,
	IPU_FW_ISYS_RESP_TYPE_STREAM_CAPTURE_ACK,
	IPU_FW_ISYS_RESP_TYPE_STREAM_STOP_ACK,
	IPU_FW_ISYS_RESP_TYPE_STREAM_FLUSH_ACK,
	IPU_FW_ISYS_RESP_TYPE_STREAM_CLOSE_ACK,
	IPU_FW_ISYS_RESP_TYPE_PIN_DATA_READY,
	IPU_FW_ISYS_RESP_TYPE_PIN_DATA_WATERMARK,
	IPU_FW_ISYS_RESP_TYPE_FRAME_SOF,
	IPU_FW_ISYS_RESP_TYPE_FRAME_EOF,
	IPU_FW_ISYS_RESP_TYPE_STREAM_START_AND_CAPTURE_DONE,
	IPU_FW_ISYS_RESP_TYPE_STREAM_CAPTURE_DONE,
	IPU_FW_ISYS_RESP_TYPE_PIN_DATA_SKIPPED,
	IPU_FW_ISYS_RESP_TYPE_STREAM_CAPTURE_SKIPPED,
	IPU_FW_ISYS_RESP_TYPE_FRAME_SOF_DISCARDED,
	IPU_FW_ISYS_RESP_TYPE_FRAME_EOF_DISCARDED,
	IPU_FW_ISYS_RESP_TYPE_STATS_DATA_READY,
	N_IPU_FW_ISYS_RESP_TYPE
};

static const char resp_msg_types[N_IPU_FW_ISYS_RESP_TYPE][32] = {
        "STREAM_OPEN_DONE",
	"STREAM_START_ACK",
	"STREAM_START_AND_CAPTURE_ACK",
	"STREAM_CAPTURE_ACK",
	"STREAM_STOP_ACK",
	"STREAM_FLUSH_ACK",
	"STREAM_CLOSE_ACK",
	"PIN_DATA_READY",
	"PIN_DATA_WATERMARK",
	"FRAME_SOF",
	"FRAME_EOF",
	"STREAM_START_AND_CAPTURE_DONE",
	"STREAM_CAPTURE_DONE",
	"PIN_DATA_SKIPPED",
	"STREAM_CAPTURE_SKIPPED",
	"FRAME_SOF_DISCARDED",
	"FRAME_EOF_DISCARDED",
	"STATS_DATA_READY",
};

/**
 * enum ipu_fw_isys_error. Describes the error type detected by the FW
 */
enum ipu_fw_isys_error {
	IPU_FW_ISYS_ERROR_NONE = 0,	/* No details */
	IPU_FW_ISYS_ERROR_FW_INTERNAL_CONSISTENCY,	/* enum */
	IPU_FW_ISYS_ERROR_HW_CONSISTENCY,	/* enum */
	IPU_FW_ISYS_ERROR_DRIVER_INVALID_COMMAND_SEQUENCE,	/* enum */
	IPU_FW_ISYS_ERROR_DRIVER_INVALID_DEVICE_CONFIGURATION,	/* enum */
	IPU_FW_ISYS_ERROR_DRIVER_INVALID_STREAM_CONFIGURATION,	/* enum */
	IPU_FW_ISYS_ERROR_DRIVER_INVALID_FRAME_CONFIGURATION,	/* enum */
	IPU_FW_ISYS_ERROR_INSUFFICIENT_RESOURCES,	/* enum */
	IPU_FW_ISYS_ERROR_HW_REPORTED_STR2MMIO,	/* HW code */
	IPU_FW_ISYS_ERROR_HW_REPORTED_SIG2CIO,	/* HW code */
	IPU_FW_ISYS_ERROR_SENSOR_FW_SYNC,	/* enum */
	IPU_FW_ISYS_ERROR_STREAM_IN_SUSPENSION,	/* FW code */
	IPU_FW_ISYS_ERROR_RESPONSE_QUEUE_FULL,	/* FW code */
	N_IPU_FW_ISYS_ERROR
};

static const char isys_error_types[N_IPU_FW_ISYS_ERROR][32] = {
        "NONE",     /* No details */
	"FW_INTERNAL_CONSISTENCY",      /* enum */
	"HW_CONSISTENCY",       /* enum */
	"DRIVER_INVALID_COMMAND_SEQ",      /* enum */
	"DRIVER_INVALID_DEVICE_CFG",  /* enum */
	"DRIVER_INVALID_STREAM_CFG",  /* enum */
	"DRIVER_INVALID_FRAME_CFG",   /* enum */
	"INSUFFICIENT_RESOURCES",       /* enum */
	"HW_REPORTED_STR2MMIO", /* HW code */
	"HW_REPORTED_SIG2CIO",  /* HW code */
	"SENSOR_FW_SYNC",       /* enum */
	"STREAM_IN_SUSPENSION", /* FW code */
	"RESPONSE_QUEUE_FULL",  /* FW code */
};

enum ipu_fw_isys_sensor_type {
	/* non-snoopable to PSYS */
	IPU_FW_ISYS_VC1_SENSOR_DATA	= 0,
	/* non-snoopable for PDAF */
	IPU_FW_ISYS_VC1_SENSOR_PDAF,
	/* snoopable to CPU */
	IPU_FW_ISYS_VC0_SENSOR_METADATA,
	/* snoopable to CPU */
	IPU_FW_ISYS_VC0_SENSOR_DATA,
	N_IPU_FW_ISYS_SENSOR_TYPE
};

/**
 * enum ipu_fw_isys_send_type
 */
enum ipu_fw_isys_send_type {
	IPU_FW_ISYS_SEND_TYPE_STREAM_OPEN = 0,
	IPU_FW_ISYS_SEND_TYPE_STREAM_START,
	IPU_FW_ISYS_SEND_TYPE_STREAM_START_AND_CAPTURE,
	IPU_FW_ISYS_SEND_TYPE_STREAM_CAPTURE,
	IPU_FW_ISYS_SEND_TYPE_STREAM_STOP,
	IPU_FW_ISYS_SEND_TYPE_STREAM_FLUSH,
	IPU_FW_ISYS_SEND_TYPE_STREAM_CLOSE,
	N_IPU_FW_ISYS_SEND_TYPE
};

static const char send_msg_types[N_IPU_FW_ISYS_SEND_TYPE][32] = {
	"STREAM_OPEN",
	"STREAM_START",
	"STREAM_START_AND_CAPTURE",
	"STREAM_CAPTURE",
	"STREAM_STOP",
	"STREAM_FLUSH",
	"STREAM_CLOSE"
};

/**
 * enum ipu_fw_isys_state
 */
enum ipu_fw_isys_state {
	IPU_FW_ISYS_UNINIT = 0,
	IPU_FW_ISYS_IDLE,
	IPU_FW_ISYS_IN_TRANFER,
	N_IPU_FW_ISYS_STATES
};

static const char ipu_isys_state_msg[N_IPU_FW_ISYS_STATES][32] = {
        "STREAM_UNINIT",
	"STREAM_IDLE",
	"STREAM_IN_TRANFER",
};

/**
 * enum dphy_fsm_state
 */
enum phy_fsm_state {
        PHY_FSM_STATE_POWERON = 0,
        PHY_FSM_STATE_BGPON = 1,
        PHY_FSM_STATE_CAL_TYPE = 2,
        PHY_FSM_STATE_BURNIN_CAL = 3,
        PHY_FSM_STATE_TERMCAL = 4,
        PHY_FSM_STATE_OFFSETCAL = 5,
        PHY_FSM_STATE_OFFSET_LANE = 6,
        PHY_FSM_STATE_IDLE = 7,
        PHY_FSM_STATE_ULP = 8,
        PHY_FSM_STATE_DDLTUNNING = 9,
        PHY_FSM_STATE_SKEW_BACKWARD = 10,
        PHY_FSM_STATE_INVALID,
        N_PHY_FSM_STATE
};
#define IPU_DWC_DPHY_MAX_NUM             (6)
#define IPU_DWC_DPHY_HSFREQRANGE         (0x08)
#define IPU_DWC_DPHY_STATE               (0x1e)
static const char dphy_fsm_state_types[N_PHY_FSM_STATE][32] = {
	"DPHY_STATE_POWERON",
	"DPHY_STATE_BGPON",
        "DPHY_STATE_CAL_TYPE",
	"DPHY_STATE_BURNIN_CAL",
	"DPHY_STATE_TERMCAL",
	"DPHY_STATE_OFFSETCAL",
	"DPHY_STATE_OFFSET_LANE",
	"DPHY_STATE_IDLE",
	"DPHY_STATE_ULP",
	"DPHY_STATE_DDLTUNNING",
	"DPHY_STATE_SKEW_BACKWARD",
	"DPHY_STATE_UNKOWN",
};

struct dwc_dphy_freq_range {
        u8 hsfreq;
        u32 min;
        u32 max;
        u32 default_mbps;
        u32 osc_freq_target;
};

#define DPHY_FREQ_RANGE_NUM             (63)
#define DPHY_FREQ_RANGE_INVALID_INDEX   (0xff)
const struct dwc_dphy_freq_range freqranges[DPHY_FREQ_RANGE_NUM] = {
        {0x00,  80,     97,     80,     335},
        {0x10,  80,     107,    90,     335},
        {0x20,  84,     118,    100,    335},
        {0x30,  93,     128,    110,    335},
        {0x01,  103,    139,    120,    335},
        {0x11,  112,    149,    130,    335},
        {0x21,  122,    160,    140,    335},
        {0x31,  131,    170,    150,    335},
        {0x02,  141,    181,    160,    335},
        {0x12,  150,    191,    170,    335},
        {0x22,  160,    202,    180,    335},
        {0x32,  169,    212,    190,    335},
        {0x03,  183,    228,    205,    335},
        {0x13,  198,    244,    220,    335},
        {0x23,  212,    259,    235,    335},
        {0x33,  226,    275,    250,    335},
        {0x04,  250,    301,    275,    335},
        {0x14,  274,    328,    300,    335},
        {0x25,  297,    354,    325,    335},
        {0x35,  321,    380,    350,    335},
        {0x05,  369,    433,    400,    335},
        {0x16,  416,    485,    450,    335},
        {0x26,  464,    538,    500,    335},
        {0x37,  511,    590,    550,    335},
        {0x07,  559,    643,    600,    335},
        {0x18,  606,    695,    650,    335},
        {0x28,  654,    748,    700,    335},
        {0x39,  701,    800,    750,    335},
        {0x09,  749,    853,    800,    335},
        {0x19,  796,    905,    850,    335},
        {0x29,  844,    958,    900,    335},
        {0x3a,  891,    1010,   950,    335},
        {0x0a,  939,    1063,   1000,   335},
        {0x1a,  986,    1115,   1050,   335},
        {0x2a,  1034,   1168,   1100,   335},
        {0x3b,  1081,   1220,   1150,   335},
        {0x0b,  1129,   1273,   1200,   335},
        {0x1b,  1176,   1325,   1250,   335},
        {0x2b,  1224,   1378,   1300,   335},
        {0x3c,  1271,   1430,   1350,   335},
        {0x0c,  1319,   1483,   1400,   335},
        {0x1c,  1366,   1535,   1450,   335},
        {0x2c,  1414,   1588,   1500,   335},
        {0x3d,  1461,   1640,   1550,   208},
        {0x0d,  1509,   1693,   1600,   214},
        {0x1d,  1556,   1745,   1650,   221},
        {0x2e,  1604,   1798,   1700,   228},
        {0x3e,  1651,   1850,   1750,   234},
        {0x0e,  1699,   1903,   1800,   241},
        {0x1e,  1746,   1955,   1850,   248},
        {0x2f,  1794,   2008,   1900,   255},
        {0x3f,  1841,   2060,   1950,   261},
        {0x0f,  1889,   2113,   2000,   268},
        {0x40,  1936,   2165,   2050,   275},
        {0x41,  1984,   2218,   2100,   281},
        {0x42,  2031,   2270,   2150,   288},
        {0x43,  2079,   2323,   2200,   294},
        {0x44,  2126,   2375,   2250,   302},
        {0x45,  2174,   2428,   2300,   308},
        {0x46,  2221,   2480,   2350,   315},
        {0x47,  2269,   2500,   2400,   321},
        {0x48,  2316,   2500,   2450,   328},
        {0x49,  2364,   2500,   2500,   335},
};

enum {
        IPU6_FW_PSYS_CMD_QUEUE_COMMAND_ID = 0,
        IPU6_FW_PSYS_CMD_QUEUE_DEVICE_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG0_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG1_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG2_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG3_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG4_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG5_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG6_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG7_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG8_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG9_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG10_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG11_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG12_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG13_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG14_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG15_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG16_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG17_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG18_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG19_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG20_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG21_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG22_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG23_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG24_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG25_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG26_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG27_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG28_COMMAND_ID,
        IPU6_FW_PSYS_CMD_QUEUE_PPG29_COMMAND_ID,
        IPU6_FW_PSYS_N_PSYS_CMD_QUEUE_ID
};

enum ipu_psys_cmd_state {
	KCMD_STATE_PPG_NEW,
	KCMD_STATE_PPG_START,
	KCMD_STATE_PPG_ENQUEUE,
	KCMD_STATE_PPG_STOP,
	KCMD_STATE_PPG_COMPLETE,
	N_KCMD_STATE_PPG_TYPE
};

static const char ipu_psys_cmd_state_types[N_KCMD_STATE_PPG_TYPE][32] = {
	"KCMD_STATE_PPG_NEW",
	"KCMD_STATE_PPG_START",
	"KCMD_STATE_PPG_ENQUEUE",
	"KCMD_STATE_PPG_STOP",
	"KCMD_STATE_PPG_COMPLETE",
};

enum ipu_psys_ppg_state {
	PPG_STATE_START = (1 << 0),
	PPG_STATE_STARTING = (1 << 1),
	PPG_STATE_STARTED = (1 << 2),
	PPG_STATE_RUNNING = (1 << 3),
	PPG_STATE_SUSPEND = (1 << 4),
	PPG_STATE_SUSPENDING = (1 << 5),
	PPG_STATE_SUSPENDED = (1 << 6),
	PPG_STATE_RESUME = (1 << 7),
	PPG_STATE_RESUMING = (1 << 8),
	PPG_STATE_RESUMED = (1 << 9),
	PPG_STATE_STOP = (1 << 10),
	PPG_STATE_STOPPING = (1 << 11),
	PPG_STATE_STOPPED = (1 << 12),
};

enum ipu_psys_ppg_type_state {
	PPG_TYPE_STATE_START =  0,
	PPG_TYPE_STATE_STARTING,
	PPG_TYPE_STATE_STARTED,
	PPG_TYPE_STATE_RUNNING,
	PPG_TYPE_STATE_SUSPEND,
	PPG_TYPE_STATE_SUSPENDING,
	PPG_TYPE_STATE_SUSPENDED,
	PPG_TYPE_STATE_RESUME,
	PPG_TYPE_STATE_RESUMING,
	PPG_TYPE_STATE_RESUMED,
	PPG_TYPE_STATE_STOP,
	PPG_TYPE_STATE_STOPPING,
	PPG_TYPE_STATE_STOPPED,
	N_STATE_PPG_TYPE
};

static const char ipu_psys_ppg_state_str[N_STATE_PPG_TYPE][32] = {
	"PPG_STATE_START",
	"PPG_STATE_STARTING",
	"PPG_STATE_STARTED",
	"PPG_STATE_RUNNING",
	"PPG_STATE_SUSPEND",
	"PPG_STATE_SUSPENDING",
	"PPG_STATE_SUSPENDED",
	"PPG_STATE_RESUME",
	"PPG_STATE_RESUMING",
	"PPG_STATE_RESUMED",
	"PPG_STATE_STOP",
	"PPG_STATE_STOPPING",
	"PPG_STATE_STOPPED",
};

struct ipu6_fw_state {
  enum ipu_fw_isys_send_type prev_send_t[IPU_ISYS_MAX_STREAMS];
  enum ipu_fw_isys_resp_type prev_resp_t[IPU_ISYS_MAX_STREAMS];
  enum ipu_fw_isys_state state[IPU_ISYS_MAX_STREAMS];
  unsigned int prev_source[IPU_ISYS_MAX_STREAMS];
  unsigned int prev_pid[IPU_ISYS_MAX_STREAMS];
  enum phy_fsm_state phy_state[IPU_DWC_DPHY_MAX_NUM];
  enum ipu_psys_ppg_state prev_ppg_state[IPU6_FW_PSYS_N_PSYS_CMD_QUEUE_ID];
  unsigned int prev_ppg_id[IPU6_FW_PSYS_N_PSYS_CMD_QUEUE_ID];
  int capture_cmd_count;
  bool first;
  bool phy_first;
  bool ppg_first;
  int phyid_state_ret;
} g_state = { .first = true, .phy_first = true,  .ppg_first = true, .phyid_state_ret = -1, .capture_cmd_count = 0};

/**
 * enum ipu_fw_isys_queue_type
 */
enum ipu_fw_isys_queue_type {
	IPU_FW_ISYS_QUEUE_TYPE_PROXY = 0,
	IPU_FW_ISYS_QUEUE_TYPE_DEV,
	IPU_FW_ISYS_QUEUE_TYPE_MSG,
	N_IPU_FW_ISYS_QUEUE_TYPE
};

/**
 * enum ipu_fw_isys_mipi_vc: MIPI csi2 spec
 * supports up to 4 virtual per physical channel
 */
enum ipu_fw_isys_mipi_vc {
	IPU_FW_ISYS_MIPI_VC_0 = 0,
	IPU_FW_ISYS_MIPI_VC_1,
	IPU_FW_ISYS_MIPI_VC_2,
	IPU_FW_ISYS_MIPI_VC_3,
	N_IPU_FW_ISYS_MIPI_VC
};

/**
 * enum vb2_buffer_state - current video buffer state.
 * @VB2_BUF_STATE_DEQUEUED:	buffer under userspace control.
 * @VB2_BUF_STATE_IN_REQUEST:	buffer is queued in media request.
 * @VB2_BUF_STATE_PREPARING:	buffer is being prepared in videobuf2.
 * @VB2_BUF_STATE_QUEUED:	buffer queued in videobuf2, but not in driver.
 * @VB2_BUF_STATE_ACTIVE:	buffer queued in driver and possibly used
 *				in a hardware operation.
 * @VB2_BUF_STATE_DONE:		buffer returned from driver to videobuf2, but
 *				not yet dequeued to userspace.
 * @VB2_BUF_STATE_ERROR:	same as above, but the operation on the buffer
 *				has ended with an error, which will be reported
 *				to the userspace when it is dequeued.
 */
enum vb2_buffer_state {
	VB2_BUF_STATE_DEQUEUED,
	VB2_BUF_STATE_IN_REQUEST,
	VB2_BUF_STATE_PREPARING,
	VB2_BUF_STATE_QUEUED,
	VB2_BUF_STATE_ACTIVE,
	VB2_BUF_STATE_DONE,
	VB2_BUF_STATE_ERROR,
	N_VB2_BUF_STATE_TYPE,
};

static const char vb2_buf_state_type[N_VB2_BUF_STATE_TYPE][32] = {
	"VB2_BUF_DEQUEUED",
	"VB2_BUF_IN_REQUEST",
	"VB2_BUF_PREPARING",
	"VB2_BUF_QUEUED",
	"VB2_BUF_ACTIVE",
	"VB2_BUF_DONE",
	"VB2_BUF_ERROR"
};

struct device_private___local;
struct device_driver___local;
struct device_type___local;
struct bus_type___local;

/**
 * struct device - The basic device structure
 * @parent:	The device's "parent" device, the device to which it is attached.
 * 		In most cases, a parent device is some sort of bus or host
 * 		controller. If parent is NULL, the device, is a top-level device,
 * 		which is not usually what you want.
 * @p:		Holds the private data of the driver core portions of the device.
 * 		See the comment of the struct device_private for detail.
 * @kobj:	A top-level, abstract class from which other classes are derived.
 * @init_name:	Initial name of the device.
 * @type:	The type of device.
 * 		This identifies the device type and carries type-specific
 * 		information.
 * @bus:	Type of bus device is on.
 * @driver:	Which driver has allocated this
 */
struct device___local {
	struct kobject___local kobj;
	struct device___local		*parent;

	struct device_private___local	*p;

	const char		*init_name; /* initial name of the device */
	const struct device_type___local *type;

	const struct bus_type___local	*bus;	/* type of bus device is on */
	struct device_driver___local *driver;	/* which driver has allocated this
						   device */
  	void            *platform_data; /* Platform specific data, device
					   core doesn't touch it */
	void            *driver_data;   /* Driver data, set and get with
					   dev_set_drvdata/dev_get_drvdata */
} __attribute__((preserve_access_index));

#define ipu_bus_get_drvdata(adev) dev_get_drvdata(&(adev)->dev)

/*
 * placeholder
 */
struct device_private___local {
	void *children;
} __attribute__((preserve_access_index));

/*
 * The type of device, "struct device" is embedded in. A class
 * or bus can contain devices of different types
 * like "partitions" and "disks", "mouse" and "event".
 * This identifies the device type and carries type-specific
 * information, equivalent to the kobj_type of a kobject.
 * If "name" is specified, the uevent will contain it in
 * the DEVTYPE variable.
 */
struct device_type___local {
	const char *name;
} __attribute__((preserve_access_index));

/**
 * struct bus_type - The bus type of the device
 *
 * @name:	The name of the bus.
 * @dev_name:	Used for subsystems to enumerate devices like ("foo%u", dev->id).
 */
struct bus_type___local {
	const char		*name;
	const char		*dev_name;
} __attribute__((preserve_access_index));

/**
 * struct device_driver - The basic device driver structure
 * @name:	Name of the device driver.
 * @bus:	The bus which the device of this driver belongs to.
 */
struct device_driver___local {
	const char		*name;
	const struct bus_type___local	*bus;
} __attribute__((preserve_access_index));

struct media_devnode___local;

/**
 * struct media_device - Media device
 * @dev:	Parent device
 * @devnode:	Media device node
 * @driver_name: Optional device driver name. If not set, calls to
 *		%MEDIA_IOC_DEVICE_INFO will return ``dev->driver->name``.
 *		This is needed for USB drivers for example, as otherwise
 *		they'll all appear as if the driver name was "usb".
 * @model:	Device model name
 * @serial:	Device serial number (optional)
 * @bus_info:	Unique and stable device location identifier
 * @hw_revision: Hardware device revision
 * @topology_version: Monotonic counter for storing the version of the graph
 *		topology. Should be incremented each time the topology changes.
 * @id:		Unique ID used on the last registered graph object
 */
struct media_device___local {
	/* dev->driver_data points to this struct. */
	struct device___local *dev;
	struct media_devnode___local *devnode;

	char model[32];
	char driver_name[32];
	char serial[40];
	char bus_info[32];
	u32 hw_revision;

	u64 topology_version;

	u32 id;
} __attribute__((preserve_access_index));


/**
 * struct media_devnode - Media device node
 * @media_dev:	pointer to struct &media_device
 */
struct media_devnode___local {
	struct media_device___local *media_dev;
} __attribute__((preserve_access_index));

/**
 * struct media_gobj - Define a graph object.
 *
 * @mdev:	Pointer to the struct &media_device that owns the object
 * @id:		Non-zero object ID identifier. The ID should be unique
 *		inside a media_device, as it is composed by
 *		%MEDIA_BITS_PER_TYPE to store the type plus
 *		%MEDIA_BITS_PER_ID to store the ID
 * @list:	List entry stored in one of the per-type mdev object lists
 *
 * All objects on the media graph should have this struct embedded
 */
struct media_gobj___local {
	struct media_device___local	*mdev;
	u32			id;
	struct list_head	list;
} __attribute__((preserve_access_index));

/**
 * struct media_pad - A media pad graph object.
 *
 * @graph_obj:	Embedded structure containing the media object common data
 * @entity:	Entity this pad belongs to
 * @index:	Pad index in the entity pads array, numbered from 0 to n
 */
struct media_pad___local {
	struct media_gobj___local graph_obj;	/* must be first field in struct */
	struct media_entity___local *entity;
	u16 index;
} __attribute__((preserve_access_index));


/**
 * struct media_pipeline - Media pipeline related information
 *
 * @allocated:		Media pipeline allocated and freed by the framework
 * @mdev:		The media device the pipeline is part of
 * @pads:		List of media_pipeline_pad
 * @start_count:	Media pipeline start - stop count
 */
struct media_pipeline___local {
	bool allocated;
	struct media_device___local *mdev;
	struct list_head pads;
	int start_count;
} __attribute__((preserve_access_index));


#define V4L2_DEVICE_NAME_SIZE (20 + 16)

/**
 * struct v4l2_device - main struct to for V4L2 device drivers
 *
 * @dev: pointer to struct device.
 * @mdev: pointer to struct media_device, may be NULL.
 * @subdevs: used to keep track of the registered subdevs
 * @lock: lock this struct; can be used by the driver as well
 *	if this struct is embedded into a larger struct.
 * @name: unique device name, by default the driver name + bus ID
 */
struct v4l2_device___local {
	struct device___local *dev;
	struct media_device___local *mdev;
	struct list_head subdevs;
	spinlock_t lock;
	char name[V4L2_DEVICE_NAME_SIZE];
} __attribute__((preserve_access_index));


/**
 * struct v4l2_plane_pix_format - additional, per-plane format definition
 * @sizeimage:		maximum size in bytes required for data, for which
 *			this plane will be used
 * @bytesperline:	distance in bytes between the leftmost pixels in two
 *			adjacent lines
 * @reserved:		drivers and applications must zero this array
 */
struct v4l2_plane_pix_format___local {
	__u32		sizeimage;
	__u32		bytesperline;
	__u16		reserved[6];
} __attribute__ ((packed, preserve_access_index));

/**
 * struct v4l2_pix_format_mplane - multiplanar format definition
 * @width:		image width in pixels
 * @height:		image height in pixels
 * @pixelformat:	little endian four character code (fourcc)
 * @field:		enum v4l2_field; field order (for interlaced video)
 * @colorspace:		enum v4l2_colorspace; supplemental to pixelformat
 * @plane_fmt:		per-plane information
 * @num_planes:		number of planes for this format
 * @flags:		format flags (V4L2_PIX_FMT_FLAG_*)
 * @ycbcr_enc:		enum v4l2_ycbcr_encoding, Y'CbCr encoding
 * @hsv_enc:		enum v4l2_hsv_encoding, HSV encoding
 * @quantization:	enum v4l2_quantization, colorspace quantization
 * @xfer_func:		enum v4l2_xfer_func, colorspace transfer function
 * @reserved:		drivers and applications must zero this array
 */
struct v4l2_pix_format_mplane___local {
	__u32				width;
	__u32				height;
	__u32				pixelformat;
	__u32				field;
	__u32				colorspace;

	struct v4l2_plane_pix_format___local	plane_fmt[VIDEO_MAX_PLANES];
	__u8				num_planes;
	__u8				flags;
	 union {
		__u8				ycbcr_enc;
		__u8				hsv_enc;
	};
	__u8				quantization;
	__u8				xfer_func;
	__u8				reserved[7];
} __attribute__ ((packed, preserve_access_index));

/*
 * Private elements (won't appear at the uAPI book):
 */
struct vb2_queue___local {
  unsigned int                    type;
  unsigned int                    io_modes;
  struct device___local                   *dev;
  unsigned long                   dma_attrs;
  unsigned int                    bidirectional:1;
  unsigned int                    fileio_read_once:1;
  unsigned int                    fileio_write_immediately:1;
  unsigned int                    allow_zero_bytesused:1;
  unsigned int               quirk_poll_must_check_waiting_for_buffers:1;
  unsigned int                    supports_requests:1;
  unsigned int                    requires_requests:1;
  unsigned int                    uses_qbuf:1;
  unsigned int                    uses_requests:1;
  unsigned int                    allow_cache_hints:1;
  unsigned int                    non_coherent_mem:1;
} __attribute__((preserve_access_index));

/*
 * struct ipu_isys BTF info
 *
 */
struct ipu_isys_sensor_info___local {
	unsigned int vc1_data_start;
	unsigned int vc1_data_end;
	unsigned int vc0_data_start;
	unsigned int vc0_data_end;
	unsigned int vc1_pdaf_start;
	unsigned int vc1_pdaf_end;
	unsigned int sensor_metadata;
} __attribute__((preserve_access_index));

struct ipu_bus_driver___local {
	struct device_driver___local drv;
	const char *wanted;
} __attribute__((preserve_access_index));

struct ipu_bus_device___local {
	struct device___local dev;
	struct list_head list;
	void *pdata;
	struct ipu_bus_driver___local *adrv;
} __attribute__((preserve_access_index));

struct sequence_info___local {
	unsigned int sequence;
	u64 timestamp;
} __attribute__((preserve_access_index));

struct ipu_isys_pipeline___local {
	struct media_pipeline___local pipe;
	struct media_pad___local *external;
	atomic_t sequence;
	int last_sequence;
	unsigned int seq_index;
	struct sequence_info___local seq[IPU_ISYS_MAX_PARALLEL_SOF];
	int source;	/* SSI stream source */
	int stream_handle;	/* stream handle for CSS API */
	unsigned int nr_output_pins;	/* How many firmware pins? */
	void *csi2_be;
	void *csi2_be_soc;
	void *csi2;

	/*
	 * Number of capture queues, write access serialised using struct
	 * ipu_isys.stream_mutex
	 */
	/* If it supports vc, this is number of links for the same vc. */
	int nr_queues;
	int nr_streaming;	/* Number of capture queues streaming */
	int streaming;	/* Has streaming been really started? */
} __attribute__((preserve_access_index));

struct ipu_fw_com_context___local {
	struct ipu_bus_device___local *adev;
} __attribute__((preserve_access_index));

/*
 * struct ipu_isys BTF info
 *
 * @media_dev: Media device
 * @v4l2_dev: V4L2 device
 * @adev: ISYS bus device
 * @power: Is ISYS powered on or not?
 * @isr_bits: Which bits does the ISR handle?
 * @power_lock: Serialise access to power (power state in general)
 * @csi2_rx_ctrl_cached: cached shared value between all CSI2 receivers
 * @lock: serialise access to pipes
 * @pipes: pipelines per stream ID
 * @fwcom: fw communication layer private pointer
 *         or optional external library private pointer
 * @line_align: line alignment in memory
 * @phy_termcal_val: the termination calibration value, only used for DWC PHY
 * @reset_needed: Isys requires d0i0->i3 transition
 * @video_opened: total number of opened file handles on video nodes
 */
struct ipu_isys___local {
	struct media_device___local media_dev;
	struct v4l2_device___local v4l2_dev;
	struct ipu_bus_device___local *adev;

	int power;
	spinlock_t power_lock;	/* Serialise access to power */
	u32 isr_csi2_bits;
	u32 csi2_rx_ctrl_cached;
	spinlock_t lock;	/* Serialise access to pipes */
	struct ipu_isys_pipeline___local *pipes[IPU_ISYS_MAX_STREAMS];
	void *fwcom;
	unsigned int line_align;
	u32 phy_termcal_val;
	bool reset_needed;
	bool icache_prefetch;
	bool csi2_cse_ipc_not_supported;
	unsigned int video_opened;
	unsigned int stream_opened;
	struct ipu_isys_sensor_info___local sensor_info;
	unsigned int sensor_types[N_IPU_FW_ISYS_SENSOR_TYPE];
} __attribute__((preserve_access_index));


/*
 * struct ipu_isys_sub_stream_vc
 */
struct ipu_isys_sub_stream_vc___local {
	unsigned int substream;	/* sub stream id */
	int vc;	/* VC number */
	u32 width;
	u32 height;
	unsigned int dt;
	unsigned int code;
} __attribute__((preserve_access_index));


struct ipu_isys_pixelformat___local {
	u32 pixelformat;
	u32 bpp;
	u32 bpp_packed;
	u32 bpp_planar;
	u32 code;
	u32 css_pixelformat;
} __attribute__((preserve_access_index));

struct ipu_isys_buffer_list___local {
	struct list_head head;
	unsigned int nbufs;
} __attribute__((preserve_access_index));

struct ipu_isys_queue___local {
	struct list_head node;	/* struct ipu_isys_pipeline.queues */
	struct vb2_queue___local vbq;
	struct device___local *dev;
	spinlock_t lock;
	struct list_head active;
	struct list_head incoming;
} __attribute__((preserve_access_index));


struct video_device___local {
#if defined(CONFIG_MEDIA_CONTROLLER)
  struct media_entity___local entity;
  struct media_intf_devnode___local *intf_devnode;
  struct media_pipeline___local pipe;
#endif
  const struct v4l2_file_operations___local *fops;

  u32 device_caps;

  /* sysfs */
  struct device___local dev;
  struct cdev___local *cdev;

  struct v4l2_device___local *v4l2_dev;
  struct device___local *dev_parent;

  struct v4l2_ctrl_handler___local *ctrl_handler;

  struct vb2_queue___local *queue;

  struct v4l2_prio_state___local *prio;

  /* device info */
  char name[32];
} __attribute__((preserve_access_index));

struct ipu_isys_video___local {
	/* Serialise access to other fields in the struct. */
	struct mutex___local mutex;
	struct media_pad___local pad;
	struct video_device___local vdev;
	struct v4l2_pix_format_mplane___local mpix;
	const struct ipu_isys_pixelformat___local *pfmts;
	const struct ipu_isys_pixelformat___local *pfmt;
	struct ipu_isys_queue___local aq;
	struct ipu_isys___local *isys;
	struct ipu_isys_pipeline___local ip;
	unsigned int streaming;
	unsigned int reset;
	unsigned int skipframe;
	unsigned int start_streaming;
} __attribute__((preserve_access_index));



/**
 * struct ipu_fw_isys_error_info_abi
 * @error: error code if something went wrong
 * @error_details: depending on error code, it may contain additional error info
 */
struct ipu_fw_isys_error_info_abi___local {
	enum ipu_fw_isys_error error;
	u32 error_details;
} __attribute__((preserve_access_index));


/**
 * struct ipu_fw_isys_output_pin_payload_abi
 * @out_buf_id: Points to output pin buffer - buffer identifier
 * @addr: Points to output pin buffer - CSS Virtual Address
 * @compress: Request frame compression (1), or  not (0)
 */
struct ipu_fw_isys_output_pin_payload_abi___local {
	u64 out_buf_id;
	u32 addr;
	u32 compress;
} __attribute__((preserve_access_index));


/**
 * struct ipu_fw_isys_resp_info_comm
 * @pin: this var is only valid for pin event related responses,
 *     contains pin addresses
 * @error_info: error information from the FW
 * @timestamp: Time information for event if available
 * @stream_handle: stream id the response corresponds to
 * @type: response type (enum ipu_fw_isys_resp_type)
 * @pin_id: pin id that the pin payload corresponds to
 */
struct ipu_fw_isys_resp_info_abi___local {
	u64 buf_id;
	struct ipu_fw_isys_output_pin_payload_abi___local pin;
	struct ipu_fw_isys_error_info_abi___local error_info;
	u32 timestamp[2];
	u8 stream_handle;
	u8 type;
	u8 pin_id;
	u16 reserved;
} __attribute__((preserve_access_index));


/**
 * struct vb2_buffer - represents a video buffer.
 * @vb2_queue:		pointer to &struct vb2_queue with the queue to
 *			which this driver belongs.
 * @index:		id number of the buffer.
 * @type:		buffer type.
 * @memory:		the method, in which the actual data is passed.
 * @num_planes:		number of planes in the buffer
 *			on an internal driver queue.
 * @timestamp:		frame timestamp in ns.
 * @request:		the request this buffer is associated with.
 * @req_obj:		used to bind this buffer to a request. This
 *			request object has a refcount.
 */
struct vb2_buffer___local {
	struct vb2_queue___local	*vb2_queue;
	unsigned int		index;
	unsigned int		type;
	unsigned int		memory;
	unsigned int		num_planes;
	u64			timestamp;
} __attribute__((preserve_access_index));

enum v4l2_ctrl_type___local {
	V4L2_CTRL_TYPE_INTEGER	     = 1,
	V4L2_CTRL_TYPE_BOOLEAN	     = 2,
	V4L2_CTRL_TYPE_MENU	     = 3,
	V4L2_CTRL_TYPE_BUTTON	     = 4,
	V4L2_CTRL_TYPE_INTEGER64     = 5,
	V4L2_CTRL_TYPE_CTRL_CLASS    = 6,
	V4L2_CTRL_TYPE_STRING        = 7,
	V4L2_CTRL_TYPE_BITMASK       = 8,
	V4L2_CTRL_TYPE_INTEGER_MENU  = 9,

	/* Compound types are >= 0x0100 */
	V4L2_CTRL_COMPOUND_TYPES     = 0x0100,
	V4L2_CTRL_TYPE_U8	     = 0x0100,
	V4L2_CTRL_TYPE_U16	     = 0x0101,
	V4L2_CTRL_TYPE_U32	     = 0x0102,
	V4L2_CTRL_TYPE_AREA          = 0x0106,

	V4L2_CTRL_TYPE_HDR10_CLL_INFO		= 0x0110,
	V4L2_CTRL_TYPE_HDR10_MASTERING_DISPLAY	= 0x0111,

	V4L2_CTRL_TYPE_H264_SPS             = 0x0200,
	V4L2_CTRL_TYPE_H264_PPS		    = 0x0201,
	V4L2_CTRL_TYPE_H264_SCALING_MATRIX  = 0x0202,
	V4L2_CTRL_TYPE_H264_SLICE_PARAMS    = 0x0203,
	V4L2_CTRL_TYPE_H264_DECODE_PARAMS   = 0x0204,
	V4L2_CTRL_TYPE_H264_PRED_WEIGHTS    = 0x0205,

	V4L2_CTRL_TYPE_FWHT_PARAMS	    = 0x0220,

	V4L2_CTRL_TYPE_VP8_FRAME            = 0x0240,

	V4L2_CTRL_TYPE_MPEG2_QUANTISATION   = 0x0250,
	V4L2_CTRL_TYPE_MPEG2_SEQUENCE       = 0x0251,
	V4L2_CTRL_TYPE_MPEG2_PICTURE        = 0x0252,

	V4L2_CTRL_TYPE_VP9_COMPRESSED_HDR	= 0x0260,
	V4L2_CTRL_TYPE_VP9_FRAME		= 0x0261,

	V4L2_CTRL_TYPE_HEVC_SPS			= 0x0270,
	V4L2_CTRL_TYPE_HEVC_PPS			= 0x0271,
	V4L2_CTRL_TYPE_HEVC_SLICE_PARAMS	= 0x0272,
	V4L2_CTRL_TYPE_HEVC_SCALING_MATRIX	= 0x0273,
	V4L2_CTRL_TYPE_HEVC_DECODE_PARAMS	= 0x0274,

	V4L2_CTRL_TYPE_AV1_SEQUENCE	    = 0x280,
	V4L2_CTRL_TYPE_AV1_TILE_GROUP_ENTRY = 0x281,
	V4L2_CTRL_TYPE_AV1_FRAME	    = 0x282,
	V4L2_CTRL_TYPE_AV1_FILM_GRAIN	    = 0x283,
};

/**
 * struct v4l2_ctrl_handler - The control handler keeps track of all the
 *	controls: both the controls owned by the handler and those inherited
 *	from other handlers.
 *
 * @_lock:	Default for "lock".
 * @lock:	Lock to control access to this handler and its controls.
 *		May be replaced by the user right after init.
 * @ctrls:	The list of controls owned by this handler.
 * @ctrl_refs:	The list of control references.
 */
struct v4l2_ctrl_handler___local {
	struct mutex___local _lock;
	struct mutex___local *lock;
	struct list_head ctrls;
	struct list_head ctrl_refs;
} __attribute__((preserve_access_index));

#define V4L2_CTRL_MAX_DIMS       (4)
/**
 * struct v4l2_ctrl - The control structure.
 *
 * @node:	The list node.
 * @ev_subs:	The list of control event subscriptions.
 * @handler:	The handler that owns the control.
 * @cluster:	Point to start of cluster array.
 * @ncontrols:	Number of controls in cluster array.
 * @done:	Internal flag: set for each processed control.
 * @is_new:	Set when the user specified a new value for this control. It
 *		is also set when called from v4l2_ctrl_handler_setup(). Drivers
 *		should never set this flag.
 * @has_changed: Set when the current value differs from the new value. Drivers
 *		should never use this flag.
 * @is_private: If set, then this control is private to its handler and it
 *		will not be added to any other handlers. Drivers can set
 *		this flag.
 * @is_auto:   If set, then this control selects whether the other cluster
 *		members are in 'automatic' mode or 'manual' mode. This is
 *		used for autogain/gain type clusters. Drivers should never
 *		set this flag directly.
 * @is_int:    If set, then this control has a simple integer value (i.e. it
 *		uses ctrl->val).
 * @is_string: If set, then this control has type %V4L2_CTRL_TYPE_STRING.
 * @is_ptr:	If set, then this control is an array and/or has type >=
 *		%V4L2_CTRL_COMPOUND_TYPES
 *		and/or has type %V4L2_CTRL_TYPE_STRING. In other words, &struct
 *		v4l2_ext_control uses field p to point to the data.
 * @is_array: If set, then this control contains an N-dimensional array.
 * @is_dyn_array: If set, then this control contains a dynamically sized 1-dimensional array.
 *		If this is set, then @is_array is also set.
 * @has_volatiles: If set, then one or more members of the cluster are volatile.
 *		Drivers should never touch this flag.
 * @call_notify: If set, then call the handler's notify function whenever the
 *		control's value changes.
 * @manual_mode_value: If the is_auto flag is set, then this is the value
 *		of the auto control that determines if that control is in
 *		manual mode. So if the value of the auto control equals this
 *		value, then the whole cluster is in manual mode. Drivers should
 *		never set this flag directly.
 * @ops:	The control ops.
 * @type_ops:	The control type ops.
 * @id:	The control ID.
 * @name:	The control name.
 * @type:	The control type.
 */
struct v4l2_ctrl___local {
	/* Administrative fields */
	struct list_head node;
	struct list_head ev_subs;
	struct v4l2_ctrl_handler___local *handler;
	struct v4l2_ctrl___local **cluster;
	unsigned int ncontrols;

	unsigned int done:1;

	unsigned int is_new:1;
	unsigned int has_changed:1;
	unsigned int is_private:1;
	unsigned int is_auto:1;
	unsigned int is_int:1;
	unsigned int is_string:1;
	unsigned int is_ptr:1;
	unsigned int is_array:1;
	unsigned int is_dyn_array:1;
	unsigned int has_volatiles:1;
	unsigned int call_notify:1;
	unsigned int manual_mode_value:8;

	void *ops;
	void *type_ops;
	u32 id;
	const char *name;
	enum v4l2_ctrl_type___local type;
	s64 minimum, maximum, default_value;
	u32 elems;
	u32 elem_size;
	u32 new_elems;
	u32 dims[V4L2_CTRL_MAX_DIMS];
	u32 nr_of_dims;
	union {
		u64 step;
		u64 menu_skip_mask;
	};
	union {
		const char * const *qmenu;
		const s64 *qmenu_int;
	};
	unsigned long flags;
	void *priv;
} __attribute__((preserve_access_index));

#define V4L2_SUBDEV_NAME_SIZE 32

struct v4l2_subdev___local {
#if defined(CONFIG_MEDIA_CONTROLLER)
	struct media_entity___local entity;
#endif
	struct list_head list;
	void  *owner;
	bool owner_v4l2_dev;
	u32 flags;
	struct v4l2_device___local *v4l2_dev;
	const void *ops;
	const void *internal_ops;
	struct v4l2_ctrl_handler___local *ctrl_handler;
	char name[V4L2_SUBDEV_NAME_SIZE];
} __attribute__((preserve_access_index));

#endif

struct cdev___local {
	struct kobject___local kobj;
} __attribute__((preserve_access_index));

struct wait_queue_head {
	spinlock_t		lock;
	struct list_head	head;
};
typedef struct wait_queue_head wait_queue_head_t;

struct ipu_psys_pdata___local {
	void *base;
} __attribute__((preserve_access_index));

struct ipu_psys_capability___local {
	u32 version;
	u8 driver[20];
	u32 pg_count;
	u8 dev_model[32];
	u32 reserved[17];
} __attribute__ ((packed));

struct ipu_psys___local {
	struct ipu_psys_capability___local caps;
	struct cdev___local cdev;
	struct device___local dev;

	struct mutex___local mutex;	/* Psys various */
	int ready; /* psys fw status */
	bool icache_prefetch_sp;
	bool icache_prefetch_isp;
	spinlock_t ready_lock;	/* protect psys firmware state */
	spinlock_t pgs_lock;	/* Protect pgs list access */
	struct list_head fhs;
	struct list_head pgs;
	struct list_head started_kcmds_list;
	struct ipu_psys_pdata___local *pdata;
	struct ipu_bus_device___local *adev;
} __attribute__((preserve_access_index));

struct ipu_psys_scheduler___local {
	struct list_head ppgs;
	struct mutex___local bs_mutex;  /* Protects buf_set field */
	struct list_head buf_sets;
} __attribute__((preserve_access_index));

#define IPU_FW_PSYS_KERNEL_BITMAP_NOF_ELEMS		4
#define IPU_FW_PSYS_RBM_NOF_ELEMS			5
#define IPU_FW_PSYS_KBM_NOF_ELEMS			4

struct ipu_fw_psys_process_group___local {
	u64 token;
	u64 private_token;
	u32 routing_bitmap[IPU_FW_PSYS_RBM_NOF_ELEMS];
	u32 kernel_bitmap[IPU_FW_PSYS_KBM_NOF_ELEMS];
	u32 size;
	u32 psys_server_init_cycles;
	u32 pg_load_start_ts;
	u32 pg_load_cycles;
	u32 pg_init_cycles;
	u32 pg_processing_cycles;
	u32 pg_next_frame_init_cycles;
	u32 pg_complete_cycles;
	u32 ID;
	u32 state;
	u32 ipu_virtual_address;
	u32 resource_bitmap;
	u16 fragment_count;
	u16 fragment_state;
	u16 fragment_limit;
	u16 processes_offset;
	u16 terminals_offset;
	u8 process_count;
	u8 terminal_count;
	u8 subgraph_count;
	u8 protocol_version;
	u8 base_queue_id;
	u8 num_queues;
	u8 mask_irq;
	u8 error_handling_enable;
} __attribute__ ((packed));

struct ipu_psys_pg___local {
	struct ipu_fw_psys_process_group___local *pg;
	size_t size;
	size_t pg_size;
	dma_addr_t pg_dma_addr;
} __attribute__((preserve_access_index));

struct ipu_psys_fh___local {
	struct ipu_psys___local *psys;
	struct mutex___local mutex;	/* Protects bufs_list & kcmds fields */
	struct list_head list;
	/* Holds all buffers that this fh owns */
	struct list_head bufs_list;
	/* Holds all descriptors (fd:kbuffer associations) */
	struct list_head descs_list;
	struct list_head bufs_lru;
	wait_queue_head_t wait;
	struct ipu_psys_scheduler___local sched;

	u32 num_bufs;
	u32 num_descs;
	u32 num_bufs_lru;
} __attribute__((preserve_access_index));

struct ipu_psys_ppg___local {
	struct ipu_psys_pg___local *kpg;
	struct ipu_psys_fh___local *fh;
	struct list_head list;
	struct list_head sched_list;
	u64 token;
	void *manifest;
	struct mutex___local mutex;     /* Protects kcmd and ppg state field */
	struct list_head kcmds_new_list;
	struct list_head kcmds_processing_list;
	struct list_head kcmds_finished_list;
	enum ipu_psys_ppg_state state;
	u32 pri_base;
	int pri_dynamic;
} __attribute__((preserve_access_index));

struct ipu_fw_psys_buffer_set___local {
	u64 token;
	u32 kernel_enable_bitmap[IPU_FW_PSYS_KERNEL_BITMAP_NOF_ELEMS];
	u32 terminal_enable_bitmap[IPU_FW_PSYS_KERNEL_BITMAP_NOF_ELEMS];
	u32 routing_enable_bitmap[IPU_FW_PSYS_KERNEL_BITMAP_NOF_ELEMS];
	u32 rbm[IPU_FW_PSYS_RBM_NOF_ELEMS];
	u32 ipu_virtual_address;
	u32 process_group_handle;
	u16 terminal_count;
	u8 frame_counter;
} __attribute__((preserve_access_index));

struct ipu_psys_kcmd___local;

struct ipu_psys_buffer_set___local {
	struct list_head list;
	struct ipu_fw_psys_buffer_set___local *buf_set;
	size_t size;
	size_t buf_set_size;
	dma_addr_t dma_addr;
	void *kaddr;
	struct ipu_psys_kcmd___local *kcmd;
} __attribute__((preserve_access_index));

struct ipu_psys_kbuffer___local {
	u64 len;
	void *userptr;
	void *kaddr;
	struct list_head list;
	dma_addr_t dma_addr;
} __attribute__((preserve_access_index));

struct ipu_psys_buffer___local;

struct ipu_psys_kcmd___local {
	struct ipu_psys_fh___local *fh;
	struct list_head list;
	struct ipu_psys_buffer_set___local *kbuf_set;
	enum ipu_psys_cmd_state state;
	void *pg_manifest;
	size_t pg_manifest_size;
	struct ipu_psys_kbuffer___local **kbufs;
	struct ipu_psys_buffer___local *buffers;
	size_t nbuffers;
	struct ipu_fw_psys_process_group___local *pg_user;
	struct ipu_psys_pg___local *kpg;
	u64 user_token;
	u64 issue_id;
	u32 priority;
} __attribute__((preserve_access_index));


/**
 * struct ipu_psys_buffer - for input/output terminals
 * @len:	total allocated size @ base address
 * @userptr:	user pointer
 * @fd:		DMA-BUF handle
 * @data_offset:offset to valid data
 * @bytes_used:	amount of valid data including offset
 * @flags:	flags
 */
struct ipu_psys_buffer___local {
	u64 len;
	union {
		int fd;
		void *userptr;
		u64 reserved;
	} base;
	u32 data_offset;
	u32 bytes_used;
	u32 flags;
} __attribute__((preserve_access_index));

#define IPU_BUFFER_FLAG_INPUT	(1 << 0)
#define IPU_BUFFER_FLAG_OUTPUT	(1 << 1)
#define IPU_BUFFER_FLAG_MAPPED	(1 << 2)
#define IPU_BUFFER_FLAG_NO_FLUSH	(1 << 3)
#define IPU_BUFFER_FLAG_DMA_HANDLE	(1 << 4)
#define IPU_BUFFER_FLAG_USERPTR	(1 << 5)

#define	IPU_PSYS_CMD_PRIORITY_HIGH	0
#define	IPU_PSYS_CMD_PRIORITY_MED	1
#define	IPU_PSYS_CMD_PRIORITY_LOW	2
#define	IPU_PSYS_CMD_PRIORITY_NUM	3

/**
 * struct ipu_psys_command - processing command
 * @issue_id:		unique id for the command set by user
 * @user_token:		token of the command
 * @priority:		priority of the command
 * @pg_manifest:	userspace pointer to program group manifest
 * @buffers:		userspace pointers to array of psys dma buf structs
 * @pg:			process group DMA-BUF handle
 * @pg_manifest_size:	size of program group manifest
 * @bufcount:		number of buffers in buffers array
 * @min_psys_freq:	minimum psys frequency in MHz used for this cmd
 * @frame_counter:      counter of current frame synced between isys and psys
 * @kernel_enable_bitmap:       enable bits for each individual kernel
 * @terminal_enable_bitmap:     enable bits for each individual terminals
 * @routing_enable_bitmap:      enable bits for each individual routing
 * @rbm:                        enable bits for routing
 *
 * Specifies a processing command with input and output buffers.
 */
struct ipu_psys_command___local {
	u64 issue_id;
	u64 user_token;
	u32 priority;
	void *pg_manifest;
	struct ipu_psys_buffer___local *buffers;
	int pg;
	u32 pg_manifest_size;
	u32 bufcount;
	u32 min_psys_freq;
	u32 frame_counter;
	u32 kernel_enable_bitmap[4];
	u32 terminal_enable_bitmap[4];
	u32 routing_enable_bitmap[4];
	u32 rbm[5];
} __attribute__((preserve_access_index));
