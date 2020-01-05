
#include <linux/kernel.h>
#include <linux/workqueue.h>

#include "hal/vdec_base.h"
#include "hal/pdec_hal_api.h"
#include "include/vdec_kapi.h"
#include "ves/ves_drv.h"
#include "ves/ves_auib.h"
#include "decoder.h"
#include "log.h"
#include "sc_if.h"
#include "dxipc_drv.h"

logm_define (suk, log_level_warning);

#define INTERVAL	(HZ/30)
#define cpb_size	0x1000000

static unsigned long cpb;
static void *cpb_mapped;
static int started = 0;
static int ves;
static void *vdec;


static int decrypt (int rd, int wr)
{
	rd -= cpb;
	wr -= cpb;

	while (1)
	{
		LX_VDEC_CPB_INFO_T cpb_info;
		LX_VDEC_IO_UPDATE_BUFFER_T update_buffer;
		SC_IF_AU_T au;
		SC_IF_CPB_T in, out;
		SC_IF_RETURN_T ret;

		vdec_ioctl (vdec, LX_VDEC_GET_CPB_INFO, (unsigned long)&cpb_info, 0);
		logm_debug (suk, "cpb  read 0x%08x, write 0x%08x\n",
				cpb_info.read_offset, cpb_info.write_offset);


		memset (&in, 0, sizeof (in));
		in.pBuffer = cpb_mapped;
		in.size = cpb_size;
		in.readOffset = rd;
		in.writeOffset = wr;

		memset (&out, 0, sizeof (out));
		out.pBuffer = (void*)(cpb_info.addr + 0x60000000);
		out.size = cpb_info.size;
		out.readOffset = cpb_info.read_offset;
		out.writeOffset = cpb_info.write_offset;
		out.emptyAUIBEntry = VES_AUIB_NumFree(0);

		memset (&au, 0, sizeof (au));

		ret = SC_IF_DecryptContent (&au, &in, &out, 0);
		logm_debug (suk, "DecryptContent() %d\n",
				ret);

		logm_debug (suk, "pdec read 0x%08x\n",
				in.readOffset);
		rd = in.readOffset;
		PDEC_HAL_CPB_SetRdPtr (0, rd + cpb);

		switch (ret)
		{
		case SC_IF_OK:
			logm_debug (suk, "got AU.offset 0x%08x, size 0x%08x, pts %lld\n",
					au.offset, au.length, au.pts);

			memset (&update_buffer, 0, sizeof (update_buffer));
			update_buffer.au_type = LX_VAU_DATA;
			update_buffer.au_ptr = (void*)(cpb_info.addr + au.offset);
			update_buffer.au_size = au.length;
			update_buffer.timestamp = au.pts * 1000000LL;

			vdec_ioctl (vdec, LX_VDEC_IO_UPDATE_BUFFER,
					(unsigned long)&update_buffer, 0);
			continue;

		default:
		case SC_IF_ERROR:
			// fallthrough

		case SC_IF_NO_AU:
		case SC_IF_WAIT:
			break;
		}

		break;
	}

	return 0;
}


static void check_pdec (struct work_struct *work);
static DECLARE_DELAYED_WORK (check_pdec_work, check_pdec);

static void check_pdec (struct work_struct *work)
{
	int rd, wr;
	int size, margin;

	logm_debug (suk, "check.\n");

	rd = PDEC_HAL_CPB_GetRdPtr (0);
	wr = PDEC_HAL_CPB_GetWrPtr (0);
	size = wr - rd;
	if (size < 0)
		size += cpb_size;

	/* update CPB size IPC register */
	margin = VDEC_CPB_MARGIN - size;
	if (margin < 0)
		margin = 0;
	DX_IPC_HAL_SetReg (DX_REG_CPB_INFO, margin);


	/* decrypt */
	if (size > 0)
		decrypt (rd, wr);


	logm_debug (suk, "pdec read 0x%08x, write 0x%08x, "
			"size %08x, margin %08x\n",
			rd-cpb, wr-cpb, size, margin);

	if (started)
		queue_delayed_work (system_wq, &check_pdec_work, INTERVAL);
}

int suk_start (void *v)
{
	logm_noti (suk, "start\n");

	PDEC_HAL_SetBypass (0);

    if(started)
        return 0;
    

	/* open pdec */
	cpb = vdec_alloc (cpb_size, 1<<12, "tvp_cpb");
	if (!cpb)
	{
		logm_warning (suk, "vdec_alloc failed\n");
		goto error;
	}
	cpb_mapped = vdec_remap (cpb, cpb_size);
	logm_noti (suk, "cpb 0x%08x, cpb_mapped %p\n", cpb, cpb_mapped);
	ves = VES_Open (VES_SRC_SDEC0, 0, cpb, cpb_size, NULL, NULL);
	if (ves == 0xff)
	{
		logm_warning (suk, "ves open failed\n");
		goto error;
	}
	VES_Start (ves);

	/* set timer */
	SC_IF_DecryptContent (NULL, NULL, NULL, 1);
	queue_delayed_work (system_wq, &check_pdec_work, 0);

	vdec = v;
	started = 1;

	return 0;

error:
	if (ves != 0xff)
	{
		VES_Close (ves);
		ves = 0xff;
	}
	if (cpb)
	{
		vdec_free (cpb);
		cpb = 0;
	}

	return -1;
}

int suk_flush (void)
{
	logm_noti (suk, "flush..\n");

	PDEC_HAL_UnsetBypass (0);

	SC_IF_DecryptContent (NULL, NULL, NULL, 1);

	return 0;
}

int suk_stop (void)
{
	logm_noti (suk, "stop\n");

	if (started == 0)
	{
		logm_warning (suk, "not working\n");
		return 0;
	}

	started = 0;

	cancel_delayed_work_sync (&check_pdec_work);

	VES_Stop (ves);
	VES_Close (ves);
	PDEC_HAL_UnsetBypass (0);
    PDEC_HAL_Reset(0);
	vdec_unmap (cpb_mapped);
	vdec_free (cpb);

	return 0;
}

