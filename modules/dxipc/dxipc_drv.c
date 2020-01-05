/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2013 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */


/** @file ipc_drv.c
 *
 *  Brief description.
 *  Detailed description starts here.
 *
 *  @author	kibeom.kim (kibeom.kim@lge.com)
 *  @version	0.9
 *  @date		2013-05-10
 *  @note		Additional information.
 */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/
//#define TEST_HEVC
#define SYSTEM_BOARD

#ifdef TEST_HEVC
//#define SDEC_PID		0x0091
#define SDEC_PID		0x0102 //Home_Choice_HEVC_4k_sample_CA_AV_3840x2160_30hz_33Mbps_fast.trp
//#define SDEC_PID		0x0123 //Home_Choice_HEVC_Test_xx.ts
//#define SDEC_PID		0x01A0 //UHD_?�험방송_QAM_20130628.trp
//#define SDEC_PID		0x0041
#else
#define SDEC_PID		0x1011
#endif

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include <linux/workqueue.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/file.h>
#include <linux/fs.h>

#include "hma_alloc.h"

#include "vdec_type_defs.h"
#include "include/vdec_kapi.h"
#include "log_kmsg.h"

#include "dxipc_drv.h"

#include "../sdec/sdec_api.h"
#include "sc_if.h"
#include "suk.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define DX_IPC_BUFFER_NUM		64

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/
typedef struct {
	UINT16 address;
	UINT32 data;
} DX_IPC_QUEUE_T;

typedef enum {
	IPC_STATUS_NORMAL,
	IPC_STATUS_RESET,
} DX_IPC_STATUS_T;

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
extern void *vdec_open(void *filp);
extern int vdec_ioctl(void *vdec, unsigned int cmd, unsigned long arg, int from_user);
extern void VDISP_SetMatchMode(UINT8 u8VdispCh, int eMatchMode);
extern int vdec_close(void *vdec);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------
	global Variables
----------------------------------------------------------------------------------------*/
logm_define(dx_ipc, log_level_noti);

/*----------------------------------------------------------------------------------------
	Static Function Prototypes Declarations
----------------------------------------------------------------------------------------*/
static void DX_IPC_Queue_Init(void);
static void DX_IPC_Queue_Process(struct work_struct *data);
static SINT32 DX_IPC_Get_Queue(DX_IPC_QUEUE_T* ipc);
static SINT32 DX_IPC_Put_Queue(DX_IPC_QUEUE_T ipc);

static DECLARE_WORK( _DX_IPC_work, DX_IPC_Queue_Process );
static DEFINE_MUTEX (dx_ipc_lock);

static void DX_IPC_Detected_workfunc(DX_IPC_QUEUE_T ipc);
static irqreturn_t DX_IPC_Interrupt(int irq, void *dev_id);
static void DX_IPC_Isr(void);


/*----------------------------------------------------------------------------------------
 *   Static Variables
 *---------------------------------------------------------------------------------------*/
struct workqueue_struct *_DX_IPC_workqueue;
static void *g_vdec;

static DX_IPC_QUEUE_T DX_IPC_BUF[DX_IPC_BUFFER_NUM];
static UINT32 DX_IPC_PUT_INDEX = 0, DX_IPC_GET_INDEX = 0, DX_IPC_STATUS = 0;

static struct
{
    unsigned int f;
    LX_VDEC_CODEC_T c;
} fs[] =
{
#define F(a,b,c,d) (((a)<<0)|((b)<<8)|((c)<<16)|((d)<<24))
    { F('m','p','g','2'), LX_VDEC_CODEC_MPEG2_HP },
    { F('m','p','g','4'), LX_VDEC_CODEC_MPEG4_ASP},
    { F('v','c','1','e'), LX_VDEC_CODEC_VC1_ES   },
    { F('v','c','r','1'), LX_VDEC_CODEC_VC1_RCV_V1},
    { F('v','c','r','2'), LX_VDEC_CODEC_VC1_RCV_V2},
    { F('d','i','v','3'), LX_VDEC_CODEC_DIVX3    },
    { F('d','i','v','4'), LX_VDEC_CODEC_DIVX4    },
    { F('d','i','v','5'), LX_VDEC_CODEC_DIVX5    },
    { F('v','p','8','0'), LX_VDEC_CODEC_VP8      },
    { F('r','m','v','b'), LX_VDEC_CODEC_RVX      },
    { F('a','v','s','c'), LX_VDEC_CODEC_AVS      },
};

