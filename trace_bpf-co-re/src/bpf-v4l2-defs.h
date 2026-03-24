/* SPDX-License-Identifier: GPL-2.0-or-later */

#ifndef BPF_V4L2_DEFS_H
#define BPF_V4L2_DEFS_H

#include <stdbool.h>

//#include <pthread.h> removed to avoid <stub32.h> i686 deps
//#include <linux/spinlock_types_raw.h>

#define EINVAL 22
#define ENETDOWN 100
#define EMSGSIZE 90
#define EOPNOTSUPP 95
#define ENOSPC 28

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

#define TASK_COMM_LEN 16
#define MAX_FILENAME_LEN 127

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


/*
 *	V I D E O   I M A G E   F O R M A T
 */
struct v4l2_pix_format___local {
	__u32			width;
	__u32			height;
	__u32			pixelformat;
	__u32			field;		/* enum v4l2_field */
	__u32			bytesperline;	/* for padding, zero if unused */
	__u32			sizeimage;
	__u32			colorspace;	/* enum v4l2_colorspace */
	__u32			priv;		/* private data, depends on pixelformat */
	__u32			flags;		/* format flags (V4L2_PIX_FMT_FLAG_*) */
	union {
		/* enum v4l2_ycbcr_encoding */
		__u32			ycbcr_enc;
		/* enum v4l2_hsv_encoding */
		__u32			hsv_enc;
	};
	__u32			quantization;	/* enum v4l2_quantization */
	__u32			xfer_func;	/* enum v4l2_xfer_func */
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

struct cdev___local {
	struct kobject___local kobj;
} __attribute__((preserve_access_index));

struct wait_queue_head {
	spinlock_t		lock;
	struct list_head	head;
};

struct swait_queue_head {
	raw_spinlock_t		lock;
	struct list_head	task_list;
};

typedef struct wait_queue_head wait_queue_head_t;

/*
 * struct completion - structure used to maintain state for a "completion"
 */
struct completion___local {
	unsigned int done;
	struct swait_queue_head wait;
} __attribute__((preserve_access_index));

struct auxiliary_device___local {
	struct device___local dev;
	const char *name;
	u32 id;
} __attribute__((preserve_access_index));

#define AUXILIARY_NAME_SIZE 32
#define AUXILIARY_MODULE_PREFIX "auxiliary:"

struct auxiliary_device_id___local {
	char name[AUXILIARY_NAME_SIZE];
} __attribute__((preserve_access_index));

typedef struct pm_message {
	int event;
} pm_message_t;

struct auxiliary_driver___local {
	int (*probe)(struct auxiliary_device___local *auxdev, const struct auxiliary_device_id___local *id);
	void (*remove)(struct auxiliary_device___local *auxdev);
	void (*shutdown)(struct auxiliary_device___local *auxdev);
	int (*suspend)(struct auxiliary_device___local *auxdev, pm_message_t state);
	int (*resume)(struct auxiliary_device___local *auxdev);
	const char *name;
	struct device_driver___local driver;
} __attribute__((preserve_access_index));

# define __iomem        __attribute__((noderef, address_space(2)))

#endif //BPF_V4L2_DEFS_H
