#ifdef MODULE
#define __NO_VERSION__
#include <linux/module.h>
#include <linux/version.h>
#ifdef MODVERSIONS
#include <linux/modversions.h>
#endif
#endif

#include <linux/param.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/fcntl.h>
#include <linux/sched.h>
#include <linux/ctype.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <asm/dma.h>
#include <linux/wait.h>
#include <linux/malloc.h>
#include <linux/vmalloc.h>
#include <linux/string.h>
#include <linux/ioport.h>
#include <linux/utsname.h>

#include <linux/wrapper.h>

#include <linux/soundcard.h>

#define FALSE	0
#define TRUE	1

struct snd_wait {
	  int opts;
	};

extern int sound_alloc_dma(int chn, char *deviceID);
extern int sound_open_dma(int chn, char *deviceID);
extern void sound_free_dma(int chn);
extern void sound_close_dma(int chn);

extern caddr_t sound_mem_blocks[1024];
extern int sound_nblocks;