/*========================================================================================
	Implementation Group
========================================================================================*/

static void DX_IPC_Queue_Init(void)
{
	/* initialize command queue */
	DX_IPC_PUT_INDEX = DX_IPC_GET_INDEX = 0;
	DX_IPC_STATUS = IPC_STATUS_NORMAL;
}

static void DX_IPC_Queue_Process(struct work_struct *data)
{
	DX_IPC_QUEUE_T newQueue;

	while( DX_IPC_Get_Queue(&newQueue))
	{
//		printk ("A:%x D:%x\n",newQueue.address,newQueue.data);
		DX_IPC_Detected_workfunc(newQueue);
	}

	DX_IPC_HAL_SetReg (DX_REG_IPC1_BUSY, 0);
}

static SINT32 DX_IPC_Get_Queue(DX_IPC_QUEUE_T* ipc)
{
	SINT32 res;

	/* check number of items left in queue */
	if (DX_IPC_PUT_INDEX == DX_IPC_GET_INDEX)
	{
		res = FALSE;
	}
	else
	{
		/* get item from queue */
		*ipc = DX_IPC_BUF[DX_IPC_GET_INDEX];
		DX_IPC_GET_INDEX = ( DX_IPC_GET_INDEX + 1 ) % DX_IPC_BUFFER_NUM;

		res = TRUE;
	}

	return res;
}

static SINT32 DX_IPC_Put_Queue(DX_IPC_QUEUE_T ipc)
{
	SINT32 num;
	SINT32 res;

	/* check number of items left in queue */
	num = (DX_IPC_PUT_INDEX - DX_IPC_GET_INDEX + DX_IPC_BUFFER_NUM) % DX_IPC_BUFFER_NUM;

	if (num > (DX_IPC_BUFFER_NUM - 2))
	{
		res = FALSE;
	}
	else
	{
		/* put item to queue */
		DX_IPC_BUF[DX_IPC_PUT_INDEX] = ipc;
		DX_IPC_PUT_INDEX = (DX_IPC_PUT_INDEX + 1) % DX_IPC_BUFFER_NUM;

		res = TRUE;
	}

	return res;
}

static int DX_IPC_HAL_Init(void)
{
	int regValue = 0;

	regValue = GetReg(AD_M1_INTR_ENABLE);
	regValue |= IPC_HW_INT_EN; //PDEC interrupt
	SetReg(AD_M1_INTR_ENABLE, regValue);

	//logm_info(dx_ipc, "IPC Interrupt enable 0x%x \n", regValue);

	SetReg(AD_M1_IPC_INTR_EN, 0xFFFF); //MCU1 IPC INTR Enable

	DX_IPC_HAL_SetReg (DX_REG_IPC1_BUSY, 0);

	return 0;
}

static int DX_IPC_HDMI_Set(UINT32 count)
{
	int regAddr = 0, regData = 0, i = 0;

	logm_info(dx_ipc, "DX_IPC_HDMI_Set count %d \r\n", count);

	for(i = 0; i < count ; i++)
	{
		regAddr = GetReg(DX_REG_HDMI_BASE + (i*8));
		regData = GetReg(DX_REG_HDMI_BASE + (i*8 + 4));
		SetReg(regAddr, regData);
	}

	DX_IPC_HAL_SetReg(DX_REG_HDMI_SET, 0);

	return 0;
}

#ifndef SYSTEM_BOARD
static int DX_IPC_SDEC_Init(UINT16 ui16Pid)
{
	*((volatile UINT32*)(0xf0020490)) = 0x0;
	*((volatile UINT32*)(0xf0020494)) = 0x80001044;
	*((volatile UINT32*)(0xf0020494)) |= (ui16Pid & 0x3FFF) << 16;
	return 0;
}
#endif

static int speed;
static LX_VDEC_PIC_SCAN_T scan_mode;
static LX_VDEC_CODEC_T codec;
static LX_VDEC_SRC_T src;
static int use_stc;
static LX_VDEC_BASETIME_T basetime;
static LX_VDEC_IO_UPDATE_BUFFER_T buffer;
static int running;
static int display_offset;
static int framerate_num, framerate_den;
static int request_picture;
static int uhd_seamless;

