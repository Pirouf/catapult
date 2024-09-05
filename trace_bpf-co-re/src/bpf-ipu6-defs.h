/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef BPF_DEFS_H
#define BPF_DEFS_H

#include <stdbool.h>
#include <pthread.h>

#include <linux/v4l2-controls.h>
#include <linux/videodev2.h>

#define V4L2_CID_IPU_BASE	(V4L2_CID_USER_BASE + 0x1080)

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


#define arch_spinlock_t pthread_mutex_t

typedef u64 dma_addr_t;

typedef struct raw_spinlock___local {
	arch_spinlock_t raw_lock;
} raw_spinlock_t;

/* Non PREEMPT_RT kernels map spinlock to raw_spinlock */
typedef struct spinlock___local {
	union {
		struct raw_spinlock___local rlock;

	};
} spinlock_t;

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
	enum v4l2_ctrl_type type;
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
