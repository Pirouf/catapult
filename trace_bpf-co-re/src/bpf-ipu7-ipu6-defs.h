/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef BPF_IPU7_IPU6_DEFS_H
#define BPF_IPU7_IPU6_DEFS_H

#include <stdbool.h>

#include "bpf-v4l2-defs.h"

#define IPU_ISYS_MAX_STREAMS 16
#define IPU_INSYS_OUTPUT_PINS		11U
#define IPU_ISYS_MAX_PARALLEL_SOF	2U

#define VIDEO_MAX_PLANES               8

struct ipu7_isys___local;
struct ipu7_isys_stream___local;
struct ipu7_isys_video___local;
struct ipu7_isys_csi2___local;
struct ipu7_isys_queue___local;
struct ipu7_bus_device___local;

struct ipu6_isys___local;
struct ipu6_isys_stream___local;
struct ipu6_isys_video___local;
struct ipu6_isys_csi2___local;
struct ipu6_isys_queue___local;
struct ipu6_bus_device___local;

/**
 * enum ipu_fw_isys_resp_type
 */
enum ipu6_fw_isys_resp_type {
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

static const char ipu6_resp_msg_types[N_IPU_FW_ISYS_RESP_TYPE][32] = {
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

enum ipu7_insys_resp_type {
	IPU_INSYS_RESP_TYPE_STREAM_OPEN_DONE = 0,
	IPU_INSYS_RESP_TYPE_STREAM_START_AND_CAPTURE_ACK = 1,
	IPU_INSYS_RESP_TYPE_STREAM_CAPTURE_ACK = 2,
	IPU_INSYS_RESP_TYPE_STREAM_ABORT_ACK = 3,
	IPU_INSYS_RESP_TYPE_STREAM_FLUSH_ACK = 4,
	IPU_INSYS_RESP_TYPE_STREAM_CLOSE_ACK = 5,
	IPU_INSYS_RESP_TYPE_PIN_DATA_READY = 6,
	IPU_INSYS_RESP_TYPE_FRAME_SOF = 7,
	IPU_INSYS_RESP_TYPE_FRAME_EOF = 8,
	IPU_INSYS_RESP_TYPE_STREAM_START_AND_CAPTURE_DONE = 9,
	IPU_INSYS_RESP_TYPE_STREAM_CAPTURE_DONE = 10,
	N_IPU_INSYS_RESP_TYPE
};

static const char ipu7_resp_msg_types[N_IPU_INSYS_RESP_TYPE][32] = {
        "STREAM_OPEN_DONE",
	"STREAM_START_AND_CAPTURE_ACK",
	"STREAM_CAPTURE_ACK",
	"STREAM_STOP_ACK",
	"STREAM_FLUSH_ACK",
	"STREAM_CLOSE_ACK",
	"PIN_DATA_READY",
	"FRAME_SOF",
	"FRAME_EOF",
	"STREAM_START_AND_CAPTURE_DONE",
	"STREAM_CAPTURE_DONE",
};

/**
 * enum ipu_fw_isys_error. Describes the error type detected by the FW
 */
enum ipu6_fw_isys_error {
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

static const char ipu6_fw_isys_error_types[N_IPU_FW_ISYS_ERROR][32] = {
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

typedef u32	ia_gofo_addr_t;
#define IA_GOFO_ADDR_NULL	(0U)

#define IA_GOFO_MSG_ERR_MAX_DETAILS		(4U)
#define IA_GOFO_MSG_ERR_OK			(0U)
#define IA_GOFO_MSG_ERR_UNSPECIFED		(0xffffffffU)
#define IA_GOFO_MSG_ERR_GROUP_UNSPECIFIED	(0U)
#define IA_GOFO_MSG_ERR_IS_OK(err)	(IA_GOFO_MSG_ERR_OK == (err).err_code)

#define IA_GOFO_MSG_ERR_GROUP_APP_EXT_START	(16U)
#define IA_GOFO_MSG_ERR_GROUP_MAX		(31U)
#define IA_GOFO_MSG_ERR_GROUP_INTERNAL_START	(IA_GOFO_MSG_ERR_GROUP_MAX + 1U)
#define IA_GOFO_MSG_ERR_GROUP_RESERVED	IA_GOFO_MSG_ERR_GROUP_UNSPECIFIED
#define IA_GOFO_MSG_ERR_GROUP_GENERAL		1

enum insys_msg_err_groups {
	INSYS_MSG_ERR_GROUP_RESERVED = IA_GOFO_MSG_ERR_GROUP_RESERVED,
	INSYS_MSG_ERR_GROUP_GENERAL = IA_GOFO_MSG_ERR_GROUP_GENERAL,
	INSYS_MSG_ERR_GROUP_STREAM = 2,
	INSYS_MSG_ERR_GROUP_CAPTURE = 3,
	INSYS_MSG_ERR_GROUP_N,
};

static const char insys_msg_err_groups_types[INSYS_MSG_ERR_GROUP_N][32] = {
        "NONE",     /* No details */
	"ERR_GENERAL",      /* enum */
	"ERR_STREAM",       /* enum */
	"ERR_CAPTURE",      /* enum */
};

enum ia_gofo_msg_err_general {
	IA_GOFO_MSG_ERR_GENERAL_OK = IA_GOFO_MSG_ERR_OK,
	IA_GOFO_MSG_ERR_GENERAL_MSG_TOO_SMALL = 1,
	IA_GOFO_MSG_ERR_GENERAL_MSG_TOO_LARGE = 2,
	IA_GOFO_MSG_ERR_GENERAL_DEVICE_STATE = 3,
	IA_GOFO_MSG_ERR_GENERAL_ALIGNMENT = 4,
	IA_GOFO_MSG_ERR_GENERAL_INDIRECT_REF_PTR_INVALID = 5,
	IA_GOFO_MSG_ERR_GENERAL_INVALID_MSG_TYPE = 6,
	IA_GOFO_MSG_ERR_GENERAL_SYSCOM_FAIL = 7,
	IA_GOFO_MSG_ERR_GENERAL_N
};

static const char insys_msg_err_general_types[IA_GOFO_MSG_ERR_GENERAL_N][32] = {
        "NONE",     /* No details */
	"ERR_MSG_TOO_SMALL",      /* enum */
	"ERR_MSG_TOO_LARGE",       /* enum */
	"ERR_DEVICE_STATE",      /* enum */
	"ERR_ALIGNMENT",  /* enum */
	"INDIRECT_REF_PTR_INVALID",  /* enum */
	"ERR_INVALID_MSG_TYPE",   /* enum */
	"ERR_SYSCOM_FAIL",       /* enum */
};

enum insys_msg_err_stream {
	INSYS_MSG_ERR_STREAM_OK = IA_GOFO_MSG_ERR_OK,
	INSYS_MSG_ERR_STREAM_STREAM_ID = 1,
	INSYS_MSG_ERR_STREAM_MAX_OPINS = 2,
	INSYS_MSG_ERR_STREAM_MAX_IPINS = 3,
	INSYS_MSG_ERR_STREAM_STREAM_MESSAGES_MAP = 4,
	INSYS_MSG_ERR_STREAM_SYNC_MESSAGES_MAP = 5,
	INSYS_MSG_ERR_STREAM_SENSOR_TYPE = 6,
	INSYS_MSG_ERR_STREAM_FOREIGN_KEY = 7,
	INSYS_MSG_ERR_STREAM_STREAMING_MODE = 8,
	INSYS_MSG_ERR_STREAM_DPCM_EN = 9,
	INSYS_MSG_ERR_STREAM_DPCM_TYPE = 10,
	INSYS_MSG_ERR_STREAM_DPCM_PREDICTOR = 11,
	INSYS_MSG_ERR_STREAM_GRANULARITY_POINTER_UPDATE = 12,
	INSYS_MSG_ERR_STREAM_MPF_LUT_ENTRY_RESOURCES_BUSY = 13,
	INSYS_MSG_ERR_STREAM_MPF_DEV_ID = 14,
	INSYS_MSG_ERR_STREAM_BUFFER_LINES = 15,
	INSYS_MSG_ERR_STREAM_IPIN_ID = 16,
	INSYS_MSG_ERR_STREAM_DATA_TYPE = 17,
	INSYS_MSG_ERR_STREAM_STREAMING_PROTOCOL_STATE = 18,
	INSYS_MSG_ERR_STREAM_SYSCOM_FLUSH = 19,
	INSYS_MSG_ERR_STREAM_MIPI_VC = 20,
	INSYS_MSG_ERR_STREAM_STREAM_SRC = 21,
	INSYS_MSG_ERR_STREAM_PBK_ID = 22,
	INSYS_MSG_ERR_STREAM_CMD_QUEUE_DEALLOCATE = 23,
	INSYS_MSG_ERR_STREAM_INSUFFICIENT_RESOURCES = 24,
	INSYS_MSG_ERR_STREAM_IPIN_CONFIGURATION = 25,
	INSYS_MSG_ERR_STREAM_INVALID_STATE = 26,
	INSYS_MSG_ERR_STREAM_SW_MANAGED = 27,
	INSYS_MSG_ERR_STREAM_PBK_SLOT_ID = 28,
	INSYS_MSG_ERR_STREAM_FLUSH_TIMEOUT = 29,
	INSYS_MSG_ERR_STREAM_IPIN_WIDTH = 30,
	INSYS_MSG_ERR_STREAM_IPIN_HEIGHT = 31,
	INSYS_MSG_ERR_STREAM_OUTPUT_PIN_EARLY_ACK_EN = 32,
	INSYS_MSG_ERR_STREAM_INCONSISTENT_PARAMS = 33,
	INSYS_MSG_ERR_STREAM_PLANE_COUNT = 34,
	INSYS_MSG_ERR_STREAM_FRAME_FORMAT_TYPE = 35,
	INSYS_MSG_ERR_STREAM_INSUFFICIENT_RESOURCES_OUTPUT = 36,
	INSYS_MSG_ERR_STREAM_WIDTH_OUTPUT_SIZE = 37,
	INSYS_MSG_ERR_STREAM_CLOSED = 38,
	INSYS_MSG_ERR_STREAM_N
};


static const char insys_msg_err_stream_types[INSYS_MSG_ERR_STREAM_N][32] = {
        "NONE",     /* No details */
	"ERR_STREAM_ID",      /* enum */
	"ERR_MAX_OPINS",       /* enum */
	"ERR_MAX_IPINS",      /* enum */
	"ERR_MESSAGES_MAPALIGNMENT",  /* enum */
	"ERR_SYNC_MESSAGES_MAP",  /* enum */
	"ERR_SENSOR_TYPE",   /* enum */
	"ERR_FOREIGN_KEY",   /* enum */
	"ERR_STREAMING_MODE",   /* enum */
	"ERR_DPCM_EN",   /* enum */
	"ERR_DPCM_TYPE",   /* enum */
	"ERR_DPCM_PREDICTOR",   /* enum */
	"ERR_GRANULARITY_PTR_UPDATE",   /* enum */
	"ERR_MPF_LUT_ENTRY_BUSY",   /* enum */
	"ERR_MPF_DEV_ID",   /* enum */
	"ERR_BUFFER_LINES",   /* enum */
	"ERR_IPIN_ID",   /* enum */
	"ERR_DATA_TYPE",   /* enum */
	"ERR_STREAMING_PROTOCOL",   /* enum */
	"ERR_SYSCOM_FLUSH",   /* enum */
	"ERR_MIPI_VC",   /* enum */
	"ERR_STREAM_SRC",   /* enum */
	"ERR_PBK_ID",   /* enum */
	"ERR_CMD_QUEUE_DEALLOCATE",   /* enum */
	"ERR_INSUFFICIENT_RESOURCES",   /* enum */
	"ERR_IPIN_CONFIGURATION",   /* enum */
	"ERR_INVALID_STATE",   /* enum */
	"ERR_SW_MANAGED",   /* enum */
	"ERR_PBK_SLOT_ID",   /* enum */
	"ERR_FLUSH_TIMEOUT",   /* enum */
	"ERR_IPIN_WIDTH",   /* enum */
	"ERR_IPIN_HEIGHT",   /* enum */
	"ERR_OUTPIN_EARLY_ACK_EN",   /* enum */
	"ERR_INCONSISTENT_PARAMS",   /* enum */
	"ERR_PLANE_COUNT",   /* enum */
	"ERR_FRAME_FORMAT_TYPE",   /* enum */
	"ERR_INSUFFICIENT_RESOURCES_OUT",   /* enum */
	"ERR_WIDTH_OUTPUT_SIZE",   /* enum */
	"ERR_CLOSED",   /* enum */
};

enum insys_msg_err_capture {
	INSYS_MSG_ERR_CAPTURE_OK = IA_GOFO_MSG_ERR_OK,
	INSYS_MSG_ERR_CAPTURE_STREAM_ID = 1,
	INSYS_MSG_ERR_CAPTURE_PAYLOAD_PTR = 2,
	INSYS_MSG_ERR_CAPTURE_MEM_SLOT = 3,
	INSYS_MSG_ERR_CAPTURE_STREAMING_MODE = 4,
	INSYS_MSG_ERR_CAPTURE_AVAILABLE_CMD_SLOT = 5,
	INSYS_MSG_ERR_CAPTURE_CONSUMED_CMD_SLOT = 6,
	INSYS_MSG_ERR_CAPTURE_CMD_SLOT_PAYLOAD_PTR = 7,
	INSYS_MSG_ERR_CAPTURE_CMD_PREPARE = 8,
	INSYS_MSG_ERR_CAPTURE_OUTPUT_PIN = 9,
	INSYS_MSG_ERR_CAPTURE_SYNC_FRAME_DROP = 10,
	INSYS_MSG_ERR_CAPTURE_FRAME_MESSAGES_MAP = 11,
	INSYS_MSG_ERR_CAPTURE_TIMEOUT = 12,
	INSYS_MSG_ERR_CAPTURE_INVALID_STREAM_STATE = 13,
	INSYS_MSG_ERR_CAPTURE_HW_ERR_MULTIBIT_PH_ERROR_DETECTED = 14,
	INSYS_MSG_ERR_CAPTURE_HW_ERR_PAYLOAD_CRC_ERROR = 15,
	INSYS_MSG_ERR_CAPTURE_HW_ERR_INPUT_DATA_LOSS_ELASTIC_FIFO_OVFL  = 16,
	INSYS_MSG_ERR_CAPTURE_HW_ERR_PIXEL_BUFFER_OVERFLOW = 17,
	INSYS_MSG_ERR_CAPTURE_HW_ERR_BAD_FRAME_DIM = 18,
	INSYS_MSG_ERR_CAPTURE_HW_ERR_PHY_SYNC_ERR = 19,
	INSYS_MSG_ERR_CAPTURE_HW_ERR_SECURE_TOUCH = 20,
	INSYS_MSG_ERR_CAPTURE_HW_ERR_MASTER_SLAVE_SYNC_ERR = 21,
	INSYS_MSG_ERR_CAPTURE_FRAME_SKIP_ERR = 22,
	INSYS_MSG_ERR_CAPTURE_FE_INPUT_FIFO_OVERFLOW_ERR = 23,
	INSYS_MSG_ERR_CAPTURE_CMD_SUBMIT_TO_HW = 24,
	INSYS_MSG_ERR_CAPTURE_N
};

static const char insys_msg_err_capture_types[INSYS_MSG_ERR_CAPTURE_N][32] = {
        "NONE",     /* No details */
	"ERR_STREAM_ID",      /* enum */
	"ERR_PAYLOAD_PTR",      /* enum */
	"ERR_MEM_SLOT",      /* enum */
	"ERR_STREAMING_MODE",      /* enum */
	"ERR_AVAILABLE_CMD_SLOT",      /* enum */
	"ERR_CONSUMED_CMD_SLOT",      /* enum */
	"ERR_CMD_SLOT_PAYLOAD_PTR",      /* enum */
	"ERR_CMD_PREPARE",      /* enum */
	"ERR_OUTPUT_PIN",      /* enum */
	"ERR_SYNC_FRAME_DROP",      /* enum */
	"ERR_FRAME_MESSAGES_MAP",      /* enum */
	"ERR_TIMEOUT",      /* enum */
	"ERR_INVALID_STREAM_STATE",      /* enum */
	"HW_ERR_MULTIBIT_PH_DETECTED",      /* enum */
	"HW_ERR_PAYLOAD_CRC_ERROR",      /* enum */
	"HW_ERR_DATALOSS_FIFO_OVFL",      /* enum */
	"HW_ERR_PIXEL_BUF_OVERFLOW",      /* enum */
	"HW_ERR_BAD_FRAME_DIM",      /* enum */
	"HW_ERR_PHY_SYNC_ERR",      /* enum */
	"HW_ERR_SECURE_TOUCH",      /* enum */
	"HW_ERR_MST_SLV_SYNC_ERR",      /* enum */
	"ERR_FRAME_SKIP",      /* enum */
	"ERR_FE_FIFO_OVERFLOW",      /* enum */
	"ERR_CMD_SUBMIT_TO_HW",      /* enum */
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
enum ipu6_fw_isys_send_type {
	IPU_FW_ISYS_SEND_TYPE_STREAM_OPEN = 0,
	IPU_FW_ISYS_SEND_TYPE_STREAM_START,
	IPU_FW_ISYS_SEND_TYPE_STREAM_START_AND_CAPTURE,
	IPU_FW_ISYS_SEND_TYPE_STREAM_CAPTURE,
	IPU_FW_ISYS_SEND_TYPE_STREAM_STOP,
	IPU_FW_ISYS_SEND_TYPE_STREAM_FLUSH,
	IPU_FW_ISYS_SEND_TYPE_STREAM_CLOSE,
	N_IPU_FW_ISYS_SEND_TYPE
};

static const char ipu6_send_msg_types[N_IPU_FW_ISYS_SEND_TYPE][32] = {
	"STREAM_OPEN",
	"STREAM_START",
	"STREAM_START_AND_CAPTURE",
	"STREAM_CAPTURE",
	"STREAM_STOP",
	"STREAM_FLUSH",
	"STREAM_CLOSE"
};

enum ipu7_insys_send_type {
	IPU_INSYS_SEND_TYPE_STREAM_OPEN = 0,
	IPU_INSYS_SEND_TYPE_STREAM_START_AND_CAPTURE = 1,
	IPU_INSYS_SEND_TYPE_STREAM_CAPTURE = 2,
	IPU_INSYS_SEND_TYPE_STREAM_ABORT = 3,
	IPU_INSYS_SEND_TYPE_STREAM_FLUSH = 4,
	IPU_INSYS_SEND_TYPE_STREAM_CLOSE = 5,
	N_IPU_INSYS_SEND_TYPE
};

static const char * const ipu7_send_msg_types[N_IPU_INSYS_SEND_TYPE] = {
	"STREAM_OPEN",
	"STREAM_START_AND_CAPTURE",
	"STREAM_CAPTURE",
	"STREAM_ABORT",
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

struct ipu7_csi2_error {
	const char *error_string;
	bool is_info_only;
};

/*
 * Strings corresponding to CSI-2 receiver errors are here.
 * Corresponding macros are defined in the header file.
 */
static const struct ipu7_csi2_error dphy_rx_errors[] = {
	{ "Error handler FIFO full", false },
	{ "Reserved Short Packet encoding detected", true },
	{ "Reserved Long Packet encoding detected", true },
	{ "Received packet is too short", false},
	{ "Received packet is too long", false},
	{ "Short packet discarded due to errors", false },
	{ "Long packet discarded due to errors", false },
	{ "CSI Combo Rx interrupt", false },
	{ "IDI CDC FIFO overflow(remaining bits are reserved as 0)", false },
	{ "Received NULL packet", true },
	{ "Received blanking packet", true },
	{ "Tie to 0", true },
	{ }
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

struct ipu_fw_state {
  u16 prev_send_t[IPU_ISYS_MAX_STREAMS];
  u16 prev_resp_t[IPU_ISYS_MAX_STREAMS];
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

enum ipu_subsys___local {
	IPU_IS = 0,
	IPU_PS = 1,
	IPU_SUBSYS_NUM = 2,
};

#define ipu_bus_get_drvdata(adev) dev_get_drvdata(&(adev)->dev)

struct sequence_info___local {
	unsigned int sequence;
	u64 timestamp;
} __attribute__((preserve_access_index));

/*
 * struct ipu6_isys_stream BTF info
 *
 */
struct ipu6_bus_device___local {
	struct auxiliary_device___local auxdev;
	const struct auxiliary_driver___local *auxdrv;
} __attribute__((preserve_access_index));

struct ipu6_fw_isys_error_info_abi___local {
	u32 error;
	u32 error_details;
} __attribute__((preserve_access_index));

struct ipu6_fw_isys_output_pin_payload_abi___local {
	u64 out_buf_id;
	u32 addr;
	u32 compress;
} __attribute__((preserve_access_index));

struct ipu6_fw_isys_resp_info_abi___local {
	u64 buf_id;
	struct ipu6_fw_isys_output_pin_payload_abi___local pin;
	struct ipu6_fw_isys_error_info_abi___local error_info;
	u32 timestamp[2];
	u8 stream_handle;
	u8 type;
	u8 pin_id;
	u8 reserved;
	u32 reserved2;
} __attribute__((preserve_access_index));


struct ipu6_insys_capture_output_pin_payload___local {
	u64 user_token;
	ia_gofo_addr_t addr;
	u8 pad[4];
} __attribute__((preserve_access_index));

struct ipu6_isys_stream___local {
	struct mutex___local mutex;
	struct media_entity___local *source_entity;
	atomic_t sequence;
	int last_sequence;
	unsigned int seq_index;
	struct sequence_info___local seq[IPU_ISYS_MAX_PARALLEL_SOF];
	int stream_source;
	int stream_handle;
	unsigned int nr_output_pins;
	struct ipu6_isys_subdev___local *asd;
} __attribute__((preserve_access_index));

/*
 * struct ipu6_isys BTF info
 *
 */
struct ipu6_isys___local {
	struct media_device___local media_dev;
	struct v4l2_device___local v4l2_dev;
	struct ipu6_bus_device___local *adev;

	int power;
	spinlock_t power_lock;	/* Serialise access to power */
	u32 isr_csi2_bits;
	u32 csi2_rx_ctrl_cached;
	spinlock_t streams_lock;
	struct ipu6_isys_stream___local streams[IPU_ISYS_MAX_STREAMS];
	int streams_ref_count[IPU_ISYS_MAX_STREAMS];
} __attribute__((preserve_access_index));


struct ipu6_isys_pixelformat___local {
	u32 pixelformat;
	u32 bpp;
	u32 bpp_packed;
	u32 code;
	u32 css_pixelformat;
} __attribute__((preserve_access_index));

struct ipu6_isys_buffer_list___local {
	struct list_head head;
	unsigned int nbufs;
} __attribute__((preserve_access_index));

/*
 * struct ipu6_isys_csi2
 */
#define IPU_NR_OF_CSI2_VC		16U
#define INVALID_VC_ID			-1
#define IPU_NR_OF_CSI2_SINK_PADS	1U
#define IPU_CSI2_PAD_SINK		0U
#define IPU_NR_OF_CSI2_SRC_PADS		16U
#define IPU_CSI2_PAD_SRC		1U
#define IPU_NR_OF_CSI2_PADS		(IPU_NR_OF_CSI2_SINK_PADS + \
					 IPU_NR_OF_CSI2_SRC_PADS)
struct ipu6_isys_subdev___local {
	struct v4l2_subdev___local sd;
	struct ipu6_isys___local *isys;
	u32 const *supported_codes;
	struct media_pad___local *pad;
	struct v4l2_ctrl_handler___local ctrl_handler;
	void (*ctrl_init)(struct v4l2_subdev___local *sd);
	int source;	/* SSI stream source; -1 if unset */
};

struct ipu6_isys_queue___local {
	struct list_head node;	/* struct ipu6_isys_pipeline.queues */
	struct vb2_queue___local vbq;
	struct device___local *dev;
	spinlock_t lock;
	struct list_head active;
	struct list_head incoming;
} __attribute__((preserve_access_index));

struct ipu6_isys_video___local {
	struct ipu6_isys_queue___local aq;
	/* Serialise access to other fields in the struct. */
	struct mutex___local mutex;
	struct media_pad___local pad;
	struct video_device___local vdev;
	struct v4l2_pix_format___local pix_fmt;
	struct ipu6_isys___local *isys;
	struct ipu6_isys_csi2___local *csi2;
	struct ipu6_isys_stream___local *stream;
	unsigned int streaming;
	u8 vc;
	u8 dt;
	unsigned int reset;
	unsigned int skipframe;
	unsigned int start_streaming;
} __attribute__((preserve_access_index));

struct ipu6_isys_csi2_pdata___local;

struct ipu6_isys_csi2___local {
	struct ipu6_isys_subdev___local asd;
	struct ipu6_isys_csi2_pdata___local *pdata;
	struct ipu6_isys___local *isys;
	struct ipu6_isys_video___local av[IPU_NR_OF_CSI2_SRC_PADS];
} __attribute__((preserve_access_index));

/*
 * struct ipu7_isys_stream BTF info
 *
 */
struct ipu7_bus_device___local {
	struct auxiliary_device___local auxdev;
	const struct auxiliary_driver___local *auxdrv;
} __attribute__((preserve_access_index));

#pragma pack(push, 1)
struct ia_gofo_msg_err___local {
	u32 err_group;
	u32 err_code;
	u32 err_detail[IA_GOFO_MSG_ERR_MAX_DETAILS];
} __attribute__((preserve_access_index));
#pragma pack(pop)

struct ipu7_insys_capture_output_pin_payload___local {
	u64 user_token;
	ia_gofo_addr_t addr;
	u8 pad[4];
} __attribute__((preserve_access_index));

struct ipu7_insys_resp___local {
	u64 buf_id;
	struct ipu7_insys_capture_output_pin_payload___local pin;
	struct ia_gofo_msg_err___local error_info;
	u32 timestamp[2];
	u8 type;
	u8 msg_link_streaming_mode;
	u8 stream_id;
	u8 pin_id;
	u8 frame_id;
	u8 skip_frame;
	u16 mipi_fn;
} __attribute__((preserve_access_index));

struct ipu7_isys_stream___local {
	struct mutex___local mutex;
	struct media_entity___local *source_entity;
	atomic_t sequence;
	int last_sequence;
	unsigned int seq_index;
	struct sequence_info___local seq[IPU_ISYS_MAX_PARALLEL_SOF];
	int stream_source;
	int stream_handle;
	unsigned int nr_output_pins;
	struct ipu7_isys_subdev___local *asd;
} __attribute__((preserve_access_index));

/*
 * struct ipu7_isys BTF info
 *
 */
struct ipu7_isys___local {
	struct media_device___local media_dev;
	struct v4l2_device___local v4l2_dev;
	struct ipu7_bus_device___local *adev;

	int power;
	spinlock_t power_lock;	/* Serialise access to power */
	u32 isr_csi2_bits;
	u32 csi2_rx_ctrl_cached;
	spinlock_t streams_lock;
	struct ipu7_isys_stream___local streams[IPU_ISYS_MAX_STREAMS];
	int streams_ref_count[IPU_ISYS_MAX_STREAMS];
} __attribute__((preserve_access_index));


struct ipu7_isys_pixelformat___local {
	u32 pixelformat;
	u32 bpp;
	u32 bpp_packed;
	u32 code;
	u32 css_pixelformat;
} __attribute__((preserve_access_index));

struct ipu7_isys_buffer_list___local {
	struct list_head head;
	unsigned int nbufs;
} __attribute__((preserve_access_index));

/*
 * struct ipu7_isys_csi2
 */
#define IPU_NR_OF_CSI2_VC		16U
#define INVALID_VC_ID			-1
#define IPU_NR_OF_CSI2_SINK_PADS	1U
#define IPU_CSI2_PAD_SINK		0U
#define IPU_NR_OF_CSI2_SRC_PADS		16U
#define IPU_CSI2_PAD_SRC		1U
#define IPU_NR_OF_CSI2_PADS		(IPU_NR_OF_CSI2_SINK_PADS + \
					 IPU_NR_OF_CSI2_SRC_PADS)
struct ipu7_isys_subdev___local {
	struct v4l2_subdev___local sd;
	struct ipu7_isys___local *isys;
	u32 const *supported_codes;
	struct media_pad___local *pad;
	struct v4l2_ctrl_handler___local ctrl_handler;
	void (*ctrl_init)(struct v4l2_subdev___local *sd);
	int source;	/* SSI stream source; -1 if unset */
};

struct ipu7_isys_queue___local {
	struct list_head node;	/* struct ipu7_isys_pipeline.queues */
	struct vb2_queue___local vbq;
	struct device___local *dev;
	spinlock_t lock;
	struct list_head active;
	struct list_head incoming;
} __attribute__((preserve_access_index));

struct ipu7_isys_video___local {
	struct ipu7_isys_queue___local aq;
	/* Serialise access to other fields in the struct. */
	struct mutex___local mutex;
	struct media_pad___local pad;
	struct video_device___local vdev;
	struct v4l2_pix_format___local pix_fmt;
	struct ipu7_isys___local *isys;
	struct ipu7_isys_csi2___local *csi2;
	struct ipu7_isys_stream___local *stream;
	unsigned int streaming;
	u8 vc;
	u8 dt;
	unsigned int reset;
	unsigned int skipframe;
	unsigned int start_streaming;
} __attribute__((preserve_access_index));

struct ipu7_isys_csi2_pdata___local;

struct ipu7_isys_csi2___local {
	struct ipu7_isys_subdev___local asd;
	struct ipu7_isys_csi2_pdata___local *pdata;
	struct ipu7_isys___local *isys;
	struct ipu7_isys_video___local av[IPU_NR_OF_CSI2_SRC_PADS];
} __attribute__((preserve_access_index));


/*
 * struct ipu_psys BTF info
 *
 */

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
	struct ipu7_bus_device___local *adev;
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

#endif //BPF_IPU7_IPU6_DEFS_H