static int DX_IPC_VDEC_Init(void)
{
	DX_IPC_HAL_SetReg(DX_REG_CPB_INFO, VDEC_CPB_MARGIN);
	DX_IPC_HAL_SetReg(DX_REG_DECODER_STATUS, 0);

	scan_mode = LX_VDEC_PIC_SCAN_ALL;
	codec = LX_VDEC_CODEC_H264_HP;
	src = LX_VDEC_SRC_SDEC0;

#ifndef SYSTEM_BOARD
	{

		g_vdec = vdec_open (NULL);

		logm_info (dx_ipc, "DX_REG_CODEC_CTRL start channel\n");

		if (vdec_ioctl (g_vdec, LX_VDEC_SET_INPUT_DEVICE, LX_VDEC_SRC_SDEC0, 0) < 0)
		{
			logm_error (dx_ipc, "LX_VDEC_SET_INPUT_DEVICE failed\n");
			return -1;
		}
		if (vdec_ioctl (g_vdec, LX_VDEC_SET_OUTPUT_DEVICE, LX_VDEC_DST_DE0, 0) < 0)
		{
			logm_error (dx_ipc, "LX_VDEC_SET_OUTPUT_DEVICE failed\n");
			return -1;
		}
		if (vdec_ioctl (g_vdec, LX_VDEC_USE_GSTC, 1, 0) < 0)
		{
			logm_error (dx_ipc, "LX_VDEC_USE_GSTC failed\n");
			return -1;
		}
		if (vdec_ioctl (g_vdec, LX_VDEC_SET_DISPLAY_OFFSET, 150, 0) < 0)
		{
			logm_error (dx_ipc, "LX_VDEC_SET_DISPLAY_OFFSET failed\n");
			return -1;
		}
		if (vdec_ioctl (g_vdec, LX_VDEC_SET_CODEC,
#ifdef TEST_HEVC
					LX_VDEC_CODEC_HEVC,
#else
					LX_VDEC_CODEC_H264_HP,
#endif
					0) < 0)
		{
			logm_error (dx_ipc, "LX_VDEC_SET_CODEC failed\n");
			return -1;
		}

		if (vdec_ioctl (g_vdec, LX_VDEC_INIT, 0, 0) < 0)
		{
			logm_error (dx_ipc, "LX_VDEC_INIT failed\n");
			return -1;
		}
	}
#endif

	logm_info (dx_ipc, "video decoder started...\n");
	return 0;
}

