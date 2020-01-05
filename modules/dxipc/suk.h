#ifndef _VDEC_SUK_H
#define _VDEC_SUK_H

#ifdef CHIP_NAME_d14
int suk_start (void *v);
int suk_stop (void);
int suk_flush (void);
#else
#define suk_start(v)	do{}while(0)
#define suk_stop()	do{}while(0)
#define suk_flush()	do{}while(0)
#endif

#endif
