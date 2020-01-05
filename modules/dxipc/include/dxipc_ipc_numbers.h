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

#ifndef __DXIPC_IPC_NUMBERS_H__
#define __DXIPC_IPC_NUMBERS_H__



/* parameter of DX_REG_3D_TYPE */
typedef enum
{
	DX_TYPE_3D_FPA_CHECKERBOARD,
	DX_TYPE_3D_FPA_COLUMN_ALTERNATION,
	DX_TYPE_3D_FPA_ROW_ALTERNATION,
	DX_TYPE_3D_FPA_SIDE_BY_SIDE,
	DX_TYPE_3D_FPA_TOP_BOTTOM,
	DX_TYPE_3D_FPA_FRAME_ALTERNATION,

	DX_TYPE_3D_NONE,
} DX_TYPE_3D;


#define DX_REG_FW_VERSION			0x0000
#define DX_REG_HEVC_VERSION			0x0001
#define DX_REG_WAVE_VERSION			0x0002

#define DX_REG_SW_RESET				0x0010
#define DX_REG_REGISTER_WRITE		0x0011
#define DX_REG_REGISTER_READ		0x0012
#define DX_REG_SYSTEM_STATUS		0x0013
#define DX_REG_INTERRUPT_STATUS		0x0014
#define DX_REG_INTERRUPT_CLEAR		0x0015
#define DX_REG_MCU0_UART			0x0016
#define DX_REG_MCU1_UART			0x0017

#define DX_REG_GPIO_DIR				0x0020
#define DX_REG_GPIO_DATA			0x0021
#define DX_REG_FLASH_DOWNLOAD		0x0022
#define DX_REG_FLASH_DOWN_STATUS	0x0023

#define DX_REG_VIDEO_DECODE_BUFFER	0x0030
#define DX_REG_VIDEO_SOURCE			0x0031
#define DX_REG_VIDEO_DECODE_BUFFER_SIZE		0x0032
#define DX_REG_VIDEO_DECODE_BUFFER_ADDR		0x0033

#define DX_REG_CODEC_CTRL			0x0040		/* no effect... deprecated */
#define DX_REG_LIPSYNC_CTRL			0x0041
#define DX_REG_BASETIME				0x0042
#define DX_REG_BASETIME_PTS			DX_REG_BASETIME
#define DX_REG_CODEC_TYPE			0x0043
#define DX_REG_MEMORY_FLUSH			0x0044
#define DX_REG_SCAN_MODE			0x0045
#define DX_REG_RENDERING_SPEED		0x0046
#define DX_REG_LOW_LATENCY			0x0047
#define DX_REG_BASETIME_STC			0x0048
#define DX_REG_REQUEST_PIC			0x0049
#define DX_REG_IPC1_BUSY			0x004a
#define DX_REG_CODEC_START			0x004b
#define DX_REG_CODEC_FRAMERATE		0x004c
#define DX_REG_CLOCKSOURCE			0x004d
#define DX_REG_VIDEO_FREEZE         0x004e      /* 0: unfreeze, nonzero: freeze video output */
#define DX_REG_VIDEO_STEP			0x004f
#define DX_REG_VIDEO_DISPLAY_SYNC	0x0050

#define DX_REG_OPEN					0x005E
#define DX_REG_CLOSE				0x005F

#define DX_REG_AUDIO_CODEC_TYPE		0x0060
#define DX_REG_AUDIO_SAMPLE_FREQ	0x0061
#define DX_REG_AUDIO_CODEC_CTRL		0x0062
#define DX_REG_AUDIO_BUFFER_FLUSH	0x0063

#define DX_REG_VCP_PATTERN			0x0070
#define DX_REG_HDMI_PATTERN			0x0071
#define DX_REG_DISPLAY_OFFSET		0x0072
#define DX_REG_VIDEO_PID			0x0074
#define DX_REG_AUDIO_PID			0x0075
#define DX_REG_PCR_PID				0x0076
#define DX_REG_HDMI_OUTMODE			0x0077
#define DX_REG_UHD_SEEMLESS			0x0078

#define DX_REG_DECODER_STATUS		0x0080
#define DX_REG_RESOLUTION			0x0081
#define DX_REG_FRAME_RATE			0x0082
#define DX_REG_3D_TYPE				0x0083
#define DX_REG_PAR					0x0084
#define DX_REG_PTS					0x0085
#define DX_REG_STC					0x0086
#define DX_REG_VIDEO_SEQ			0x0087
#define DX_REG_CPB_INFO				0x0088
#define DX_REG_DUALOUT_MODE			0x0089		// 31~16 : 0 [e/o] , 1 [L/R] 15~0 : 0 [ 8bit], 1 [10 bit]
#define DX_REG_VCP_NOTI				0x008F

#define DX_REG_START_ERROR			0x0090		/* 0: no error, non-0: error */
#define DX_REG_DECODING_ERROR		0x0091
#define DX_REG_HEVC_ERROR			0x0092
#define DX_REG_WAVE_ERROR			0x0093

#define DX_REG_HDMI_SET				0x0098
#define DX_REG_IPC0_STATUS			0x0099

#define DX_REG_SC_CMD				0x00a0

#define DX_REG_VDEC0_ALLOC1			0x00a8

#define DX_REG_SET_KEY_ADDR			0x00b0
#define DX_REG_SET_KEY_DATA			0x00b1

#define DX_REG_FOURCC_TYPE          0x00c0


#endif

/* vim:set ts=4 sw=4: */