static void DX_IPC_Detected_workfunc(DX_IPC_QUEUE_T ipc)
{
	int recieveData = 0;
	int regData = 0;
    int i;
	LX_VDEC_STC_T stc;
	extern int vdec_force_framerate_num;
	extern int vdec_force_framerate_den;

	mutex_lock (&dx_ipc_lock);

	recieveData	= ipc.address;
	regData		= ipc.data;

#ifndef SYSTEM_BOARD
	logm_info(dx_ipc, " IPC reg 0x%x, data 0x%x \r\n", recieveData, regData);
#else
	switch(recieveData)
	{
	case DX_REG_OPEN:
		if (src == LX_VDEC_SRC_BUFFTVP)
			suk_stop ();
		if (g_vdec)
		{
			logm_info (dx_ipc, "close...\n");
			vdec_close (g_vdec);
		}

		logm_info (dx_ipc, "open...\n");
		g_vdec = vdec_open (NULL);

		/* initialize initial default values */
		running = 0;
		src = LX_VDEC_SRC_SDEC0;
		vdec_force_framerate_num = vdec_force_framerate_den = 0;
		framerate_num = framerate_den = 0;
		request_picture = 0;
		speed = 0x10000;
		use_stc = 0;
		uhd_seamless = 0;
		display_offset = INT_MAX;

		/* set default IPC registers */
		DX_IPC_HAL_SetReg(DX_REG_DECODER_STATUS, 0);
		DX_IPC_HAL_SetReg(DX_REG_RESOLUTION, 0);
		DX_IPC_HAL_SetReg(DX_REG_FRAME_RATE, 0);
		DX_IPC_HAL_SetReg(DX_REG_3D_TYPE, DX_TYPE_3D_NONE);
		DX_IPC_HAL_SetReg(DX_REG_PAR, 0);
		DX_IPC_HAL_SetReg(DX_REG_PTS, 0);
		DX_IPC_HAL_SetReg(DX_REG_STC, 0);
		DX_IPC_HAL_SetReg(DX_REG_VIDEO_SEQ, 0);
		DX_IPC_HAL_SetReg(DX_REG_CPB_INFO, 0);
		DX_IPC_HAL_SetReg(DX_REG_DUALOUT_MODE, 0);
		DX_IPC_HAL_SetReg(DX_REG_VCP_NOTI, 0);
		DX_IPC_HAL_SetReg(DX_REG_START_ERROR, 0);
		DX_IPC_HAL_SetReg(DX_REG_DECODING_ERROR, 0);
		DX_IPC_HAL_SetReg(DX_REG_HEVC_ERROR, 0);
		DX_IPC_HAL_SetReg(DX_REG_WAVE_ERROR, 0);
		break;

	case DX_REG_CLOSE:
		logm_info (dx_ipc, "close...\n");
		if (src == LX_VDEC_SRC_BUFFTVP)
			suk_stop ();
		if (g_vdec)
			vdec_close (g_vdec);
		g_vdec = NULL;
		running = 0;
		break;

	case DX_REG_CODEC_START:
		logm_info (dx_ipc, "DX_REG_CODEC_START start channel\n");

		if (vdec_ioctl (g_vdec, LX_VDEC_SET_CODEC, codec, 0) < 0)
			logm_warning (dx_ipc, "LX_VDEC_SET_CODEC failed\n");

		if (vdec_ioctl (g_vdec, LX_VDEC_SET_INPUT_DEVICE, src, 0) < 0)
			logm_warning (dx_ipc, "LX_VDEC_SET_INPUT_DEVICE failed\n");

		if (vdec_ioctl (g_vdec, LX_VDEC_SET_OUTPUT_DEVICE, LX_VDEC_DST_DE0, 0) < 0)
			logm_warning (dx_ipc, "LX_VDEC_SET_OUTPUT_DEVICE failed\n");

		if (use_stc)
		{
			logm_noti (dx_ipc, "use STC\n");
			if (vdec_ioctl (g_vdec, LX_VDEC_USE_GSTC, 0, 0) < 0)
				logm_warning (dx_ipc, "LX_VDEC_USE_GSTC failed\n");

			SDEC_API_SetPcrRecovery(0, TRUE);

			if (display_offset == INT_MAX)
				display_offset = 150;
		}
		else
		{
			logm_noti (dx_ipc, "use GSTC\n");
			if (vdec_ioctl (g_vdec, LX_VDEC_USE_GSTC, 1, 0) < 0)
				logm_warning (dx_ipc, "LX_VDEC_USE_GSTC failed\n");

			SDEC_API_SetPcrRecovery(0, FALSE);

			if (display_offset == INT_MAX)
				display_offset = 700;
		}
		logm_noti (dx_ipc, "display offset %dms\n", display_offset);

		if (uhd_seamless)
		{
			logm_debug (dx_ipc, "add uhd_seamless flag\n");
			vdec_ioctl (g_vdec, LX_VDEC_ADD_FLAGS,
					lx_vdec_flags_uhd_seemless, 0);
		}

		if (vdec_ioctl (g_vdec, LX_VDEC_INIT, 0, 0) < 0)
		{
			logm_error (dx_ipc, "LX_VDEC_INIT failed\n");
			return;
		}

		if (vdec_ioctl (g_vdec, LX_VDEC_SET_SPEED,
					(unsigned long)speed, 0) < 0)
			logm_warning (dx_ipc, "LX_VDEC_SET_SPEED failed\n");

		if (vdec_ioctl (g_vdec, LX_VDEC_SET_SCAN_PICTURE,
					scan_mode, 0) < 0)
			logm_warning (dx_ipc, "LX_VDEC_SET_SCAN_PICTURE failed\n");

		if (vdec_ioctl (g_vdec, LX_VDEC_SET_BASETIME,
					(unsigned long)&basetime, 0) < 0)
			logm_warning (dx_ipc, "LX_VDEC_SET_BASETIME failed\n");

		if (vdec_ioctl (g_vdec, LX_VDEC_SET_DISPLAY_OFFSET,
					(unsigned long)display_offset, 0) < 0)
			logm_warning (dx_ipc, "LX_VDEC_SET_DISPLAY_OFFSET failed\n");

		if (request_picture > 0)
		{
			/* thumbnail... */
			logm_debug (dx_ipc, "thumbnail...\n");

			vdec_force_framerate_num = 30;
			vdec_force_framerate_den = 1;

			vdec_ioctl (g_vdec, LX_VDEC_SET_REQUEST_PICTURES, (unsigned long)request_picture, 0);

			DX_IPC_HAL_SetReg(DX_REG_DECODER_STATUS, 0);
		}
		else
		{
			if (framerate_num)
			{
				logm_debug (dx_ipc, "set framerate...\n");

				if (vdec_ioctl (g_vdec, LX_VDEC_SET_FRAMERATE,
							(unsigned long)((framerate_num<<16)|(framerate_den&0xffff)), 0) < 0)
					logm_warning (dx_ipc, "LX_VDEC_SET_FRAMERATE failed\n");
			}
		}

		running = 1;

		DX_IPC_HAL_SetReg(DX_REG_CPB_INFO, VDEC_CPB_MARGIN);
		DX_IPC_HAL_SetReg(DX_REG_DECODER_STATUS, 0);

		if (src == LX_VDEC_SRC_BUFFTVP)
		{
			//int num = 128;

			//vdec_ioctl (g_vdec, LX_VDEC_SET_DECODING_QUEUE_SIZE, &num, 0);
			suk_start (g_vdec);
		}

		logm_info(dx_ipc, "DX_REG_CODEC_START %d \n", regData);
		break;

	case DX_REG_UHD_SEEMLESS:
		logm_info (dx_ipc, "uhd seemless %d\n", regData);
		uhd_seamless = !!regData;
		break;

	case DX_REG_CLOCKSOURCE:
		logm_info (dx_ipc, "DX_REG_CLOCKSOURCE %d\n", regData);
		switch (regData)
		{
		default:
			logm_warning (dx_ipc, "wrong data %d\n", regData);
		case 0:
			logm_noti (dx_ipc, "clock source : GSTC\n");
			use_stc = 0;
			break;
		case 1:
			logm_noti (dx_ipc, "clock source : STC\n");
			use_stc = 1;
			break;
		}

		break;

	case DX_REG_LIPSYNC_CTRL:
		logm_info(dx_ipc, "DX_REG_LIPSYNC_CTRL %d\n", regData);
		VDISP_SetMatchMode (0, regData);
		/* TODO: implement here */
		break;

	case DX_REG_BASETIME:	/* DX_REG_BASETIME_PTS */
		basetime.base_pts = regData;
		logm_debug (dx_ipc, "pts set:%08x\n", basetime.base_pts);
		break;

	case DX_REG_BASETIME_STC:
		vdec_ioctl (g_vdec, LX_VDEC_GET_GLOBAL_STC, (unsigned long)&stc, 0);
		basetime.base_stc = stc.stc - regData;

		logm_debug (dx_ipc, "stc now:%08x, before:0x%x, set:%08x\n",
				stc.stc, regData, basetime.base_stc);
		break;

	case DX_REG_DISPLAY_OFFSET:
		display_offset = regData;
		logm_debug (dx_ipc, "display offset %dms\n", display_offset);
		break;

	case DX_REG_REQUEST_PIC:
		request_picture = regData;
		logm_debug (dx_ipc, "request picture %d\n", request_picture);
		DX_IPC_HAL_SetReg(DX_REG_DECODER_STATUS, 0);
		break;

	case DX_REG_CODEC_TYPE:
		if(regData == 0)
		{
			codec = LX_VDEC_CODEC_H264_HP;
			logm_info(dx_ipc, "DX_REG_CODEC_TYPE h264\n");
		}
		else
		{
			codec = LX_VDEC_CODEC_HEVC;
			logm_info(dx_ipc, "DX_REG_CODEC_TYPE hevc\n");
		}
		break;

    case DX_REG_FOURCC_TYPE: /* for d13 g1 test */
        for (i=0; i<sizeof(fs)/sizeof(fs[0]); i++)
        {
            if (regData==fs[i].f)
                break;
        }
        /* do nothing if fourcc not mapped. */
        if (i==sizeof(fs)/sizeof(fs[0]))
            break;
        else
        {
            codec = fs[i].c;
            logm_info(dx_ipc, "DX_REG_FOURCC_TYPE has %c%c%c%c"
                    ,(regData>>0)&0xff,(regData>>8)&0xff
                    ,(regData>>16)&0xff,(regData>>24)&0xff);
        }
        break;

	case DX_REG_MEMORY_FLUSH:
		if (src == LX_VDEC_SRC_BUFFTVP)
			suk_flush ();

		vdec_ioctl (g_vdec, LX_VDEC_IO_FLUSH, 0, 0);
		DX_IPC_HAL_SetReg(DX_REG_CPB_INFO, VDEC_CPB_MARGIN);
		DX_IPC_HAL_SetReg(DX_REG_DECODER_STATUS, 0);
		logm_debug (dx_ipc, "flush...\n");

		running = 0;
		break;

	case DX_REG_SCAN_MODE:
		switch (regData)
		{
		default:
			logm_warning (dx_ipc, "unknown scan mode %d\n", regData);
		case 0: scan_mode = LX_VDEC_PIC_SCAN_ALL; break;
		case 1: scan_mode = LX_VDEC_PIC_SCAN_I; break;
		case 2: scan_mode = LX_VDEC_PIC_SCAN_IP; break;
		}
		logm_debug (dx_ipc, "scan mode %d\n", scan_mode);
		break;

	case DX_REG_RENDERING_SPEED:
		if (running)
		{
			if (!use_stc && regData)
			{
				if (vdec_ioctl (g_vdec, LX_VDEC_SET_BASETIME, (unsigned long)&basetime, 0) < 0)
					logm_warning (dx_ipc, "LX_VDEC_SET_BASETIME failed\n");
			}

			vdec_ioctl (g_vdec, LX_VDEC_SET_SPEED, (unsigned long)regData, 0);
			logm_debug (dx_ipc, "set speed %x\n", regData);
		}
		else
		{
			speed = regData;
			logm_debug (dx_ipc, "speed %d\n", speed);
		}

		break;

	case DX_REG_LOW_LATENCY:
		logm_info(dx_ipc, "DX_REG_LOW_LATENCY \n");
		break;

	case DX_REG_CODEC_FRAMERATE:
		framerate_num = regData>>16;
		framerate_den = regData&0xffff;
		logm_debug (dx_ipc, "set framerate %d/%d\n",
				framerate_num, framerate_den);
		break;

	case DX_REG_VIDEO_SOURCE:
		switch (regData)
		{
		default:
			logm_warning (dx_ipc, "unknown source. %08x\n", regData);
		case A('S','D','E','0'):
			src = LX_VDEC_SRC_SDEC0;
			break;

		case A('M','E','M','O'):	/* ??? wrong spel of ????*/
		case A('M','E','M','0'):
			src = LX_VDEC_SRC_BUFF;
			break;

		case A('T','V','P','0'):
			src = LX_VDEC_SRC_BUFFTVP;
			break;
		}
		logm_info (dx_ipc, "set source %d\n", src);
		break;

	case DX_REG_VIDEO_DECODE_BUFFER_SIZE:
		memset (&buffer, 0, sizeof(buffer));

		buffer.timestamp = -1;
		buffer.au_size = regData;
		buffer.au_ptr = 0;

		logm_debug (dx_ipc, "decode buffer size 0x%08x(%d)\n", regData, regData);
		break;

	case DX_REG_VIDEO_DECODE_BUFFER_ADDR:
		buffer.au_ptr = (unsigned char*)(regData + 0x60000000);

		logm_debug (dx_ipc, "decode buffer 0x%p(%d)\n",
				buffer.au_ptr, buffer.au_size);
		vdec_ioctl (g_vdec, LX_VDEC_IO_UPDATE_BUFFER,
				(unsigned long)&buffer, 0);
		break;

	case DX_REG_VIDEO_DECODE_BUFFER:
		memset (&buffer, 0, sizeof(buffer));

#ifdef CHIP_NAME_d14
		if (regData >= 0x40000000)
		{
			regData -= 0x40000000;
			regData += 0x10000000;
		}
#endif
		regData += 0x60000000;

		buffer.timestamp = -1;
		buffer.au_type = LX_VAU_DATA;
		buffer.au_size = *(int*)(regData&~3);
		buffer.au_ptr = (unsigned char*)(regData+4);

		logm_debug (dx_ipc, "decode buffer 0x%p(%d)\n",
				buffer.au_ptr, buffer.au_size);
		vdec_ioctl (g_vdec, LX_VDEC_IO_UPDATE_BUFFER,
				(unsigned long)&buffer, 0);
		break;

	case DX_REG_MCU1_UART:
		logm_debug (dx_ipc, "control uart interrupt. 0x%x\n", regData);
#ifdef CONFIG_XTENSA_PLATFORM_D13VDEC
		if (regData == 0x0e)
		{
			unsigned int reg;

			/* enable */
			reg = *(unsigned int*)0xf00000e8;
			logm_debug (dx_ipc, "reg, 0x%08x\n", reg);
			reg |= 0x0800;

			*(unsigned int*)0xf00000e8 = reg;
			logm_debug (dx_ipc, "reg, 0x%08x\n", reg);
		}
		else
		{
			unsigned int reg;

			/* disable */
			reg = *(unsigned int*)0xf00000e8;
			logm_debug (dx_ipc, "reg, 0x%08x\n", reg);
			reg &= ~0x0800;

			*(unsigned int*)0xf00000e8 = reg;
			logm_debug (dx_ipc, "reg, 0x%08x\n", reg);
		}
#endif
		break;

	case DX_REG_HDMI_SET:
		logm_info(dx_ipc, "hdmi register set. 0x%x\n", regData);
		DX_IPC_HDMI_Set(regData);
		break;

	case DX_REG_VIDEO_PID:
		logm_info(dx_ipc, "set video 0 pid 0x%x\n", regData);
		SDEC_API_SetVideoPid(0, regData);
		break;

	case DX_REG_AUDIO_PID:
		logm_info(dx_ipc, "set audio pid 0x%x\n", regData);
		SDEC_API_SetAudioPid(0, regData);
		break;

	case DX_REG_PCR_PID:
		logm_info(dx_ipc, "set pcr pid 0x%x\n", regData);
		SDEC_API_SetPcrPid(0, regData);
		break;

	case DX_REG_SC_CMD:
		printk("DX_REG_SC_CMD : %d\n", regData);
		logm_info(dx_ipc, "sc command %d\n", regData);
		if (SC_IF_RunCommand(regData)) {
			logm_info(dx_ipc, "Failed\n");
		}
		break;

	case DX_REG_SET_KEY_ADDR:
		logm_info(dx_ipc, "set key addr 0x%x\n", regData);
		SDEC_API_SetKeyAddr(0, regData);
		break;

	case DX_REG_SET_KEY_DATA:
		logm_info(dx_ipc, "set key data 0x%x\n", regData);
		SDEC_API_SetKeyData(0, regData);
		break;

	case DX_REG_VDEC0_ALLOC1:
		{
			static unsigned long m;

#ifdef CHIP_NAME_d13
#define POOLNAME "vdec"
#else
#define POOLNAME "vdec0"
#endif

			logm_info (dx_ipc, "alloc size 0x%x\n", regData);
			if (m)
			{
				logm_info (dx_ipc, "free previous memory, 0x%lx\n", m);
				hma_free (POOLNAME, m);
			}

			m = hma_alloc_user (POOLNAME, regData, 4096, "dxipc");

			logm_info (dx_ipc, "memory allocated 0x%lx\n", m);
			DX_IPC_HAL_SetReg (DX_REG_VDEC0_ALLOC1, (int)m);
		}
		break;

        case DX_REG_VIDEO_FREEZE:
                logm_info (dx_ipc, "freeze video %d\n", regData);
                if (g_vdec == NULL)
                {
			logm_warning (dx_ipc, "no vdec\n");
			break;
                }

                if (regData)
			vdec_ioctl (g_vdec, LX_VDEC_DISPLAY_FREEZE,
					LX_VDEC_DISPLAY_FREEZE_SET, 0);
		else
			vdec_ioctl (g_vdec, LX_VDEC_DISPLAY_FREEZE,
					LX_VDEC_DISPLAY_FREEZE_UNSET, 0);

                break;

	case DX_REG_VIDEO_STEP:
                logm_info (dx_ipc, "step video %d\n", regData);
                if (g_vdec == NULL)
                {
			logm_warning (dx_ipc, "no vdec\n");
			break;
                }

		vdec_ioctl (g_vdec, LX_VDEC_SET_STEP,
				0, 0);
		break;

	case DX_REG_VIDEO_DISPLAY_SYNC:
                logm_info (dx_ipc, "sync video %d\n", regData);
                if (g_vdec == NULL)
                {
			logm_warning (dx_ipc, "no vdec\n");
			break;
                }

		switch (regData)
		{
		default:
			logm_warning (dx_ipc, "wrong data. 0x%x\n", regData);
			// fallthrough
		case A('F','R','E','E'):
			vdec_ioctl (g_vdec, LX_VDEC_DISPLAY_SYNC,
					LX_VDEC_DISPLAY_SYNC_FREERUN, 0);
			break;
		case A('M','A','T','C'):
			vdec_ioctl (g_vdec, LX_VDEC_DISPLAY_SYNC,
					LX_VDEC_DISPLAY_SYNC_MATCH, 0);
			break;
		}
		break;

	default:
		logm_info(dx_ipc, "not defined command %08x 0x%x\n", recieveData, regData);
		break;
	}
#endif
	mutex_unlock (&dx_ipc_lock);
}

