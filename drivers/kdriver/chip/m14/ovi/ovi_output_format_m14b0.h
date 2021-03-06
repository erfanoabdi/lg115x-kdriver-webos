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
 *  Header file of defining possible of output modes of M14Bx 	
 *
 *  author      won.hur@lge.com
 *  version		1.0
 *  date
 *  note		Additional information.
 *
 *  @addtogroup lg115x_ovi
 *	@{
 */
 
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "ovi_kapi.h"

/*----------------------------------------------------------------------------------------
 *   Constant Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Macro Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   Type Definitions
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Function Prototype Declarations
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

static LX_OVI_REG_T ovi_m14b0_debug_mode_detour_h3d[] =
{
	{0xC002c010 ,0x01000000},
};

static LX_OVI_REG_T ovi_m14b0_debug_mode_detour_frc[] =
{
	{0xC0028100 ,0x0},
	{0xC0028104 ,0x11},
};

static LX_OVI_REG_T ovi_m14b0_debug_mode_detour_pe1[] =
{
	{0xC0035308 ,0x01000A00},
};

static LX_OVI_REG_T ovi_m14b0_debug_mode_detour_sre[] =
{
	{0xC0025004 ,0x1},
};

static LX_OVI_REG_T ovi_m14b0_debug_mode_detour_dppb[] =
{
	{0xC0030010 ,0x1},
	{0xC0030768 ,0x10100},
};

static LX_OVI_REG_T ovi_m14b0_debug_mode_detour_tcon[] =
{
	{0xC0040004, 0x0},
	{0xC0040008, 0x80},
	{0xC004001C, 0x0},
	{0xC0049000 ,0x042D0004},
	{0xC0049018 ,0x0000721A},
};



/**  @} */

