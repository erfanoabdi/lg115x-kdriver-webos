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


/** @file ipc_drv.h
 *
 *  Brief description.
 *  Detailed description starts here.
 *
 *  @author	kibeom.kim (kibeom.kim@lge.com)
 *  @version	0.9
 *  @date		2013-05-10
 *  @note		Additional information.
 */

#ifndef _INT_DRV_H_
#define _INT_DRV_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "uvd_reg.h"

#ifdef CHIP_NAME_d13
#include "../vdec/hal/d13/mcu_base.h"
#include "../vdec/hal/d13/vdec_base.h"
#endif

#ifdef CHIP_NAME_d14
#include "../vdec/hal/d14/mcu_base.h"
#include "../vdec/hal/d14/vdec_base.h"
#endif

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/
#define DX_IPC_HAL_SetReg(addr,data)	(*((volatile int *)(DX_REG_BASE_ADDR + (addr * 4)))) = (data)
#define DX_IPC_HAL_GetReg(addr)		(*((volatile int *)(DX_REG_BASE_ADDR + (addr * 4))))

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
int DX_IPC_Init(void);
void DX_IPC_Send_to_MCU0(int regData);
void DX_IPC_Send_to_MCU0_Noti(int regData);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#endif /* _INT_DRV_H_ */