int DX_IPC_Init(void)
{
	int ret = 0;
	int num_be_irq = 0;

	num_be_irq = IPC_INT_NUM;

	printk ("dxipc: firmware version %s\n",
			FIRMWARE_VERSION);

	logm_info(dx_ipc, "DX_IPC_Init\n");
	_DX_IPC_workqueue = create_workqueue("DX_IPC");

	ret = request_irq(num_be_irq,(irq_handler_t)DX_IPC_Interrupt,IRQF_SHARED,"dx ipc", (void*)0xdecdec);

	if (ret)
	{
		logm_error (dx_ipc, "request_irq failed %d\n", ret);
		return ret;
	}

	DX_IPC_HAL_Init();
	DX_IPC_Queue_Init();
#ifndef SYSTEM_BOARD
	DX_IPC_SDEC_Init(SDEC_PID);
#endif
	DX_IPC_VDEC_Init();
	return ret;
}

void DX_IPC_exit(void)
{
	free_irq(IPC_INT_NUM, (void*)0xdecdec);

	return;
}

void DX_IPC_Send_to_MCU0(int regData)
{
	SetReg(AD_M0_IPC_13, regData);
	SetReg(AD_M0_IPC_INTR_STATUS, DX_REG_MCU0_INT_BIT);

	logm_info(dx_ipc, "send IPC to MCU0 : sendData 0x%x\n", regData);
	return;
}

