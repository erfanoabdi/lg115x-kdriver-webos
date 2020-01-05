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


/*****************************************************************************
**
**  Name: ademod_common_userdef.c
**
**  Description:    ABB Demodulator user-defined interface.
**				Low level functions such as reading and writing to the register
**				as well as timing delay are platform dependent and therefore
**				must be implemented by the user.
**
**  Functions
**  Implemented:    LX_ADEMOD_Result  ADEMOD_WriteAutoIncrement
**                  LX_ADEMOD_Result  ADEMOD_ReadAutoIncrement
**                  LX_ADEMOD_Result  ADEMOD_Sleep
**
**  References:     
**
**  Exports:		LX_ADEMOD_Result  ADEMOD_WriteAutoIncrement
**                  LX_ADEMOD_Result  ADEMOD_ReadAutoIncrement
**                  LX_ADEMOD_Result  ADEMOD_Sleep
**
**  Dependencies:  demod_module.h   - for low level 2 wire functions.
**
**  Revision History:
**
**     Date        Author          Description
**  -------------------------------------------------------------------------
**   31-07-2013   Jeongpil Yun    Initial draft.
**
*****************************************************************************/
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
#include "ademod_common_device.h"
#include "ademod_common_userdef.h"

#define AUTO_INCREMENT	(0x40) // bit 6: 1- Auto increment, 0- Poll


extern	LX_ADEMOD_Result ADEMOD_GetContext(Handle_t hDevice, LX_ADEMOD_Demod_Instance** ppADEMOD_instance);
//LGIT EnD

