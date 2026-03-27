/* SPDX-License-Identifier: GPL-2.0+ */
#include "ipu6-isys-legacy_ktrace_kern.c"

extern int LINUX_KERNEL_VERSION __kconfig;

/*************************/
#define DS5_DEPTH_STREAM_DT		0x4000
#define DS5_CAMERA_CID_BASE	(V4L2_CTRL_CLASS_CAMERA | DS5_DEPTH_STREAM_DT)

enum ds5_mux_pad {
	DS5_MUX_PAD_EXTERNAL,
	DS5_MUX_PAD_DEPTH,
	DS5_MUX_PAD_RGB,
	DS5_MUX_PAD_IR,
	DS5_MUX_PAD_IMU,
	DS5_MUX_PAD_COUNT,
};

static const char d4xx_mux_pad_types[DS5_MUX_PAD_COUNT][16] = {
        "IPU",
	"DEPTH",
	"RGB",
	"IR",
	"IMU",
};

enum d4xx__v4l2_cid_type {
	CID_LOG = 0,
	CID_LASER_POWER,
	CID_MANUAL_LASER_POWER,
	DEPTH_CALIBRATION_TABLE_GET,
	DEPTH_CALIBRATION_TABLE_SET,
	COEFF_CALIBRATION_TABLE_GET,
	COEFF_CALIBRATION_TABLE_SET,
	CID_FW_VERSION,
	CID_GVD,
	CID_AE_ROI_GET,
	CID_AE_ROI_SET,
	CID_AE_SETPOINT_GET ,
	CID_AE_SETPOINT_SET,
	CID_ERB,
	CID_EWB,
	CID_HWMC, // DS5_CAMERA_CID_BASE+15
	CID_PWM = 22, // DS5_CAMERA_CID_BASE+22
	CID_HWMC_RW = 32, // DS5_CAMERA_CID_BASE+32
	N_D4XX_V4L2_CID_TYPE
};

struct ds5_ctrls___local {
	struct v4l2_ctrl_handler___local handler;
	struct v4l2_ctrl_handler___local handler_depth;
	struct v4l2_ctrl_handler___local handler_rgb;
	struct v4l2_ctrl_handler___local handler_y8;
	struct v4l2_ctrl_handler___local handler_imu;
	struct {
		struct v4l2_ctrl___local *log;
		struct v4l2_ctrl___local *fw_version;
		struct v4l2_ctrl___local *gvd;
		struct v4l2_ctrl___local *get_depth_calib;
		struct v4l2_ctrl___local *set_depth_calib;
		struct v4l2_ctrl___local *get_coeff_calib;
		struct v4l2_ctrl___local *set_coeff_calib;
		struct v4l2_ctrl___local *ae_roi_get;
		struct v4l2_ctrl___local *ae_roi_set;
		struct v4l2_ctrl___local *ae_setpoint_get;
		struct v4l2_ctrl___local *ae_setpoint_set;
		struct v4l2_ctrl___local *erb;
		struct v4l2_ctrl___local *ewb;
		struct v4l2_ctrl___local *hwmc;
		struct v4l2_ctrl___local *laser_power;
		struct v4l2_ctrl___local *manual_laser_power;
		struct v4l2_ctrl___local *auto_exp;
		struct v4l2_ctrl___local *exposure;
		/* in DS5 manual gain only works with manual exposure */
		struct v4l2_ctrl___local *gain;
		struct v4l2_ctrl___local *link_freq;
		struct v4l2_ctrl___local *query_sub_stream;
		struct v4l2_ctrl___local *set_sub_stream;
	};
} __attribute__((preserve_access_index));

struct ds5_mux_subdev___local {
	struct v4l2_subdev___local subdev;
} __attribute__((preserve_access_index));

struct ds5_sensor___local {
	struct v4l2_subdev___local sd;
	struct media_pad___local pad;
	struct v4l2_mbus_framefmt___local format;
	u16 mux_pad;
} __attribute__((preserve_access_index));

struct ds5___local {
	struct { struct ds5_sensor___local sensor; } depth;
	struct { struct ds5_sensor___local sensor; } ir;
	struct { struct ds5_sensor___local sensor; } rgb;
	struct { struct ds5_sensor___local sensor; } imu;
	struct {
		struct ds5_mux_subdev___local sd;
		struct media_pad___local pads[DS5_MUX_PAD_COUNT];
		struct ds5_sensor___local *last_set;
	} mux;
	struct ds5_ctrls___local ctrls;
} __attribute__((preserve_access_index));