void DX_IPC_Send_to_MCU0_Noti(int regData)
{
	SetReg(AD_M0_IPC_12, regData);
	SetReg(AD_M0_IPC_INTR_STATUS, (1<<12));

	logm_trace(dx_ipc, "send Noti to MCU0 : sendData 0x%x\n", regData);
	return;
}

static irqreturn_t DX_IPC_Interrupt(int irq, void *dev_id)
{
	DX_IPC_Isr();

	return IRQ_HANDLED;
}

static void DX_IPC_Isr(void)
{
	int intrStatus = 0;
	DX_IPC_QUEUE_T ipc;
	int regdata = 0;

	intrStatus = GetReg(AD_M1_IPC_INTR_STATUS);
	//logm_info(dx_ipc, "IPC M1 : status 0x%x\n", intrStatus);

	if(intrStatus & (DX_REG_MCU0_INT_BIT))
	{
		ipc.address	= GetReg(AD_M1_IPC_13);
		ipc.data	= DX_IPC_HAL_GetReg(ipc.address);
		DX_IPC_Put_Queue(ipc);

		queue_work(_DX_IPC_workqueue,  &_DX_IPC_work);
	}

	if(intrStatus & (0x10))
	{
		regdata	= DX_IPC_HAL_GetReg(DX_REG_HDMI_SET);
		DX_IPC_HDMI_Set(regdata);
	}

	// clear interrupt
	if(intrStatus) SetReg(AD_M1_IPC_INTR_CLR, intrStatus);
}

module_init (DX_IPC_Init);
module_exit (DX_IPC_exit);

MODULE_AUTHOR("LGE");
MODULE_DESCRIPTION("LG IPC driver for host and decoder chip");
MODULE_LICENSE("GPL");
