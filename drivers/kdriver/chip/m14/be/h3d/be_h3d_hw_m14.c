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


/** @file
 *
 *  drv file for BE device
 *
 *  author		justine.jeong
 *  version		1.0
 *  date		2012.02.07
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

/*----------------------------------------------------------------------------------------
  Control Constants
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  File Inclusions
  ----------------------------------------------------------------------------------------*/
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/delay.h>

#include "os_util.h"
#include "be_dbg.h"

#include "be_h3d_hw_m14.h"
#include "be_def.h"
#include "../fwi/ipc_def_m14.h"
#include "ipc_drv.h"

#include "ipc_cmd_header.h"

// For Status Management
#include "../../../core/be/video_status_keeper.h"

/*----------------------------------------------------------------------------------------
  Constant Definitions
  ----------------------------------------------------------------------------------------*/
//#define USE_BE_IOCTL_MACROS
#define CONV_ENABLE 1
#define CONV_DISABLE 0

/*----------------------------------------------------------------------------------------
  Macro Definitions
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Type Definitions
  ----------------------------------------------------------------------------------------*/
/**
 *  main control block for BE device.
 *  each minor device has unique control block
 *
 */

/*----------------------------------------------------------------------------------------
  External Function Prototype Declarations
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  External Variables
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  global Variables
  ----------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------
  Static Function Prototypes Declarations
  ----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
  Static Variables
  ----------------------------------------------------------------------------------------*/

/**
 * main control block for gfx device
 */


/**
 * file I/O description for gfx device
 */


/*========================================================================================
  Implementation Group
  ========================================================================================*/

int BE_H3D_HW_M14_Get_Version(LX_BE_H3D_VERSION_T *pstParams)
{
	int ret = RET_OK;

	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_GET_PARAM(BE_IPC_H3D_VERSION_T, stHwParams);

			/* Send command  to MCU via IPC & Check result */
			ret = BE_KIPC_SetData(BE_H3D_GET_VERSION, &stHwParams, sizeof(BE_IPC_H3D_VERSION_T));
			if(ret) BREAK_WRONG(ret);
			ret = BE_KIPC_GetData(&stHwParams, sizeof(BE_IPC_H3D_VERSION_T));
			if(ret) BREAK_WRONG(ret);

			/* change value BE_IPC to BE_IPCLX_BE */
			pstParams->u16A3CVersion = stHwParams.u8A3CVersion;
			pstParams->u16T3CVersion = stHwParams.u8T3CVersion;
		}
	} while(0);
	return ret;
}

int BE_H3D_HW_M14_Reset_Block(LX_BE_H3D_RESET_BLOCK_T *pstParams)
{
	int ret = RET_OK;

	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_H3D_RESET_BLOCK_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.bReset = pstParams->bReset;

			/* return unexpected value */
			if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}	

			ret = BE_KIPC_SetData(BE_H3D_RESET_BLOCK, &stHwParams, sizeof(BE_IPC_H3D_RESET_BLOCK_T));
			if(ret) BREAK_WRONG(ret);
		}
	} while(0);
	return ret;
}

int BE_H3D_HW_M14_Bypass_All(LX_BE_H3D_BYPASS_ALL_T *pstParams)
{
	int ret = RET_OK;

	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_H3D_BYPASS_ALL_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.bBypass = pstParams->bBypass;

			/* return unexpected value */
			if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}	

			ret = BE_KIPC_SetData(BE_H3D_BYPASS_ALL, &stHwParams, sizeof(BE_IPC_H3D_BYPASS_ALL_T));
			if(ret) BREAK_WRONG(ret);
		}
	} while(0);
	return ret;
}