/*
*/
SEC("kprobe/ds5_g_volatile_ctrl")
int BPF_KPROBE(ds5_g_volatile_ctrl_entry, struct v4l2_ctrl___local *ctrl)
{
	u32 id;
	const char *name;
	BPF_CORE_READ_INTO(&id,  ctrl, id);
	BPF_CORE_READ_INTO(&name,  ctrl, name);

	struct ds5_sensor___local *sensor = (struct ds5_sensor___local *)BPF_CORE_READ(ctrl, priv);

	enum ds5_mux_pad mux_pad;
	BPF_CORE_READ_INTO(&mux_pad,  sensor, mux_pad);

	if (mux_pad != DS5_MUX_PAD_EXTERNAL &&
	    mux_pad < DS5_MUX_PAD_COUNT) {
	  // this is DS4XX
	  bpf_printk("graph_ent func=v4l2_g_volatile_ctrl args=name=%s;id=%u;type=%s",
		      name, id, d4xx_mux_pad_types[mux_pad]);
	} else if (id > V4L2_CID_IPU_BASE &&
	    id < (V4L2_CID_IPU_BASE + N_IPU_ISYS_V4L2_CID_TYPE) ) {

	  // this is IPU6_ISYS
	  bpf_printk("graph_ent func=v4l2_g_volatile_ctrl args=name=%s;id=%u;type=%s",
		      name, id, ipu_isys_v4l2_cid_types[id - V4L2_CID_IPU_BASE]);

	} else {
	  bpf_printk("graph_ent func=v4l2_g_volatile_ctrl args=name=%s;id=%u",
		      name, id);
	}
	return 0;
}

SEC("kretprobe/ds5_g_volatile_ctrl")
int BPF_KRETPROBE(ds5_g_volatile_ctrl_exit, int ret)
{
	bpf_printk("graph_ret func=v4l2_g_volatile_ctrl ret=%d", ret);
	return 0;
}



/*
*/
SEC("kprobe/ds5_s_ctrl")
int BPF_KPROBE(ds5_s_ctrl_entry, struct v4l2_ctrl___local *ctrl)
{
	u32 id;
	const char *name;
	BPF_CORE_READ_INTO(&id,  ctrl, id);
	BPF_CORE_READ_INTO(&name,  ctrl, name);

	struct ds5_sensor___local *sensor = (struct ds5_sensor___local *)BPF_CORE_READ(ctrl, priv);

	enum ds5_mux_pad mux_pad;
	BPF_CORE_READ_INTO(&mux_pad,  sensor, mux_pad);

	if (mux_pad != DS5_MUX_PAD_EXTERNAL &&
	    mux_pad < DS5_MUX_PAD_COUNT) {
	  // this is DS4XX
	  bpf_printk("graph_ent func=v4l2_s_ctrl args=name=%s;id=%u;type=%s",
		      name, id, d4xx_mux_pad_types[mux_pad]);

	} else if (id > V4L2_CID_IPU_BASE &&
	    id < (V4L2_CID_IPU_BASE + N_IPU_ISYS_V4L2_CID_TYPE) ) {

	  // this is IPU6_ISYS
	  bpf_printk("graph_ent func=v4l2_s_ctrl args=name=%s;id=%u;type=%s",
		      name, id, ipu_isys_v4l2_cid_types[id - V4L2_CID_IPU_BASE]);

	} else {
	  bpf_printk("graph_ent func=v4l2_s_ctrl args=name=%s;id=%u",
		      name, id);
	}
	return 0;
}

SEC("kretprobe/ds5_s_ctrl")
int BPF_KRETPROBE(ds5_s_ctrl_ctrl_exit, int ret)
{
	bpf_printk("graph_ret func=v4l2_s_ctrl ret=%d", ret);
	return 0;
}

SEC("kprobe/ipu_isys_video_s_ctrl")
int BPF_KPROBE(ipu_isys_video_s_ctrl_entry, struct v4l2_ctrl___local *ctrl)
{
	u32 id;
	const char *name;
	BPF_CORE_READ_INTO(&id,  ctrl, id);
	BPF_CORE_READ_INTO(&name,  ctrl, name);
	bpf_printk("graph_ent func=v4l2_s_ctrl args=name=%s;id=%d",
		    id, name);
	return 0;
}

SEC("kretprobe/ipu_isys_video_s_ctrl")
int BPF_KRETPROBE(ipu_isys_video_s_ctrl_exit, int ret)
{
	bpf_printk("graph_ret func=v4l2_s_ctrl ret=%d", ret);
	return 0;
}
