#include <linux/config.h>
#include <linux/module.h>
#include <linux/linkage.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/user.h>
#include <linux/elfcore.h>
#include <linux/in6.h>
#include <linux/interrupt.h>
#include <linux/pci.h>

#include <asm/setup.h>
#include <asm/machdep.h>
#include <asm/pgtable.h>
#include <asm/irq.h>
#include <asm/semaphore.h>
#include <asm/checksum.h>
#include <asm/hardirq.h>
#include <asm/softirq.h>

asmlinkage long long __ashrdi3 (long long, int);
extern char m68k_debug_device[];

extern void dump_thread(struct pt_regs *, struct user *);
extern int dump_fpu(elf_fpregset_t *);

/* platform dependent support */

EXPORT_SYMBOL(m68k_machtype);
EXPORT_SYMBOL(m68k_cputype);
EXPORT_SYMBOL(m68k_is040or060);
EXPORT_SYMBOL(cache_push);
EXPORT_SYMBOL(cache_clear);
EXPORT_SYMBOL(mm_vtop);
EXPORT_SYMBOL(mm_ptov);
EXPORT_SYMBOL(mm_end_of_chunk);
EXPORT_SYMBOL(kernel_map);
EXPORT_SYMBOL(m68k_debug_device);
EXPORT_SYMBOL(dump_fpu);
EXPORT_SYMBOL(dump_thread);
EXPORT_SYMBOL(strnlen);
EXPORT_SYMBOL(strrchr);
EXPORT_SYMBOL(strstr);
EXPORT_SYMBOL(local_irq_count);
EXPORT_SYMBOL(local_bh_count);

/* Networking helper routines. */
EXPORT_SYMBOL(csum_partial_copy);

/* The following are special because they're not called
   explicitly (the C compiler generates them).  Fortunately,
   their interface isn't gonna change any time soon now, so
   it's OK to leave it out of version control.  */
EXPORT_SYMBOL_NOVERS(__ashrdi3);
EXPORT_SYMBOL_NOVERS(memcpy);
EXPORT_SYMBOL_NOVERS(memset);
EXPORT_SYMBOL_NOVERS(memcmp);

EXPORT_SYMBOL_NOVERS(__down_failed);
EXPORT_SYMBOL_NOVERS(__down_failed_interruptible);
EXPORT_SYMBOL_NOVERS(__up_wakeup);

#ifdef CONFIG_PCI
EXPORT_SYMBOL(pci_devices);
#endif