#if defined( __cplusplus )
extern "C"                     /* Use "C" external linkage                  */
{
#endif

#define AUTO_INCREMENT	(0x40) // bit 6: 1- Auto increment, 0- Poll




/*
**  API implementation.
*/

/*****************************************************************************
**
**  Name: ADEMOD_WriteAutoIncrement
**
**  Description:    Writes up to 4 bytes of data to the device using I2C Auto Increment mode.
**					In auto increment mode, the register address is automatically incremented
**					after each byte of data is written.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to demod's Open function.
**                  reg_addr   - Register Address.
**                  reg_data   - Data to be written to the device.
**                  cnt        - number of bytes to be written (from 1 to 4)
**
**  Returns:        status:
**                      retOK           - No errors
**
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_WriteAutoIncrement(Handle_t hUserData,
                    UINT32  reg_addr,
                    UINT32  reg_data,
                    UINT32  cnt)
{
	UINT8		*pData		= NULL;
//	UINT8		index		= 0;

	if (cnt > LX_ADEMOD_MAX_REGISTER_LENGTH)
	{
		DEMOD_PRINT("[M14  ABB] FAIL (%s:%d) INVALID COUNT(%d) !!!\n", __F__, __L__, cnt);
		return retErr;
	}

	pData		= (UINT8 *)&reg_data;


	if (retOK != DEMOD_ANALOG_I2C_Write((UINT16)reg_addr, cnt, pData))
	{
		DEMOD_PRINT("[M14  ABB] FAIL (%s:%d) DEMOD_ANALOG_I2C_Write  !!!\n", __F__, __L__);

		if (retOK != DEMOD_ANALOG_I2C_Write((UINT16)reg_addr, cnt, pData))
		{
			DEMOD_PRINT("[M14 ABB] FAIL (%s:%d) DEMOD_ANALOG_I2C_Write  !!!\n", __F__, __L__);
			return retErr;
		}
	}

	return retOK;


}

/*****************************************************************************
**
**  Name: ADEMOD_ReadAutoIncrement
**
**  Description:    Reads up to 4 bytes of data from the device using I2C with Auto Increment.
**
**  Parameters:     hUserData  - User-specific I/O parameter that was
**                               passed to demod's Open function.
**                  reg_addr   - Register Address.
**                  preg_data  - pointer to the Data read from the device.
**                  cnt        - number of bytes to read (from 1 to 4).
**
**  Returns:        status:
**                      retOK           - No errors
**
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_ReadAutoIncrement(Handle_t hUserData,
                   UINT32 reg_addr,
                   UINT32 *preg_data,
                   UINT32 cnt)
{

	UINT8		data[8] 	= {0};
//	UINT8		index		= 0;

	if (cnt > LX_ADEMOD_MAX_REGISTER_LENGTH)
	{
		DEMOD_PRINT("[M14  ABB] FAIL (%s:%d) INVALID COUNT(%d) !!!\n", __F__, __L__, cnt);
		return retErr;
	}


	if (retOK != DEMOD_ANALOG_I2C_Read((UINT16)reg_addr, cnt, data))
	{
		DEMOD_PRINT( "[M14 ABB] FAIL (%s:%d) DEMOD_ANALOG_I2C_Read  !!!\n", __F__, __L__);

		if (retOK != DEMOD_ANALOG_I2C_Read((UINT16)reg_addr, cnt, data))
		{
			DEMOD_PRINT( "[M14 ABB] FAIL (%s:%d) DEMOD_ANALOG_I2C_Read !!!\n", __F__, __L__);
			return retErr;
		}
	}

	*preg_data	= *(UINT64 *)data;

	return retOK;


}

/*****************************************************************************
**
**  Name: ADEMOD_Sleep
**
**  Description:    Delay execution for "nMinDelayTime" milliseconds
**
**  Parameters:     hUserData     - User-specific I/O parameter that was
**                                  passed to demod's Open function.
**                  nMinDelayTime - Delay time in milliseconds
**
**  Returns:        None.
**
**  Notes:          This is a callback function that is called from the
**                  the tuning algorithm.  You MUST provide code that
**                  blocks execution for the specified period of time.
**
*****************************************************************************/
LX_ADEMOD_Result ADEMOD_Sleep(UINT32 nMinDelayTime)
{
    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */

    return status;

}

// << modified for normal spectrum>>
//static	UINT32 LX_ADEMOD_DK_NICAM[] = { 0x1fd,0x2,0x0,0x1ff,0x4,0x1,0x1f8,0x1f5,0x3,0x0,0xb,0x0,0x1f6,0x9,0x1fc,0x1ff,0x1fe,0x0,0x6,0x1ff,0x2,0x1ffa,0x9,0x7,0x1ff2,0x5,0x1ffb,0xa,0x1ff8,0x1fde,0x6f,0x1f70,0x40,0xdb,0x1dac,0x3c2,0xb81,0x3c1,0x1dad,0xda,0x41,0x1f6f,0x70,0x1fdd,0x1ff8,0xa,0x1ffb,0x5,0x1ff2,0x7,0x8,0x1ffa,0x2,0x1fff,0x6,0x0,0x1ffe,0x1fff,0x1ffc,0x9,0x1f6,0x0,0xb,0x0,0x3,0x1f5,0x1f7,0x2,0x3,0x0,0x0,0x3,0x1fd,0x2,0x1fe,0x1fe,0x1,0x1fd,0x2,0x1fe,};
//static	UINT32 LX_ADEMOD_L[] = { 0x1fd,0x1,0x2,0x1fd,0x6,0x1fe,0x1f8,0x1f6,0x3,0x2,0x8,0x0,0x1f5,0x9,0x1fd,0x1fc,0x1,0x1ff,0x7,0x1ff,0x1,0x1ffb,0x9,0x6,0x1ff4,0x3,0x1ffc,0xa,0x1ff7,0x1fe1,0x67,0x1f80,0x2b,0xed,0x1da7,0x3ba,0xb96,0x3ba,0x1da7,0xe9,0x34,0x1f71,0x79,0x1fcf,0x5,0x2,0x1fff,0x5,0x1fef,0xb,0x4,0x1ffd,0x0,0x1fff,0x6,0x1,0x1ffe,0x1fff,0x1ffb,0x9,0x1f6,0x1fe,0xa,0x1,0x3,0x1f6,0x1f7,0x0,0x4,0x1ff,0x1ff,0x4,0x1fc,0x3,0x1fd,0x1ff,0x1,0x1fd,0x2,0x1fd,};
//static	UINT32 LX_ADEMOD_DK[] = { 0x0,0x2,0x1fc,0x6,0x1,0x1f6,0x1f8,0x1fc,0x3,0x8,0x1fe,0x1fc,0x1fe,0x3,0x1fd,0x1fa,0x7,0x0,0x5,0x0,0x1ff5,0x10,0x1,0x1ff7,0x0,0x6,0x7,0x1ff2,0x1ffb,0x43,0x1f97,0x23,0x35,0x1d27,0xf9,0x9e9,0x741,0x1f12,0x47,0x19e,0x1f7e,0x1fe2,0x90,0x1faa,0xe,0x19,0x1fe3,0x11,0x1ff0,0xc,0x4,0x1ff7,0x1,0x8,0x1ffc,0x8,0x1ff7,0x1,0x5,0x1ff8,0x1fe,0x2,0x7,0x3,0x1fa,0x1f9,0x1f8,0x4,0x0,0x1fd,0x3,0x0,0x1fe,0x2,0x1fd,0x1ff,0x0,0x1fd,0x2,0x1fe,};
//static	UINT32 LX_ADEMOD_I[] =  { 0x1ff,0x1ff,0x4,0x1fc,0x5,0x3,0x1f4,0x1fb,0x1fc,0x5,0x9,0x1fe,0x1fd,0x1,0x3,0x1fd,0x1fb,0x6,0x1fd,0x8,0x1ffb,0x1ffb,0xd,0x0,0x1ff5,0x7,0x1ff4,0x1d,0x1fd3,0x10,0x53,0x1f4c,0xa6,0x6c,0x1dd6,0x3f7,0xb1a,0x3f7,0x1dd6,0x6c,0xa6,0x1f4b,0x53,0x10,0x1fd3,0x1d,0x1ff4,0x8,0x1ff5,0x1,0xd,0x1ffb,0x1ffb,0x8,0x1ffd,0x6,0x1ffb,0x1ffd,0x3,0x0,0x1fd,0x1fe,0x9,0x5,0x1fc,0x1fb,0x1f3,0x4,0x5,0x1fd,0x3,0x0,0x1ff,0x1,0x1fd,0x0,0x1fe,0x1ff,0x0,0x1fe, };
//static	UINT32 LX_ADEMOD_M[] = { 0x1ff,0x0,0x3,0x1fd,0x2,0x1ff,0x1f1,0x1f9,0x4,0x5,0x7,0x1fe,0x1f7,0x7,0x5,0x1f7,0x1f9,0x2,0x5,0x3,0x1ff7,0x1ffb,0x7,0x0,0x1ffa,0x1ff9,0x4,0xe,0x1fed,0x1ff9,0x42,0x1fad,0x22,0xb9,0x1e00,0x355,0xbd6,0x423,0x1cf4,0x153,0x49,0x1f01,0xeb,0x1fb9,0x1fbf,0x6a,0x1fc1,0xa,0x9,0x1fef,0xf,0x1ff8,0x1ffc,0x1ffb,0xc,0x1ffd,0x1ffb,0x1ff6,0x7,0x5,0x1f9,0x1fc,0xa,0x4,0x3,0x1fb,0x1f1,0x1fc,0x5,0x1fc,0x2,0x3,0x1fd,0x0,0x0,0x1fd,0x2,0x1fd,0x1ff,0x1fe,};
//static	UINT32 LX_ADEMOD_BG[] = { 0x0,0x0,0x2,0x1,0x5,0x1fd,0x1f1,0x0,0x1ff,0x6,0x8,0x1f7,0x2,0x6,0x1fb,0x1fc,0x4,0x1fe,0x4,0x2,0x1ff7,0x8,0x7,0x1ffd,0x1ffe,0x1ffc,0x3,0x1ffd,0x1fe9,0x1a,0x1ff1,0x1fe2,0x2,0x1e2c,0xa,0x99e,0x87d,0x1e03,0x82,0x215,0x1e8d,0x95,0x73,0x1f47,0x9d,0x1fb5,0xf,0x1a,0x1fc5,0x45,0x1fd1,0x1c,0x1ff6,0x1,0xd,0x1ff5,0x2,0x0,0x0,0x3,0x1f4,0xd,0x6,0x1ff,0x1fe,0x1f4,0x1fd,0x6,0x1ff,0x1ff,0x4,0x1fe,0x1ff,0x2,0x1fb,0x3,0x1fd,0x1,0x1ff,0x1ff, };
//static	UINT32 LX_ADEMOD_L1[] = { 0x1fd,0x1,0x2,0x1fd,0x6,0x1fe,0x1f8,0x1f6,0x3,0x2,0x8,0x0,0x1f5,0x9,0x1fd,0x1fc,0x1,0x1ff,0x7,0x1ff,0x1,0x1ffb,0x9,0x6,0x1ff4,0x3,0x1ffc,0xa,0x1ff7,0x1fe1,0x67,0x1f80,0x2b,0xed,0x1da7,0x3ba,0xb96,0x3ba,0x1da7,0xe9,0x34,0x1f71,0x79,0x1fcf,0x5,0x2,0x1fff,0x5,0x1fef,0xb,0x4,0x1ffd,0x0,0x1fff,0x6,0x1,0x1ffe,0x1fff,0x1ffb,0x9,0x1f6,0x1fe,0xa,0x1,0x3,0x1f6,0x1f7,0x0,0x4,0x1ff,0x1ff,0x4,0x1fc,0x3,0x1fd,0x1ff,0x1,0x1fd,0x2,0x1fd,};

// << modified for inverted spectrum>>
static  UINT32 LX_ADEMOD_M[] = {0x1fe,0x1fd,0x7,0x1f9,0x6,0x1fe,0x1f0,0x0,0x1ff,0x8,0x3,0x1fe,0x1fa,0x7,0x1fc,0x1fd,0x1fd,0x0,0x5,0x1f3,0x1ffc,0x3,0x1ff8,0x9,0x1ff5,0x8,0x6,0x1ffc,0x1,0x9,0x6,0x1fea,0xa,0x4c,0x1e47,0x5ea,0xaf5,0xe3,0x1e5a,0x1c1,0x1f57,0x1fe5,0xcf,0x1f46,0x90,0x1fc9,0x1a,0x3,0x1ff9,0xb,0x1ff5,0x5,0x1ff8,0x1ff6,0x8,0x0,0x1ff7,0x3,0x1ffd,0x5,0x1f9,0x1,0x9,0x2,0x2,0x1f6,0x1f7,0x0,0x4,0x1fa,0x5,0x1fe,0x1fd,0x5,0x1f9,0x3,0x1ff,0x1fe,0x2,0x1fd}; //(Ver. 1121_4M)
static	UINT32 LX_ADEMOD_BG[] = {0x1f7,0xf,0x1ee,0x15,0x1f1,0x7,0x1ec,0x0,0x9,0x1f9,0x13,0x1eb,0xe,0x1fb,0x0,0x1fd,0x1fc,0xf,0x1ec,0x14,0x1fe6,0xf,0x7,0x1fe4,0x18,0x1ff1,0x1ff8,0x1ffd,0x1fdd,0xb,0x1fe4,0xc,0x1f90,0x1e65,0x202,0x9e7,0x639,0x1dfc,0x1ca,0xba,0x1eb8,0x156,0x1f7d,0x1ff6,0x71,0x1f75,0x82,0x1fa3,0x39,0x1ff3,0x1ffd,0xd,0x1fea,0x21,0x1fe3,0x18,0x1fef,0x1fff,0x13,0x1fe6,0x12,0x1fc,0x7,0x7,0x1eb,0x8,0x1ee,0x14,0x1f3,0x5,0x4,0x1f4,0xf,0x1ef,0xf,0x1f1,0x9,0x1fa,0x1fd,0x9};
static	UINT32 LX_ADEMOD_DK[] = {0x1fe,0x2,0x1,0x1fd,0x8,0x1f6,0x1f4,0x2,0x1fe,0x7,0x5,0x1f2,0x8,0x0,0x1f5,0xc,0x1f8,0x0,0xe,0x1f4,0x3,0x3,0x0,0x2,0x1ff0,0x18,0x1ffe,0x1fcf,0x42,0x1fe2,0x16,0x1feb,0x1fc8,0x1e9c,0x1f9d,0x9ce,0x772,0x1e89,0x1ae,0xc1,0x1f7a,0xa2,0x1fa8,0x57,0x1fc6,0x8,0x17,0x1fd4,0x19,0x1ff6,0x4,0x1ffe,0x1ff7,0xc,0x1ffc,0x1ffc,0xb,0x1ff3,0x6,0x3,0x1f0,0xd,0x4,0x1ff,0x3,0x1f1,0x1fc,0x6,0x1fa,0x3,0x2,0x1fc,0x2,0x1fe,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff,0x1ff};  //(Ver. 1114)
static	UINT32 LX_ADEMOD_I[] =  {0x1ff,0x1ff,0x4,0x1fc,0x6,0x2,0x1f5,0x1fa,0x1fe,0x3,0xa,0x0,0x1fa,0x4,0x3,0x1fc,0x1ff,0x1ff,0x2,0x2,0x1ffc,0x1ffc,0xb,0x1ff8,0x1ffb,0x8,0x1fed,0x1ffa,0x26,0x1f9a,0x78,0x1fd1,0x1fb5,0x14b,0x1dfc,0x360,0xb73,0x360,0x1dfc,0x14b,0x1fb5,0x1fd1,0x78,0x1f9a,0x27,0x1ffa,0x1fed,0x8,0x1ffa,0x1ff8,0xb,0x1ffd,0x1ffc,0x2,0x2,0x1fff,0x1fff,0x1ffc,0x3,0x4,0x1fb,0x0,0xa,0x2,0x1fe,0x1fa,0x1f5,0x3,0x6,0x1fc,0x4,0x1ff,0x1fe,0x1,0x1fe,0x0,0x1ff,0x1fe,0x0,0x1ff}; //(Ver. 1112_7)
static	UINT32 LX_ADEMOD_L[] =  {0x1fb,0x1ff,0x5,0x1fb,0x4,0x6,0x1ee,0x1fe,0x1fe,0x4,0x6,0x6,0x1ed,0x12,0x1f5,0x3,0x1fa,0xa,0x1fa,0xa,0x1ff6,0x1ffb,0x10,0x1fff,0x1fef,0x1f,0x1fd5,0x32,0x1fc8,0x23,0x0,0x12,0x1fb2,0xbe,0x1efb,0x18a,0xde3,0x1f9,0x1e34,0x1b7,0x1eb2,0xf2,0x1f59,0x8a,0x1f9c,0x32,0x1fef,0x1ffc,0xf,0x1fe7,0x1f,0x1ff3,0x1ffc,0x5,0x1fff,0x5,0x1fff,0x1ffd,0x1ffd,0x8,0x1f8,0x1f9,0x11,0x1f9,0x7,0x1f6,0x1f5,0x1ff,0xa,0x1f6,0x8,0x1fb,0x0,0x1,0x1fe,0x1fe,0x3,0x1f9,0x6,0x1f8};
static  UINT32 LX_ADEMOD_L1[] = {0x1fb,0x0,0x4,0x1fa,0x4,0x6,0x1ec,0x1ff,0x1fe,0x4,0x5,0x6,0x1eb,0x14,0x1f4,0x4,0x1f9,0xb,0x1f9,0x8,0x1ff8,0x1ffc,0xc,0x4,0x1fec,0x1c,0x1fd8,0x37,0x1fb8,0x27,0x1ffa,0x19,0x1fa6,0xc9,0x1ede,0x14b,0xeb1,0x1c7,0x1dfe,0x1e1,0x1e87,0x115,0x1f3e,0x9b,0x1f86,0x36,0x1ff6,0x1ff4,0x10,0x1fea,0x1d,0x1ff2,0x1ffe,0x3,0x1ffd,0x6,0x1ffe,0x1ffd,0x1ffd,0x9,0x1f8,0x1f9,0x13,0x1f8,0x8,0x1f5,0x1f5,0x1ff,0xa,0x1f5,0x9,0x1fb,0x0,0x1,0x1fe,0x1fe,0x3,0x1f9,0x7,0x1f7};
static	UINT32 LX_ADEMOD_DK_NICAM[] = {0x1f6,0x13,0x1e6,0x20,0x1e6,0x19,0x1da,0x11,0x1f8,0x1ff,0x1a,0x1da,0x24,0x1e4,0x14,0x1f0,0x5,0xa,0x1ee,0x1c,0x1fde,0x25,0x1ff0,0x0,0x9,0x1ff3,0x35,0x1fad,0x3f,0x1fd7,0x1a,0x1fff,0x1f91,0x1f1e,0x1f12,0x9c5,0x77c,0x1eda,0x1ce,0x5d,0x1f71,0x104,0x1f1f,0xb9,0x1f86,0x2e,0x11,0x1fd0,0x3a,0x1fc5,0x40,0x1fcd,0x18,0x1ffc,0x1ff8,0x14,0x1fe5,0x17,0x1ff0,0x9,0x1fa,0x1fb,0x1f,0x1df,0x23,0x1d3,0x16,0x1f7,0x1ff,0xb,0x1ed,0x1a,0x1e3,0x1d,0x1e6,0x11,0x1f7,0x1fd,0xa,0x1ef, };


//static	UINT32 LX_ADEMOD_BG[] = {0x1f5,0x11,0x1eb,0x17,0x1f0,0x7,0x1ea,0x0,0x9,0x1f8,0x16,0x1e8,0xf,0x1fa,0x0,0x1fd,0x1fa,0x10,0x1ea,0x13,0x1fe1,0x11,0x8,0x1fdf,0x1e,0x1ff0,0x1fef,0x1ff9,0x1fda,0x1ffa,0x1fd0,0x1a,0x1f96,0x1e00,0x1ec,0xacf,0x6b0,0x1d4c,0x207,0xe4,0x1e57,0x19b,0x1f3c,0x10,0x60,0x1f65,0x98,0x1f89,0x4b,0x1fe7,0x3,0x5,0x1fe9,0x23,0x1fde,0x1a,0x1fee,0x1fff,0x15,0x1fe2,0x14,0x1fb,0x8,0x7,0x1ea,0x8,0x1eb,0x16,0x1f0,0x5,0x4,0x1f3,0x11,0x1ec,0x11,0x1ef,0x9,0x1f9,0x1fc,0x9};  //(Ver. 1122)
//static	UINT32 LX_ADEMOD_BG[] = {0x1f6,0x10,0x1ed,0x16,0x1f1,0x7,0x1ec,0x0,0x9,0x1f9,0x15,0x1ea,0xe,0x1fb,0x0,0x1fd,0x1fc,0x10,0x1eb,0x16,0x1fe4,0xf,0x9,0x1fe1,0x1b,0x1ff1,0x1ff8,0x0,0x1fdb,0xe,0x1fe5,0x9,0x1fa6,0x1e4d,0x1db,0xa1a,0x655,0x1d9b,0x1fe,0xcb,0x1e7b,0x19f,0x1f47,0x14,0x6b,0x1f6e,0x8f,0x1f94,0x46,0x1fe9,0x5,0x7,0x1fed,0x20,0x1fe2,0x1a,0x1fed,0x0,0x14,0x1fe4,0x14,0x1fb,0x8,0x7,0x1eb,0x9,0x1ed,0x15,0x1f2,0x6,0x4,0x1f4,0x10,0x1ee,0x10,0x1f0,0x9,0x1fa,0x1fd,0x9};  //(Ver. 1122)



//old -- gi
//static UINT32 LX_ADEMOD_BG[] = {0x2d,0x1d8,0x1b,0x1fb,0x1f4,0x22,0x1be,0x39,0x1cd,0x26,0x1fc,0x1ec,0x20,0x1dc,0x26,0x1d8,0x1d,0x1f6,0x1ff,0x16,0x1fd9,0x29,0x1fe9,0xd,0x1ff9,0x1ff1,0x2,0x1fdd,0xe,0x1feb,0x1fff,0x1ff7,0x1fac,0x1e92,0x10a,0x89c,0x83e,0x1e74,0x44,0x283,0x1d92,0x1d2,0x1f78,0x1fb0,0xd4,0x1f1e,0xb3,0x1f9c,0x18,0x1c,0x1fda,0x1b,0x1ff3,0x1ff9,0x1b,0x1fe2,0xf,0x1ffd,0x1ff3,0x1a,0x1da,0x26,0x1f5,0x2,0x9,0x1d9,0x24,0x1d8,0x27,0x1e3,0xb,0xf,0x1da,0x34,0x1c4,0x33,0x1d9,0x12,0x1,0x1ec, }; 
//static UINT32 LX_ADEMOD_DK[] = {0x1ff,0x1ff,0x3,0x1fd,0x8,0x1f4,0x1f7,0x1ff,0x1ff,0x9,0x1fd,0x0,0x1f7,0x11,0x1ea,0x13,0x1ef,0xf,0x1fb,0x0,0x3,0x1ffb,0x13,0x1fe9,0x15,0x1ffa,0x11,0x1fde,0x7,0x25,0x1fc1,0x42,0x1f4a,0x1eed,0x13,0x938,0x7c6,0x1e26,0x22b,0x61,0x1f58,0x11e,0x1f22,0xa7,0x1f9f,0x27,0x6,0x1fd9,0x33,0x1fcd,0x34,0x1fd5,0x1d,0x1feb,0xd,0x1ffd,0x1ff9,0x9,0x1ff5,0xd,0x1ea,0x12,0x0,0x1,0x4,0x1ed,0x5,0x1fa,0x9,0x1f5,0xe,0x1f2,0xa,0x1f8,0x3,0x1fe,0x0,0x1fe,0x1,0x1fe, };
//static UINT32 LX_ADEMOD_I[] =  {0x25,0x1dd,0x1c,0x1f8,0x1fa,0x20,0x1d0,0x23,0x1da,0x1c,0x1fd,0x1fe,0xd,0x1eb,0x24,0x1de,0x19,0x1f2,0x1,0x10,0x1fe5,0x1a,0x1ff8,0xa,0xa,0x1fe0,0x2c,0x1fc9,0x18,0xa,0x1fc2,0x9e,0x1f37,0x135,0x1ebf,0x264,0xbc7,0x269,0x1eba,0x13d,0x1f2e,0xa7,0x1fb7,0x12,0x10,0x1fcd,0x2d,0x1fe0,0xd,0x8,0x1ff9,0x17,0x1fe7,0xe,0x1,0x1ff4,0x17,0x1fe2,0x20,0x1fee,0x9,0x0,0x1fb,0x1c,0x1dd,0x20,0x1d6,0x1a,0x1,0x1ef,0x25,0x1d2,0x2f,0x1d7,0x1c,0x1f6,0x1fb,0x16,0x1e0,0x24, }; 
//static UINT32 LX_ADEMOD_L[] =  {0x1fe,0x0,0x3,0x1fe,0x3,0x4,0x1f2,0x1fc,0x1ff,0x4,0x6,0x3,0x1f2,0xd,0x1f7,0x3,0x1fc,0x6,0x0,0x3,0x1ffb,0x1fff,0x3,0x8,0x1ff1,0xa,0x1fef,0x1a,0x1fda,0x22,0x1fea,0x2f,0x1fc1,0x6b,0x1f6d,0x22c,0xc2d,0x282,0x1ed2,0x12d,0x1efc,0xdb,0x1f6b,0x6e,0x1fbd,0x15,0x5,0x1fee,0x9,0x1ff8,0xd,0x1ffa,0x1fff,0x1,0x3,0x3,0x1fff,0x1fff,0x1ffc,0x7,0x1fa,0x1fb,0xe,0x1fd,0x5,0x1f6,0x1f7,0x1ff,0x7,0x1fa,0x5,0x1fd,0x0,0x0,0x1ff,0x1fe,0x1,0x1fc,0x3,0x1fb, }; 
//static UINT32 LX_ADEMOD_L1[] = {0x0,0x1fe,0x5,0x1fb,0x4,0x2,0x1f2,0x1f8,0x3,0x1ff,0xd,0x1fc,0x1f7,0x8,0x1fd,0x1fa,0x4,0x1fb,0xa,0x1fd,0x1fff,0x0,0x0,0x9,0x1fee,0x1,0x4,0x1ff8,0xc,0x1fec,0x6,0x2f,0x1f6f,0xf9,0x1e98,0x1db,0xe08,0x233,0x1dfc,0x1b9,0x1eaf,0xd0,0x1f96,0x28,0x1ffb,0x1fef,0x17,0x1ff2,0x1ff5,0xb,0x1ff9,0x8,0x1ff8,0x2,0x7,0x1fff,0x0,0x1fff,0x1ff8,0xd,0x1f3,0x0,0xa,0x2,0x1ff,0x1fc,0x1ee,0x6,0x1,0x1ff,0x1,0x1,0x1fd,0x3,0x1fb,0x2,0x1fc,0x0,0x0,0x1ff,   };
//static UINT32 LX_ADEMOD_DK_NICAM[] = {0x1f6,0x13,0x1e6,0x20,0x1e6,0x19,0x1da,0x11,0x1f8,0x1ff,0x1a,0x1da,0x24,0x1e4,0x14,0x1f0,0x5,0xa,0x1ee,0x1c,0x1fde,0x25,0x1ff0,0x0,0x9,0x1ff3,0x35,0x1fad,0x3f,0x1fd7,0x1a,0x1fff,0x1f91,0x1f1e,0x1f12,0x9c5,0x77c,0x1eda,0x1ce,0x5d,0x1f71,0x104,0x1f1f,0xb9,0x1f86,0x2e,0x11,0x1fd0,0x3a,0x1fc5,0x40,0x1fcd,0x18,0x1ffc,0x1ff8,0x14,0x1fe5,0x17,0x1ff0,0x9,0x1fa,0x1fb,0x1f,0x1df,0x23,0x1d3,0x16,0x1f7,0x1ff,0xb,0x1ed,0x1a,0x1e3,0x1d,0x1e6,0x11,0x1f7,0x1fd,0xa,0x1ef, };
   


void ADEMOD_SetGroupDelay(Handle_t ADEMOD_h, LX_ADEMOD_VideoStandard videoStd )
{


    LX_ADEMOD_Result status = retOK; /*  Status to be returned.  */
    pLX_ADEMOD_Demod_Instance pInstance = NULL;

	//int i;
	//UINT32 addr = 0x80;
	//UINT32 *_ADEMOD_group_delay = NULL;
	UINT32 *_abb_group_delay = NULL;
#if 0
	//Ver. C38 20091121/ LGIT ������
	//����: �𵨺� Groupdelay �� ����
	//�������: ��� Ʃ��
	//*�⺻ Groupdelay �� ����(TDTR, TDFR, TDTD)
	UINT32 TDTD_BG[] = { 0x0,0x5,0x1fe,0x5,0x0,0x4,0x1fa,0x9,0x1fa,0x1f3,0x4,0x1fb,0x4,0x2,0x3,0x8,0x1fd,0x1f5,0x1ff,0x0,0x1ffb,0x1ffc,0x1,0x1ff9,0x4,0xc,0x1ff4,0xd,0xd,0x1ff7,0x13,0x1fe7,0xe,0x0,0x1fdd,0x1fc0,0x1eb6,0xa5,0x8f2,0x724,0x1ebf,0x13b,0x137,0x1ef7,0xb9,0x1e,0x1f7a,0xab,0x1f8d,0x4c,0x5,0x1fcc,0x4d,0x1fc6,0x2b,0x1fe4,0xb,0x1ff7,0x2,0x1ffb,0x1f4,0x1,0x4,0x0,0x3,0x3,0x1,0x1fc,0x1f8,0x1fc,0x5,0x1fa,0x1fe,0x9,0x1fc,0x3,0x7,0x1fa,0x5,0x1fc, };
	UINT32 TDTD_I[] = { 0x7,0x1fd,0x5,0x0,0x1fc,0x7,0x1f7,0x6,0x1fd,0x1f5,0x1fb,0x1fb,0x2,0x6,0x0,0x1fc,0x1,0x1,0x1f6,0x7,0x1fff,0x1ffb,0x9,0x1ff7,0x1ffe,0x11,0x1ff5,0x8,0x1ff4,0x25,0x1fe8,0x2f,0x1fe0,0x30,0x17,0x1f7d,0x127,0x1ea3,0x317,0xa6c,0x381,0x1e73,0x121,0x1f94,0x1ffe,0x41,0x1fd5,0x34,0x1fe6,0x25,0x1ff6,0x7,0x1ff5,0x10,0x0,0x1ff6,0x9,0x1ffb,0x1ffe,0x7,0x1f5,0x0,0x2,0x1fb,0x0,0x7,0x2,0x1fc,0x1fa,0x1f6,0x1fc,0x6,0x1f8,0x5,0x1ff,0x1fd,0x8,0x1fa,0x8,0x1fc, };
	UINT32 TDTD_DK[] = { 0x1fd,0x1fb,0x1,0x1fb,0x8,0x1fa,0x6,0x7,0x1f4,0x1fe,0x1fc,0x0,0xa,0x2,0x1fd,0x3,0x4,0x1f9,0x1fe,0x2,0x1ffa,0x5,0x1ffa,0x1ffe,0xe,0x1ffd,0xc,0x1,0x5,0xd,0xc,0x1fe6,0x22,0x19,0x1fa7,0x94,0x1da5,0xd6,0x9b7,0x4da,0x1fff,0x1fc,0x1ffc,0x6,0x76,0x1fb4,0x3f,0x1fe9,0x10,0x1ffd,0x8,0x6,0x9,0x9,0x1ff6,0xa,0x1ff9,0x0,0x2,0x1ff8,0x4,0x2,0x1fb,0x0,0x8,0x6,0x0,0x1,0x1f5,0x1ff,0x8,0x1fa,0x6,0x2,0x1fd,0x1,0x1f7,0x6,0x1fb,0x1fe, };
	UINT32 TDTD_M[] = { 0x2,0x1fe,0x3,0x1,0x0,0x6,0x1f4,0x1f9,0x2,0x1ff,0xb,0x1fe,0x1fc,0x2,0x3,0x1fd,0x1fa,0x1ff,0x1fe,0x8,0x1ffc,0x1ff5,0xc,0x1ffc,0x1ff5,0x1fff,0xb,0x1ffe,0x1fe9,0x21,0x8,0x1fe0,0x49,0x62,0x1eb4,0x2f3,0xb43,0x412,0x1d33,0x159,0x54,0x1f0f,0xfe,0x1f8b,0x1,0x31,0x1fcc,0x2d,0x1fd3,0x1d,0x1fed,0xc,0x1ff1,0x9,0x1,0x1ffa,0x0,0x1ff9,0x3,0x7,0x1f6,0x4,0x5,0x5,0x1fc,0x1fb,0x1f6,0x1ff,0x7,0x1fc,0x3,0x3,0x1fa,0x6,0x1fa,0x1,0x0,0x1fd,0x2,0x1fd,};
	UINT32 TDTD_L[] = { 0xd,0x1db,0x2a,0x1d4,0x2b,0x1dc,0x16,0x4,0x1f3,0x15,0x1db,0x24,0x1f0,0x9,0x17,0x1dc,0x26,0x1de,0x1fe,0xb,0x1fdd,0x2b,0x1fe4,0x9,0x5,0x1fe0,0x4f,0x1f99,0x70,0x1fb5,0x35,0x1c,0x1f8d,0xd1,0x1f55,0xd,0x16e,0x1d71,0x3d8,0xb26,0x2d6,0x1ddd,0x1a5,0x1fae,0x1fa4,0xa9,0x1f92,0x26,0x2a,0x1fbd,0x65,0x1fab,0x36,0x8,0x1fd3,0x3c,0x1fb8,0x4b,0x1fc9,0x14,0x1fe,0x1dc,0x29,0x1df,0xe,0x18,0x1de,0x35,0x1cd,0x1b,0x1f5,0x1f8,0x2a,0x1c5,0x46,0x1bc,0x3d,0x1ca,0x1c,0x1f0,};

	//*Ư���� ��� Groupdelay �� ����(TDVJ, TDTJ)
	UINT32 TDTJ_BG[] = { 0x1fc,0x5,0x1f9,0x14,0x1ea,0x1c,0x1e7,0x1a,0x1f1,0x2,0x1f6,0x1fb,0xe,0x1f9,0x13,0x1f1,0x1f7,0x17,0x1d9,0x22,0x1fde,0x15,0x1ff7,0x1ff3,0x1ff6,0x1ff2,0x4,0x0,0x1fe6,0x33,0x1fa9,0x4c,0x1fc1,0x9,0x1fde,0x1ff7,0x1b,0x1e06,0xbe,0x920,0x739,0x1e81,0xc7,0x271,0x1dcf,0x1ec,0x1f33,0x57,0x1ffb,0x7,0xe,0x1ff6,0x1,0x1ffa,0x2a,0x1fc9,0x2c,0x1fe5,0x5,0xd,0x1e4,0x1b,0x1ee,0x1f4,0x0,0x1fd,0x28,0x1e5,0x19,0x1f2,0x1f4,0x17,0x1eb,0x13,0x1f5,0x9,0x1fc,0x1ff,0x0,0x1ff, };
	UINT32 TDTJ_BG_UHF[] = { 0x3,0x5,0x0,0x3,0x1,0x2,0x1fc,0x8,0x1f8,0x1f0,0x7,0x1fa,0x5,0x7,0x1f6,0x1c,0x1e6,0xc,0x1e9,0xd,0x1ff3,0x1fff,0xf,0x1fd9,0x31,0x1fd9,0x21,0x1fd7,0x30,0x1fd1,0x1a,0x1fdb,0x0,0x7,0x1fba,0x1fad,0x1dd9,0x164,0xa53,0x6b9,0x1e7e,0x1b5,0xce,0x1ee6,0xfe,0x1fa5,0x1fd6,0x5c,0x1fa6,0x45,0x1fe6,0x1fe7,0x30,0x1fdd,0xe,0x1ffe,0x1ff8,0x8,0x1ff2,0x4,0x1ee,0x4,0x7,0x1fa,0x9,0x1,0x3,0x1fb,0x1f6,0x1fd,0x3,0x1fa,0x1fe,0xa,0x1f7,0x8,0x4,0x1fe,0x5,0x1fc, };
	UINT32 TDTJ_I[] = { 0x2,0x1f7,0x12,0x1f2,0x14,0x1ed,0x14,0x1f0,0xd,0x1ee,0x1f9,0xe,0x1ed,0x1f,0x1ef,0x1fc,0x13,0x1e0,0x23,0x1d6,0x1e,0x1fea,0x1fff,0x8,0x1fe1,0x1f,0x1fe7,0x1ff8,0x0,0x0,0x0,0x0,0x1fef,0x47,0x1faa,0x4d,0xa9,0x1e8f,0x309,0xc04,0x1a2,0x1f57,0x7e,0x52,0x1fa5,0x5e,0x1fe1,0x1,0x1ffc,0xb,0x1ff3,0x8,0x1fda,0x35,0x1fc6,0x28,0x1fe2,0xa,0x6,0x1feb,0x19,0x1e8,0x14,0x1,0x1ed,0x26,0x1dc,0x23,0x1e2,0x3,0x1fc,0x1ff,0x9,0x1f5,0x11,0x1f2,0xb,0x1f5,0x7,0x1fe, };
	UINT32 TDTJ_DK[] = { 0x1ff,0x1ff,0x7,0x9,0x1f3,0x15,0x1e9,0x1d,0x1e8,0x11,0x1e1,0x12,0x1f8,0xf,0x15,0x1d9,0xd,0x1fc,0x1f3,0xe,0x1fe0,0x25,0x1fd2,0x26,0x1fd5,0x17,0x1ff8,0x1ff7,0x4,0x0,0x0,0x0,0x0,0x2e,0x1fc1,0x1fed,0x71,0x1e0a,0x1fa0,0x977,0x76d,0x1e4c,0x261,0x1ff3,0x63,0x1fef,0x71,0x1fa7,0x4d,0x1fd7,0x28,0x1fea,0x1,0x21,0x1ffc,0x1fe4,0x2b,0x1fca,0x33,0x1fd6,0x18,0x1f3,0x0,0x7,0x1e5,0x1a,0x1ff,0x4,0xe,0x1e6,0x11,0x1f0,0x12,0x1f1,0x11,0x1f6,0x8,0x1f9,0x2,0x1ff, };
	UINT32 TDTJ_L[] = { 0xd,0x1db,0x2a,0x1d4,0x2b,0x1dc,0x16,0x4,0x1f3,0x15,0x1db,0x24,0x1f0,0x9,0x17,0x1dc,0x26,0x1de,0x1fe,0xb,0x1fdd,0x2b,0x1fe4,0x9,0x5,0x1fe0,0x4f,0x1f99,0x70,0x1fb5,0x35,0x1c,0x1f8d,0xd1,0x1f55,0xd,0x16e,0x1d71,0x3d8,0xb26,0x2d6,0x1ddd,0x1a5,0x1fae,0x1fa4,0xa9,0x1f92,0x26,0x2a,0x1fbd,0x65,0x1fab,0x36,0x8,0x1fd3,0x3c,0x1fb8,0x4b,0x1fc9,0x14,0x1fe,0x1dc,0x29,0x1df,0xe,0x18,0x1de,0x35,0x1cd,0x1b,0x1f5,0x1f8,0x2a,0x1c5,0x46,0x1bc,0x3d,0x1ca,0x1c,0x1f0,};
	UINT32 TDVJ_M[] = {0x4,0x0,0x3,0x6,0x1,0xb,0x1f1,0x1,0x1fa,0x7,0x6,0x1fb,0x1f9,0x2,0x1,0x1f5,0x4,0x1f3,0x3,0x1f8,0x1ffd,0x1ffd,0x2,0x5,0x1fea,0x14,0x1ff8,0x1ffa,0x1d,0x1fda,0x33,0x1fd7,0x15,0x1d,0x1f64,0x413,0x977,0x397,0x1e37,0x1da,0x1efb,0x3e,0x41,0x1fc3,0x11,0x1c,0x1fe9,0x1ffd,0x1e,0x1fc9,0x39,0x1fd4,0x14,0x1ff4,0x1ffb,0x1,0x1ff7,0x1ffc,0x2,0x1ffd,0x0,0x1f2,0x14,0x1f9,0x5,0x1f3,0x1ff,0x1ff,0xa,0x0,0x3,0x5,0x1fb,0xb,0x1fa,0xb,0x1fd,0x5,0x0,0x5,};
	//*091125 �̳��� ��û
	//Ver. C40 20091123 / LGIT �ϻ�ä, ������
	//����: TDFR BG,I,DK ȭ�� Ư��
	//����: Groupdelay ���� (11�� 22�� ���� Lot ���� ���)
	//�������: TDFR-G035D, TDFR-G135D, TDFR-C035D, TDFR-C135D
	UINT32 TDFR_BG[] = { 0x1fc,0x5,0x1f9,0x14,0x1ea,0x1c,0x1e7,0x1a,0x1f1,0x2,0x1f6,0x1fb,0xe,0x1f9,0x13,0x1f1,0x1f7,0x17,0x1d9,0x22,0x1fde,0x15,0x1ff7,0x1ff3,0x1ff6,0x1ff2,0x4,0x0,0x1fe6,0x33,0x1fa9,0x4c,0x1fc1,0xf,0x1fd6,0x1fed,0x32,0x1e09,0x9e,0x91d,0x797,0x1dfd,0xfc,0x2c7,0x1d21,0x29c,0x1eb1,0xa3,0x1fdf,0x2,0x22,0x1fe8,0x1,0x1ffa,0x2a,0x1fc9,0x2c,0x1fe5,0x5,0xd,0x1e4,0x1b,0x1ee,0x1f4,0x0,0x1fd,0x28,0x1e5,0x19,0x1f2,0x1f4,0x17,0x1eb,0x13,0x1f5,0x9,0x1fc,0x1ff,0x0,0x1ff, };
	UINT32 TDFR_I[] = { 0x2,0x1f7,0x12,0x1f2,0x14,0x1ed,0x14,0x1f0,0xd,0x1ee,0x1f9,0xe,0x1ed,0x1f,0x1ef,0x1fc,0x13,0x1e0,0x23,0x1d6,0x1e,0x1fea,0x1fff,0x8,0x1fe1,0x1f,0x1fe7,0x1ff8,0x0,0x0,0x0,0x0,0x1ff3,0x42,0x1fb5,0x2b,0xd2,0x1ead,0x268,0xcf8,0xda,0x1fa6,0x85,0x41,0x1fa5,0x64,0x1fd7,0x13,0x1fe5,0x1e,0x1fec,0x8,0x1fda,0x35,0x1fc6,0x28,0x1fe2,0xa,0x6,0x1feb,0x19,0x1e8,0x14,0x1,0x1ed,0x26,0x1dc,0x23,0x1e2,0x3,0x1fc,0x1ff,0x9,0x1f5,0x11,0x1f2,0xb,0x1f5,0x7,0x1fe, };
	UINT32 TDFR_DK[] = { 0x1ff,0x1ff,0x7,0x9,0x1f3,0x15,0x1e9,0x1d,0x1e8,0x11,0x1e1,0x12,0x1f8,0xf,0x15,0x1d9,0xd,0x1fc,0x1f3,0xe,0x1fe0,0x25,0x1fd2,0x26,0x1fd5,0x17,0x1ff8,0x1ff7,0x4,0x0,0x0,0x0,0x0,0x1c,0x1fec,0x1fb0,0x91,0x1e49,0x1f1c,0x9bc,0x7e3,0x1d38,0x380,0x1f43,0x9c,0x1ff4,0x64,0x1fa3,0x69,0x1faf,0x4a,0x1fda,0x1,0x21,0x1ffc,0x1fe4,0x2b,0x1fca,0x33,0x1fd6,0x18,0x1f3,0x0,0x7,0x1e5,0x1a,0x1ff,0x4,0xe,0x1e6,0x11,0x1f0,0x12,0x1f1,0x11,0x1f6,0x8,0x1f9,0x2,0x1ff, };
	UINT32 TDFR_LP[] = {0x6,0x1ef,0x1b,0x1e0,0x28,0x1d8,0x21,0x1ec,0x13,0x1e4,0xb,0x1f6,0x8,0x4,0x4,0x1f9,0x1,0xe,0x1f2,0x8,0x0,0x1ff9,0xe,0x1fe6,0x1d,0x1fe1,0x38,0x1fa5,0x58,0x1fb1,0x24,0xd,0x1fbf,0x54,0x27,0x1f21,0x22b,0x1d0e,0x4c3,0x9cd,0x3a5,0x1dcf,0x1d2,0x1f70,0x1fc4,0xe0,0x1f18,0xba,0x1f88,0x33,0x1ff2,0x1ff7,0x1fed,0x31,0x1fc6,0x40,0x1fb8,0x45,0x1fc2,0x36,0x1d2,0x25,0x1f1,0x3,0x1fa,0x12,0x1f5,0xe,0x1ef,0x2,0x1f6,0x9,0x5,0x1ef,0x18,0x1eb,0x14,0x1f3,0x5,0x2, };
#endif
	//100218 �ε� field ���� ����.
	//Ver C67 20100212 LGE �̰�Ź LGIT ������
	//����: �ε� field issue ����(���� noise, red noise)
	//���� : Groupdelay,Register ���� �� ����.
	//������� : TDTJ �ε��� only
	//���� ���� : Group delay����, Register ����.
//	UINT32 TDTJ_INDIA[]={ 0x1fc,0x5,0x1f9,0x14,0x1ea,0x1c,0x1e7,0x1a,0x1f1,0x2,0x1f6,0x1fb,0xe,0x1f9,0x13,0x1f1,0x1f7,0x17,0x1d9,0x22,0x1fde,0x15,0x1ff7,0x1ff3,0x1ff6,0x1ff2,0x4,0x0,0x1fe6,0x33,0x1fa9,0x4c,0x1fc1,0x15,0x1fc6,0xe,0x19,0x1de7,0xe1,0x927,0x6f2,0x1ee1,0x9e,0x230,0x1e6b,0x138,0x1fbb,0x14,0x4,0x1c,0x1ff5,0x3,0x1,0x1ffa,0x2a,0x1fc9,0x2c,0x1fe5,0x5,0xd,0x1e4,0x1b,0x1ee,0x1f4,0x0,0x1fd,0x28,0x1e5,0x19,0x1f2,0x1f4,0x17,0x1eb,0x13,0x1f5,0x9,0x1fc,0x1ff,0x0,0x1ff, };

	if ((status = ADEMOD_GetContext(ADEMOD_h, &pInstance)) != retOK)
	{
		return;
	}

	switch(videoStd)
	{
		case LX_ADEMOD_VIDEO_STANDARD_PAL_B:	// BG
//			memcpy((void*)_abb_group_delay, (void*)LX_ADEMOD_BG, sizeof(LX_ADEMOD_BG));
			_abb_group_delay = LX_ADEMOD_BG;
			break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_I:	  // I
//			memcpy((void*)_abb_group_delay, (void*)LX_ADEMOD_I, sizeof(LX_ADEMOD_I));
			_abb_group_delay = LX_ADEMOD_I;
			break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_D:	// DK
//			memcpy((void*)_abb_group_delay, (void*)LX_ADEMOD_DK, sizeof(LX_ADEMOD_DK));
			_abb_group_delay = LX_ADEMOD_DK;
			break;

		case LX_ADEMOD_VIDEO_STANDARD_SECAM_L:   // L
			_abb_group_delay = LX_ADEMOD_L;
			break;
		case LX_ADEMOD_VIDEO_STANDARD_SECAM_L1:   // LL
//			memcpy((void*)_abb_group_delay, (void*)LX_ADEMOD_L, sizeof(LX_ADEMOD_L));
			_abb_group_delay = LX_ADEMOD_L1;
			break;

		case LX_ADEMOD_VIDEO_STANDARD_NTSC_M:
//			memcpy((void*)_abb_group_delay, (void*)LX_ADEMOD_M, sizeof(LX_ADEMOD_M));
			_abb_group_delay = LX_ADEMOD_M;
			break;

		case LX_ADEMOD_VIDEO_STANDARD_PAL_DK_NICAM:
//			memcpy((void*)_abb_group_delay, (void*)LX_ADEMOD_DK_NICAM, sizeof(LX_ADEMOD_DK_NICAM));
			_abb_group_delay = LX_ADEMOD_DK_NICAM;
			break;
		default:
//			memcpy((void*)_abb_group_delay, (void*)LX_ADEMOD_BG, sizeof(LX_ADEMOD_BG));
			_abb_group_delay = LX_ADEMOD_BG;
			break;
	}

	ADEMOD_Parameter_Set(ADEMOD_h,LX_ADEMOD_PARM_GRP_DEL,_abb_group_delay,TRUE);

}

#if defined( __cplusplus )
}
#endif