int BE_H3D_HW_M14_Set_Mode(LX_BE_H3D_MODE_T *pstParams)
{
	int ret = RET_OK;
	unsigned int status = 0;

	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_H3D_MODE_T, stHwParams);

			if ( pstParams->bA3CMode >1 || pstParams->bT3CMode > 1 )
			{
				BE_ERROR("Invalied params[1: on, 0: off] bA3CMode = %d, bT3CMode = %d\n", pstParams->bA3CMode, pstParams->bT3CMode);
				return RET_INVALID_PARAMS; /* out of range (1: on, 0: off) */
			}

			stHwParams.uA3CMode = pstParams->bA3CMode == 0xFFFF ? 0xFFFFFFFF : pstParams->bA3CMode;
			stHwParams.uT3CMode = pstParams->bT3CMode == 0xFFFF ? 0xFFFFFFFF : pstParams->bT3CMode;	

			/* For Status Management */
			status = (0xFFFF0000&(stHwParams.uA3CMode<<16))|(0x0000FFFF&(stHwParams.uT3CMode));
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_3D_MODE, status);


			ret = BE_KIPC_SetData(H3D_PACK_CMD(BE_H3D_B0_SET_MODE), &stHwParams, sizeof(BE_IPC_B0_H3D_MODE_T));
			if(ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_H3D_MODE_T, stHwParams);

			if ( pstParams->bA3CMode >1 || pstParams->bT3CMode > 1 )
			{
				BE_ERROR("Invalied params[1: on, 0: off] bA3CMode = %d, bT3CMode = %d\n", pstParams->bA3CMode, pstParams->bT3CMode);
				return RET_INVALID_PARAMS; /* out of range (1: on, 0: off) */
			}

			stHwParams.bA3CMode = pstParams->bA3CMode;
			stHwParams.bT3CMode = pstParams->bT3CMode;

			/* return unexpected value */
			if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}	

			/* For Status Management */
			status = (0xFFFF0000&(stHwParams.bA3CMode<<16))|(0x0000FFFF&(stHwParams.bT3CMode));
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_3D_MODE, status);

			ret = BE_KIPC_SetData(BE_H3D_SET_MODE, &stHwParams, sizeof(BE_IPC_H3D_MODE_T));
			if(ret) BREAK_WRONG(ret);
		}
	} while(0);
	return ret;
}

int BE_H3D_HW_M14_Get_Mode(LX_BE_H3D_MODE_T *pstParams)
{
	int ret = RET_OK;

	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_GET_PARAM(BE_IPC_H3D_MODE_T, stHwParams);

			ret = BE_KIPC_SetData(BE_H3D_SET_MODE, &stHwParams, sizeof(BE_IPC_H3D_MODE_T));
			if(ret) BREAK_WRONG(ret);
			ret = BE_KIPC_GetData((void *)&stHwParams, sizeof(BE_IPC_H3D_MODE_T));
			if(ret) BREAK_WRONG(ret);

			pstParams->bA3CMode = stHwParams.bA3CMode;
			pstParams->bT3CMode = stHwParams.bT3CMode;
		}
	} while(0);
	return ret;
}

int BE_H3D_HW_M14_In_Format(LX_BE_H3D_IN_FORMAT_T *pstParams)
{
	int ret = RET_OK;

	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			/* Do Nothing */
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_H3D_IN_FORMAT_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.u4H3DInFormat = pstParams->u32H3DInFormat;

			/* return unexpected value */
			if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}	

			ret = BE_KIPC_SetData(BE_H3D_IN_FORMAT, &stHwParams, sizeof(BE_IPC_H3D_IN_FORMAT_T));
			if(ret) BREAK_WRONG(ret);
		}
	} while(0);
	return ret;
}

int BE_H3D_HW_M14_T3C_Depth_Level(LX_BE_H3D_T3C_DEPTH_LEVEL_T *pstParams)
{
	int ret = RET_OK;
	unsigned int status = 0;

	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_H3D_T3C_DEPTH_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.uT3CDepth = pstParams->u32T3CDepthLevel;	

			/* For Status Management */
			status = (unsigned int)stHwParams.uT3CDepth;
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_2D_TO_3D_DEPTH, status);

			ret = BE_KIPC_SetData(H3D_PACK_CMD(BE_H3D_B0_T3C_DEPTH_LEVEL), &stHwParams, sizeof(BE_IPC_B0_H3D_T3C_DEPTH_T));
			if(ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_H3D_T3C_DEPTH_LEVEL_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.u8T3CDepthLevel = pstParams->u32T3CDepthLevel;

			/* For Status Management */
			status = (unsigned int)stHwParams.u8T3CDepthLevel;
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_2D_TO_3D_DEPTH, status);

			/* return unexpected value */
			if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}	

			ret = BE_KIPC_SetData(BE_H3D_T3C_DEPTH_LEVEL, &stHwParams, sizeof(BE_IPC_H3D_T3C_DEPTH_LEVEL_T));
			if(ret) BREAK_WRONG(ret);
		}
	} while(0);
	return ret;
}

