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

#ifndef __UVD_REG_Dx_H__
#define __UVD_REG_Dx_H__

#include "include/dxipc_ipc_numbers.h"


#define D13_REG_HDMI_BASE			0x7FF80000
#define D13_REG_BASE_ADDR			0x7FF00000
#define D13_REG_MCU0_IPC_INT		0xF0000034
#define D13_REG_MCU0_IPC14			0xF0000078

#define D14_REG_HDMI_BASE			0x7FF80000
#define D14_REG_BASE_ADDR			0xE0005000
#define D14_REG_MCU0_IPC_INT		0xB0080034
#define D14_REG_MCU0_IPC14			0xB0080078



#ifdef CHIP_NAME_d13
#define DX_REG_HDMI_BASE		D13_REG_HDMI_BASE
#define DX_REG_BASE_ADDR		D13_REG_BASE_ADDR
#define DX_REG_MCU0_IPC_INT		D13_REG_MCU0_IPC_INT
#define DX_REG_MCU0_IPC14		D13_REG_MCU0_IPC14
#define DX_REG_MCU0_INT_BIT		1
#endif

#ifdef CHIP_NAME_d14
#define DX_REG_HDMI_BASE		D14_REG_HDMI_BASE
#define DX_REG_BASE_ADDR		D14_REG_BASE_ADDR
#define DX_REG_MCU0_IPC_INT		D14_REG_MCU0_IPC_INT
#define DX_REG_MCU0_IPC14		D14_REG_MCU0_IPC14
#define DX_REG_MCU0_INT_BIT		1<<13
#endif

#ifndef DX_REG_BASE_ADDR
#error "unknown chip..."
#endif

#define A(a,b,c,d)			((a)<<0|(b)<<8|(c)<<16|(d)<<24)

#endif

/* vim:set ts=4 sw=4: */
