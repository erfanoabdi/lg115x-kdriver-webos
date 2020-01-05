/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2013 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or 
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of 
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
	GNU General Public License for more details.
*/ 

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>			/**< printk() */
#include <linux/slab.h>			 	/**< kmalloc() */
#include <linux/fs.h> 				/**< everything\ldots{} */
#include <linux/types.h>		 	/**< size_t */
#include <linux/fcntl.h>			/**< O_ACCMODE */
#include <asm/uaccess.h>
#include <linux/ioport.h>			/**< For request_region, check_region etc */
#include <asm/io.h>					/**< For ioremap_nocache */
#include <linux/workqueue.h>		/**< For working queue */
#include <linux/interrupt.h>
#include <linux/irq.h>

#include "ademod_common.h"
#include "ademod_common_Presets.h"

#define INIT_ABB_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_Init = {
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define NTSC_M_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_NTSC_M = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	4400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	45750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xfffe,0x2d,0xffa7,0xd9,0x2a2,0x13b,0xff12,0xb2,0xffb0,0x11,0xd,0xfff5,0xfff9,0x11,0xfff1,0x5,0x3,0xfff9,0x4,0xffff, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
	 { LX_ADEMOD_PARM_VID_GAIN, 	15750, },
};

#define NTSC_M_J_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_NTSC_M_J = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	4400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	45750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xfffe,0x2d,0xffa7,0xd9,0x2a2,0x13b,0xff12,0xb2,0xffb0,0x11,0xd,0xfff5,0xfff9,0x11,0xfff1,0x5,0x3,0xfff9,0x4,0xffff, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
	 { LX_ADEMOD_PARM_VID_GAIN, 	15750, },
};

#define NTSC_433_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_NTSC_433 = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	4400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	45750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xfffe,0x2d,0xffa7,0xd9,0x2a2,0x13b,0xff12,0xb2,0xffb0,0x11,0xd,0xfff5,0xfff9,0x11,0xfff1,0x5,0x3,0xfff9,0x4,0xffff, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
	 { LX_ADEMOD_PARM_VID_GAIN, 	15750, },
};

#define PAL_B_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_PAL_B = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	5400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	45750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xffee,0xffc2,0xff9f,0x29a,0x21f,0xff72,0x4,0xe7,0xfefe,0xee,0xff66,0x59,0xffdd,0xd,0xfffb,0x9,0xffef,0x13,0xfff0,0xb, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

//AlexL June 15th
#define PAL_D_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_PAL_D = {

	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	5400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },

	{ LX_ADEMOD_PARM_GRP_DEL, 	0xfff4,0xff78,0x2b,0x240,0x296,0xfea8,0x112,0xffdc,0xffe0,0x56,0xffad,0x4a,0xffc8,0x29,0xffe0,0x17,0xffef,0xb,0xfff8,0x5, },
};

#define PAL_H_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_PAL_H = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	5400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xfff4,0xff78,0x2b,0x240,0x296,0xfea8,0x112,0xffdc,0xffe0,0x56,0xffad,0x4a,0xffc8,0x29,0xffe0,0x17,0xffef,0xb,0xfff8,0x5, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

//AlexL June 15th
#define PAL_I_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_PAL_I = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	5400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },


	{ LX_ADEMOD_PARM_GRP_DEL, 	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3ff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0, },
};

#define PAL_M_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_PAL_M = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	4400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xfffe,0x2d,0xffa7,0xd9,0x2a2,0x13b,0xff12,0xb2,0xffb0,0x11,0xd,0xfff5,0xfff9,0x11,0xfff1,0x5,0x3,0xfff9,0x4,0xffff, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
	 { LX_ADEMOD_PARM_VID_GAIN, 	15750, },
};

#define PAL_N_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_PAL_N = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	4400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xfffe,0x2d,0xffa7,0xd9,0x2a2,0x13b,0xff12,0xb2,0xffb0,0x11,0xd,0xfff5,0xfff9,0x11,0xfff1,0x5,0x3,0xfff9,0x4,0xffff, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
	 { LX_ADEMOD_PARM_VID_GAIN, 	15750, },
};

#define PAL_60_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_PAL_60 = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	4400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xfffe,0x2d,0xffa7,0xd9,0x2a2,0x13b,0xff12,0xb2,0xffb0,0x11,0xd,0xfff5,0xfff9,0x11,0xfff1,0x5,0x3,0xfff9,0x4,0xffff, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
	 { LX_ADEMOD_PARM_VID_GAIN, 	15750, },
};