int BE_H3D_HW_M14_A3C_Depth_Level(LX_BE_H3D_A3C_DEPTH_LEVEL_T *pstParams)
{
	int ret = RET_OK;
	unsigned int status = 0;

	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_H3D_A3C_DEPTH_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.uA3CDepth = pstParams->u32A3CDepthLevel;	

			/* For Status Management */
			status = (unsigned int)stHwParams.uA3CDepth;
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_3D_DEPTH, status);

			ret = BE_KIPC_SetData(H3D_PACK_CMD(BE_H3D_B0_A3C_DEPTH_LEVEL), &stHwParams, sizeof(BE_IPC_B0_H3D_A3C_DEPTH_T));
			if(ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_H3D_A3C_DEPTH_LEVEL_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.u8A3CDepthLevel = pstParams->u32A3CDepthLevel;

			/* return unexpected value */
			if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}	

			/* For Status Management */
			status = (unsigned int)stHwParams.u8A3CDepthLevel;
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_3D_DEPTH, status);

			ret = BE_KIPC_SetData(BE_H3D_A3C_DEPTH_LEVEL, &stHwParams, sizeof(BE_IPC_H3D_A3C_DEPTH_LEVEL_T));
			if(ret) BREAK_WRONG(ret);
		}
	} while(0);
	return ret;
}

int BE_H3D_HW_M14_A3C_AutoConvergence(LX_BE_H3D_A3C_AUTOCONVERGENCE_T *pstParams)
{
	int ret = RET_OK;
	unsigned int status;

	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_H3D_AUTOCONVERGENCE_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.bTDAutoConvOn = pstParams->bA3CAutoConvergence; 	

			/* For Status Management */
			status = (unsigned int)stHwParams.bTDAutoConvOn;
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_AUTO_CONVERGENCE, status);

			ret = BE_KIPC_SetData(H3D_PACK_CMD(BE_H3D_B0_A3C_AUTOCONVERGENCE), &stHwParams, sizeof(BE_IPC_B0_H3D_AUTOCONVERGENCE_T));
			if(ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_H3D_A3C_AUTOCONVERGENCE_T, stHwParams);

			/* change value LX_BE to BE_IPC */
			stHwParams.bA3CAutoConvergence = pstParams->bA3CAutoConvergence; 

			/* return unexpected value */
			if(*(ULONG *)&stHwParams == (ULONG)0xFFFFFFFF)
			{
				return ret;
			}	

			/* For Status Management */
			status = (unsigned int)stHwParams.bA3CAutoConvergence;
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_AUTO_CONVERGENCE, status);

			ret = BE_KIPC_SetData(BE_H3D_A3C_AUTOCONVERGENCE, &stHwParams, sizeof(BE_IPC_H3D_A3C_AUTOCONVERGENCE_T));
			if(ret) BREAK_WRONG(ret);
		}
	} while(0);
	return ret;
}

