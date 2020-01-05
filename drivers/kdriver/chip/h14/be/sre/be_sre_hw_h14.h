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
 *	BE SRE module control Drivers linux module
 *
 *  author      dj911.kim@lge.com
 *  version		1.0
 *  date
 *  note		Additional information.
 *
 *  @addtogroup lg115x_be
 *	@{
 */
 
#ifndef  BE_SRE_HW_H14_INC
#define  BE_SRE_HW_H14_INC
/*----------------------------------------------------------------------------------------
 *   Control Constants
 *---------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
 *   File Inclusions
 *---------------------------------------------------------------------------------------*/
#include "be_kapi.h"

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
int BE_SRE_HW_H14_Initialize(void);
int BE_SRE_HW_H14_Close(void);
int BE_SRE_HW_H14_Set_Command(LX_BE_SRE_CMN_T *pstParams);
int BE_SRE_HW_H14_Set_Params(LX_BE_SRE_PRAM_T *pstParams);
int BE_SRE_HW_H14_Get_Version(LX_BE_SRE_VERSION_T *pstParams);
int BE_SRE_HW_H14_Set_Input_Format(LX_BE_SRE_INPUT_MODE_T *pstParams);

/*----------------------------------------------------------------------------------------
 *   External Variables
 *---------------------------------------------------------------------------------------*/

#endif   /* ----- #ifndef BE_SRE_HW_H14_INC  ----- */
/**  @} */

