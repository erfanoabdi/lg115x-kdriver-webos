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
 *  ipc reg header file for BE device
 *	
 *  author		dj911.kim
 *  version		1.0
 *  date		2013.04.03
 *  note		Additional information.
 *
 *  @addtogroup BE
 *	@{
 */

#ifndef _BE_IPC_HW_H14_H_
#define _BE_IPC_HW_H14_H_

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	File Inclusions
----------------------------------------------------------------------------------------*/
#include "base_types.h"

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Type Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	External Function Prototype Declarations
----------------------------------------------------------------------------------------*/
int BE_IPC_HW_H14_Init(UINT32 *pIPCBuffToMCU, UINT32 *pIPCBuffFrMCU);
int BE_IPC_HW_H14_Close(UINT32 *pIPCBuffToMCU, UINT32 *pIPCBuffFrMCU);

/*----------------------------------------------------------------------------------------
	External Variables
----------------------------------------------------------------------------------------*/

#endif /* _BE_IPC_HW_H14_H_ */

/** @} */








