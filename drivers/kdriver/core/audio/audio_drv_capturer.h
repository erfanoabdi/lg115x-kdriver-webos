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



/** @file audio_drv_capturer.h
 *
 *  driver interface header for audio device. ( used only within kdriver )
 *	audio device will teach you how to make device driver with new platform.
 *
 *  @author	wonchang.shin (wonchang.shin@lge.com)
 *  @version	0.1
 *  @date		2012.04.25
 *
 *  @addtogroup lg1150_audio
 *	@{
 */

#ifndef	_AUDIO_DRV_CAPTURER_H_
#define	_AUDIO_DRV_CAPTURER_H_

#include "audio_kapi_capturer.h"

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*----------------------------------------------------------------------------------------
	Control Constants
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    File Inclusions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Constant Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
	Macro Definitions
----------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------
    Type Definitions
----------------------------------------------------------------------------------------*/
/**
 * AUDIO Capturer Information.
 */
typedef struct {
	UINT32								ui32AllocDev;
	LX_AUD_CAPTURE_PARAM_T				capParam;
	AUD_CONNECT_T						captureConnect;	// For Reset Recovery
	UINT32								bStarted;
	UINT32								bClosed;
	UINT32								ui32Volume;		// volume
	UINT32								bResetting;		// For Reset Recovery
} AUD_CAPTURER_INFO_T;

/*----------------------------------------------------------------------------------------
	Extern Function Prototype Declaration
----------------------------------------------------------------------------------------*/
SINT32	KDRV_AUDIO_OpenCapture(struct inode *inode, struct file *filp);
SINT32	KDRV_AUDIO_CloseCapture(struct inode *inode, struct file *filp);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,36)
int KDRV_AUDIO_IoctlCapture(struct inode *, struct file *, unsigned int, unsigned long);
#else
long KDRV_AUDIO_IoctlCapture(struct file *filp, unsigned int cmd, unsigned long arg);
#endif
void	KDRV_AUDIO_ResetCapturer(void);
void	KDRV_AUDIO_ResetCapturerParam(void);
void	KDRV_AUDIO_RestartCapturer(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _AUDIO_DRV_CAPTURER_H_ */