#define SECAM_B_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_SECAM_B = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	5400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xffee,0xffc2,0xff9f,0x29a,0x21f,0xff72,0x4,0xe7,0xfefe,0xee,0xff66,0x59,0xffdd,0xd,0xfffb,0x9,0xffef,0x13,0xfff0,0xb, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define SECAM_D_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_SECAM_D = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	6400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xfff4,0xff78,0x2b,0x240,0x296,0xfea8,0x112,0xffdc,0xffe0,0x56,0xffad,0x4a,0xffc8,0x29,0xffe0,0x17,0xffef,0xb,0xfff8,0x5, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define SECAM_G_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_SECAM_G = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	5400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xffee,0xffc2,0xff9f,0x29a,0x21f,0xff72,0x4,0xe7,0xfefe,0xee,0xff66,0x59,0xffdd,0xd,0xfffb,0x9,0xffef,0x13,0xfff0,0xb, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define SECAM_H_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_SECAM_H = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	5400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xfff4,0xff78,0x2b,0x240,0x296,0xfea8,0x112,0xffdc,0xffe0,0x56,0xffad,0x4a,0xffc8,0x29,0xffe0,0x17,0xffef,0xb,0xfff8,0x5, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define SECAM_K_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_SECAM_K = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	6400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xfff4,0xff78,0x2b,0x240,0x296,0xfea8,0x112,0xffdc,0xffe0,0x56,0xffad,0x4a,0xffc8,0x29,0xffe0,0x17,0xffef,0xb,0xfff8,0x5, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define SECAM_K1_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_SECAM_K1 = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	6400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xfff4,0xff78,0x2b,0x240,0x296,0xfea8,0x112,0xffdc,0xffe0,0x56,0xffad,0x4a,0xffc8,0x29,0xffe0,0x17,0xffef,0xb,0xfff8,0x5, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define SECAM_L_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_SECAM_L = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	5555000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0x9,0x17,0x7a9,0x8b,0x3f6,0x78a,0x4b,0x7f8,0x7f0,0x20,0x7e1,0x1d,0x7e2,0x20,0x7e1,0x18,0x7f2,0x8,0x7f9,0xa, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define SECAM_L1_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_SECAM_L1 = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	5555000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	1, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0x9,0x17,0x7a9,0x8b,0x3f6,0x78a,0x4b,0x7f8,0x7f0,0x20,0x7e1,0x1d,0x7e2,0x20,0x7e1,0x18,0x7f2,0x8,0x7f9,0xa, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define PAL_NC_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_PAL_NC = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	4400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xfffe,0x2d,0xffa7,0xd9,0x2a2,0x13b,0xff12,0xb2,0xffb0,0x11,0xd,0xfff5,0xfff9,0x11,0xfff1,0x5,0x3,0xfff9,0x4,0xffff, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
	 { LX_ADEMOD_PARM_VID_GAIN, 	15750, },
};

#define NTSC_50_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_NTSC_50 = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	4400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	45750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xfffe,0x2d,0xffa7,0xd9,0x2a2,0x13b,0xff12,0xb2,0xffb0,0x11,0xd,0xfff5,0xfff9,0x11,0xfff1,0x5,0x3,0xfff9,0x4,0xffff, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
	 { LX_ADEMOD_PARM_VID_GAIN, 	15750, },
};

#define PAL_G_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_PAL_G = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	4400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xffee,0xffc2,0xff9f,0x29a,0x21f,0xff72,0x4,0xe7,0xfefe,0xee,0xff66,0x59,0xffdd,0xd,0xfffb,0x9,0xffef,0x13,0xfff0,0xb, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define PAL_K_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_PAL_K = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	4400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xfff4,0xff78,0x2b,0x240,0x296,0xfea8,0x112,0xffdc,0xffe0,0x56,0xffad,0x4a,0xffc8,0x29,0xffe0,0x17,0xffef,0xb,0xfff8,0x5, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define PAL_DK_NICAM_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_PAL_DK_NICAM = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	4400000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	46750000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_GRP_DEL, 	0xfff4,0xff78,0x2b,0x240,0x296,0xfea8,0x112,0xffdc,0xffe0,0x56,0xffad,0x4a,0xffc8,0x29,0xffe0,0x17,0xffef,0xb,0xfff8,0x5, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define LOIF_8MHZ_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_LowIF_8MHz = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	8185000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	36167000, },
	 { LX_ADEMOD_PARM_VIDCR_OUT_CF, 	9000000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define LOIF_7MHZ_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_LowIF_7MHz = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	7000000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	36167000, },
	 { LX_ADEMOD_PARM_VIDCR_OUT_CF, 	9000000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define LOIF_6MHZ_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_LowIF_6MHz = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	6000000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	36167000, },
	 { LX_ADEMOD_PARM_VIDCR_OUT_CF, 	9000000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define LOIF_DVBC_8MHZ_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_LowIF_DVBC_8MHz = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	8185000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	36125000, },
	 { LX_ADEMOD_PARM_VIDCR_OUT_CF, 	9000000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define LOIF_DVBC_7MHZ_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_LowIF_DVBC_7MHz = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	7000000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	36125000, },
	 { LX_ADEMOD_PARM_VIDCR_OUT_CF, 	9000000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

#define LOIF_DVBC_6MHZ_USER
static const LX_ADEMOD_PRESET_TABLE ADEMOD_UserSettings_LowIF_DVBC_6MHz = {
	 { LX_ADEMOD_PARM_FACTORY, 	0, },
	 { LX_ADEMOD_PARM_VID_BNDW, 	6000000, },
	 { LX_ADEMOD_PARM_VID_FRQ, 	36125000, },
	 { LX_ADEMOD_PARM_VIDCR_OUT_CF, 	9000000, },
	 { LX_ADEMOD_PARM_AUD_MODE, 	6, },
	 { LX_ADEMOD_PARM_SPCTR_INV, 	0, },
	 { LX_ADEMOD_PARM_CONFIG_OPTIONS, 	544768, },
	 { LX_ADEMOD_PARM_AGC_OPTIONS, 	72148, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_THR, 	0,0,0, },
	 { LX_ADEMOD_PARM_DIGAGC_NBND_OFFS, 	0,0,0,0, },
};