int BE_H3D_HW_M14_Set_TridFmt( LX_BE_TRIDTV_FMT_CTRL_T *pstParams)
{
	int ret = RET_OK;
	unsigned int status = 0;

	do{
		if (lx_chip_rev() >= LX_CHIP_REV( M14, B0))
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_H3D_MODE_T, stHwParams1);
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_H3D_A3C_DEPTH_T, stHwParams3);
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_H3D_T3C_DEPTH_T, stHwParams4);
			IPC_SET_DEFAULT_PARAM(BE_IPC_B0_3D_CONVERGENCE_T, stHwParams5);

			switch (pstParams->lx_mode)
			{
				case LX_TRIDTV_3DMODE_ON:
					{
						switch (pstParams->lx_in_fmt)
						{
							case LX_SC_OUT_DUAL_FHD:
								stHwParams1.uA3CMode = 1; // 0:off, 1:on
								stHwParams1.uT3CMode = 0; // 0:off, 1:on
								break;
							case LX_SC_OUT_DUAL_TB:
								stHwParams1.uA3CMode = 1; // 0:off, 1:on
								stHwParams1.uT3CMode = 0; // 0:off, 1:on
								break;
							case LX_SC_OUT_DUAL_SS:
								stHwParams1.uA3CMode = 1; // 0:off, 1:on
								stHwParams1.uT3CMode = 0; // 0:off, 1:on
								break;
							default :
								BE_ERROR("lx_in_fmt is invalied param for[M14], only use ""LX_SC_OUT_DUAL_"" case\n");
								BREAK_WRONG(-1);
								break;
						}
						break;
					}

				case LX_TRIDTV_3DMODE_2DTO3D:
					{
						stHwParams1.uA3CMode = 0; // 0:off, 1:on
						stHwParams1.uT3CMode = 1; // 0:off, 1:on
					}
					break;
				case LX_TRIDTV_3DMODE_3DTO2D:
				case LX_TRIDTV_3DMODE_OFF:
					stHwParams1.uA3CMode = 0; // 0:off, 1:on
					stHwParams1.uT3CMode = 0; // 0:off, 1:on
					break;

				default:
					BE_ERROR("lx_mode is invalied param for[M14]\n");
					BREAK_WRONG(-1);
					break;
			}

			/* For Status Management */
			status = (0xFFFF0000&(stHwParams1.uA3CMode<<16))|(0x0000FFFF&(stHwParams1.uT3CMode));
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_3D_MODE, status);

			if((stHwParams1.uA3CMode == 1)  || (stHwParams1.uT3CMode == 1))
			{
				if(stHwParams1.uA3CMode == 1)
				{
					/* change value LX_BE to BE_IPC */
					stHwParams3.uA3CDepth = pstParams->depthCtrl;
					/* return unexpected value */
					ret = BE_KIPC_SetData(H3D_PACK_CMD(BE_H3D_B0_A3C_DEPTH_LEVEL), &stHwParams3, sizeof(BE_IPC_B0_H3D_A3C_DEPTH_T));
					if(ret) BREAK_WRONG(ret);
				}	
				else 
				{
					/* change value LX_BE to BE_IPC */
					stHwParams4.uT3CDepth = pstParams->depthCtrl;
					/* return unexpected value */
					ret = BE_KIPC_SetData(H3D_PACK_CMD(BE_H3D_B0_T3C_DEPTH_LEVEL), &stHwParams4, sizeof(BE_IPC_B0_H3D_T3C_DEPTH_T));
					if(ret) BREAK_WRONG(ret);
				}	
				msleep(20);

				/* change value LX_BE to BE_IPC */
				stHwParams5.bTDConvEnable = CONV_ENABLE;
				stHwParams5.uTDConvVal = (UINT32)(pstParams->convCtrl+10);

				/* Video Status */
				status = (0xFFFF0000&(stHwParams5.bTDConvEnable<<16))|(0x0000FFFF&(stHwParams5.uTDConvVal));
				(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_CONVERGENCE, status);

				/* return unexpected value */
				ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_SET_3D_CONVERGENCE), (void *)&stHwParams5, sizeof(BE_IPC_B0_3D_CONVERGENCE_T));
				if(ret) BREAK_WRONG(ret);
			}
			else
			{	
				/* change value LX_BE to BE_IPC */
				stHwParams5.bTDConvEnable = CONV_DISABLE;
				stHwParams5.uTDConvVal = (UINT32)(pstParams->convCtrl+10);

				/* Video Status */
				status = (0xFFFF0000&(stHwParams5.bTDConvEnable<<16))|(0x0000FFFF&(stHwParams5.uTDConvVal));
				(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_CONVERGENCE, status);

			}
			/* return unexpected value */
			ret = BE_KIPC_SetData(TOP_PACK_CMD(BE_TOP_B0_SET_3D_CONVERGENCE), (void *)&stHwParams5, sizeof(BE_IPC_B0_3D_CONVERGENCE_T));
			if(ret) BREAK_WRONG(ret);
			ret = BE_KIPC_SetData(H3D_PACK_CMD(BE_H3D_B0_SET_MODE), &stHwParams1, sizeof(BE_IPC_B0_H3D_MODE_T));
			if(ret) BREAK_WRONG(ret);
		}
		else
		{
			IPC_SET_DEFAULT_PARAM(BE_IPC_TOP_3D_IN_FORMAT_T, stHwParams0);
			IPC_SET_DEFAULT_PARAM(BE_IPC_H3D_MODE_T, stHwParams1);
			IPC_SET_DEFAULT_PARAM(BE_IPC_H3D_IN_FORMAT_T, stHwParams2);
			IPC_SET_DEFAULT_PARAM(BE_IPC_H3D_A3C_DEPTH_LEVEL_T, stHwParams3);
			IPC_SET_DEFAULT_PARAM(BE_IPC_H3D_T3C_DEPTH_LEVEL_T, stHwParams4);
			IPC_SET_DEFAULT_PARAM(BE_IPC_3D_CONVERGENCE_T, stHwParams5);

			switch (pstParams->lx_mode)
			{
				case LX_TRIDTV_3DMODE_ON:
					{
						switch (pstParams->lx_in_fmt)
						{
							case LX_SC_OUT_DUAL_FHD:
								stHwParams0.u4BE3DInFormat 	= 3; // 0:2D, 1:S/S, 2:T/B, 3:Dual
								stHwParams0.u4BE3DInType   	= 1; // 0:3D Normal, 1:Dual Stream
								stHwParams0.u4BE3DInSize	= 0; // 0:1920x1080, 1:960x1080, 2:1920x540, 3:UD
								stHwParams2.u4H3DInFormat 	= 0; // 0:FHD, 1:SS, 2:TB
								stHwParams1.bA3CMode = 1; // 0:off, 1:on
								stHwParams1.bT3CMode = 0; // 0:off, 1:on
								break;
							case LX_SC_OUT_DUAL_TB:
								stHwParams0.u4BE3DInFormat 	= 2; // 0:2D, 1:S/S, 2:T/B, 3:Dual
								stHwParams0.u4BE3DInType   	= 0; // 0:3D Normal, 1:Dual Stream
								stHwParams0.u4BE3DInSize	= 2; // 0:1920x1080, 1:960x1080, 2:1920x540, 3:UD
								stHwParams2.u4H3DInFormat 	= 2; // 0:FHD, 1:SS, 2:TB
								stHwParams1.bA3CMode = 1; // 0:off, 1:on
								stHwParams1.bT3CMode = 0; // 0:off, 1:on
								break;
							case LX_SC_OUT_DUAL_SS:
								stHwParams0.u4BE3DInFormat 	= 1; // 0:2D, 1:S/S, 2:T/B, 3:Dual
								stHwParams0.u4BE3DInType   	= 0; // 0:3D Normal, 1:Dual Stream
								stHwParams0.u4BE3DInSize	= 1; // 0:1920x1080, 1:960x1080, 2:1920x540, 3:UD
								stHwParams2.u4H3DInFormat 	= 1; // 0:FHD, 1:SS, 2:TB
								stHwParams1.bA3CMode = 1; // 0:off, 1:on
								stHwParams1.bT3CMode = 0; // 0:off, 1:on
								break;
							default :
								BE_ERROR("lx_in_fmt is invalied param for[M14], only use ""LX_SC_OUT_DUAL_"" case\n");
								BREAK_WRONG(-1);
								break;
						}
						break;
					}

				case LX_TRIDTV_3DMODE_2DTO3D:
					{
						stHwParams0.u4BE3DInFormat 	= 2; // 0:2D, 1:S/S, 2:T/B, 3:Dual
						stHwParams0.u4BE3DInType   	= 0; // 0:3D Normal, 1:Dual Stream
						stHwParams0.u4BE3DInSize	= 2; // 0:1920x1080, 1:960x1080, 2:1920x540, 3:UD, H13A1���� ����  忹��
						stHwParams1.bA3CMode = 0; // 0:off, 1:on
						stHwParams1.bT3CMode = 1; // 0:off, 1:on
						stHwParams2.u4H3DInFormat = 2; // 0:FHD, 1:SS, 2:TB

					}
					break;
				case LX_TRIDTV_3DMODE_3DTO2D:
				case LX_TRIDTV_3DMODE_OFF:
					stHwParams0.u4BE3DInFormat 	= 0; // 0:2D, 1:S/S, 2:T/B, 3:Dual
					stHwParams0.u4BE3DInType   	= 0; // 0:3D Normal, 1:Dual Stream
					stHwParams0.u4BE3DInSize	= 0; // 0:1920x1080, 1:960x1080, 2:1920x540, 3:UD
					stHwParams1.bA3CMode = 0; // 0:off, 1:on
					stHwParams1.bT3CMode = 0; // 0:off, 1:on
					stHwParams2.u4H3DInFormat = 0 ; // 0:FHD, 1:SS, 2:TB
					break;

				default:
					BE_ERROR("lx_mode is invalied param for[M14]\n");
					BREAK_WRONG(-1);
					break;
			}

			/* For Status Management */
			status = (0xFFFF0000&(stHwParams1.bA3CMode<<16))|(0x0000FFFF&(stHwParams1.bT3CMode));
			(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_3D_MODE, status);

			if((stHwParams1.bA3CMode == 1)  || (stHwParams1.bT3CMode == 1))
			{
				if(stHwParams1.bA3CMode == 1)
					/* change value LX_BE to BE_IPC */
					stHwParams3.u8A3CDepthLevel = pstParams->depthCtrl;
				/* return unexpected value */
				if(*(ULONG *)&stHwParams3 != (ULONG)0xFFFFFFFF)
				{
					ret = BE_KIPC_SetData(BE_H3D_A3C_DEPTH_LEVEL, &stHwParams3, sizeof(BE_IPC_H3D_A3C_DEPTH_LEVEL_T));
					if(ret) BREAK_WRONG(ret);
				}
				else 
					/* change value LX_BE to BE_IPC */
					stHwParams4.u8T3CDepthLevel = pstParams->depthCtrl;
				/* return unexpected value */
				if(*(ULONG *)&stHwParams4 != (ULONG)0xFFFFFFFF)
				{
					ret = BE_KIPC_SetData(BE_H3D_T3C_DEPTH_LEVEL, &stHwParams4, sizeof(BE_IPC_H3D_T3C_DEPTH_LEVEL_T));
					if(ret) BREAK_WRONG(ret);
				}

				msleep(20);

				/* change value LX_BE to BE_IPC */
				stHwParams5.u16ConvergenceEnable = CONV_ENABLE;
				stHwParams5.u16ConvergenceVal = (UINT16)(pstParams->convCtrl + 10);

				/* Video Status */
				status = (0xFFFF0000&(stHwParams5.u16ConvergenceEnable<<16))|(0x0000FFFF&(stHwParams5.u16ConvergenceVal));
				(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_CONVERGENCE, status);

				/* return unexpected value */
				if(*(ULONG *)&stHwParams5 != (ULONG)0xFFFFFFFF)
				{
					ret = BE_KIPC_SetData(BE_TOP_SET_3D_CONVERGENCE, (void *)&stHwParams5, sizeof(BE_IPC_3D_CONVERGENCE_T));
					if(ret) BREAK_WRONG(ret);
				}
			}
			else
			{	
				/* change value LX_BE to BE_IPC */
				stHwParams5.u16ConvergenceEnable = CONV_DISABLE;
				stHwParams5.u16ConvergenceVal = (UINT16)(pstParams->convCtrl + 10);

				/* Video Status */
				status = (0xFFFF0000&(stHwParams5.u16ConvergenceEnable<<16))|(0x0000FFFF&(stHwParams5.u16ConvergenceVal));
				(void)VIDEO_SKeeperUpdateIndex(LX_VP_ST_KEEPER_MODULE_BE, BE_SKEEPER_H3D_CONVERGENCE, status);

				/* return unexpected value */
				if(*(ULONG *)&stHwParams5 != (ULONG)0xFFFFFFFF)
				{
					ret = BE_KIPC_SetData(BE_TOP_SET_3D_CONVERGENCE, (void *)&stHwParams5, sizeof(BE_IPC_3D_CONVERGENCE_T));
					if(ret) BREAK_WRONG(ret);
				}
			}


			/* check unexpected value */
			if(*((ULONG *)&stHwParams2) != (ULONG)0xFFFFFFFF)
			{
				ret = BE_KIPC_SetData(BE_H3D_IN_FORMAT, &stHwParams2, sizeof(BE_IPC_H3D_IN_FORMAT_T));
				if(ret) BREAK_WRONG(ret);
			}

			if(*((ULONG *)&stHwParams1) != (ULONG)0xFFFFFFFF)
			{
				ret = BE_KIPC_SetData(BE_H3D_SET_MODE, &stHwParams1, sizeof(BE_IPC_H3D_MODE_T));
				if(ret) BREAK_WRONG(ret);
			}

			if(*((ULONG *)&stHwParams0) != (ULONG)0xFFFFFFFF)
			{
				ret = BE_KIPC_SetData(BE_TOP_SET_3D_IN_FORMAT, &stHwParams0, sizeof(BE_IPC_TOP_3D_IN_FORMAT_T));		
				if(ret) BREAK_WRONG(ret);
			}
		}
	} while(0);
	return ret;
} 
