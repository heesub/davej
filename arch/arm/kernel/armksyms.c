#include <linux/config.h>
#include <linux/module.h>
#include <linux/user.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/in6.h>

#include <asm/elf.h>
#include <asm/io.h>
#include <asm/dma.h>
#include <asm/pgtable.h>
#include <asm/semaphore.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <asm/checksum.h>

extern void dump_thread(struct pt_regs *, struct user *);
extern int dump_fpu(struct pt_regs *, struct user_fp_struct *);
extern void inswb(unsigned int port, void *to, int len);
extern void outswb(unsigned int port, const void *to, int len);

extern unsigned int local_bh_count[NR_CPUS];
extern unsigned int local_irq_count[NR_CPUS];

extern void * __ioremap(unsigned long phys_addr, unsigned long size, unsigned long flags);
extern void iounmap(void *addr);

extern pid_t kernel_thread(int (*fn)(void *), void *arg, unsigned long flags);

/*
 * syscalls
 */
extern int sys_write(int, const char *, int);
extern int sys_read(int, char *, int);
extern int sys_lseek(int, off_t, int);
extern int sys_open(const char *, int, int);
extern int sys_exit(int);
extern int sys_wait4(int, int *, int, struct rusage *);

/*
 * libgcc functions - functions that are used internally by the
 * compiler...  (prototypes are not correct though, but that
 * doesn't really matter since they're not versioned).
 */
extern void __gcc_bcmp(void);
extern void __ashldi3(void);
extern void __ashrdi3(void);
extern void __cmpdi2(void);
extern void __divdi3(void);
extern void __divsi3(void);
extern void __lshrdi3(void);
extern void __moddi3(void);
extern void __modsi3(void);
extern void __muldi3(void);
extern void __negdi2(void);
extern void __ucmpdi2(void);
extern void __udivdi3(void);
extern void __udivmoddi4(void);
extern void __udivsi3(void);
extern void __umoddi3(void);
extern void __umodsi3(void);

extern void ret_from_exception(void);
extern void fpundefinstr(void);
extern void fp_enter(void);
#define EXPORT_SYMBOL_ALIAS(sym,orig) \
 const char __kstrtab_##sym##[] __attribute__((section(".kstrtab"))) = \
    __MODULE_STRING(##sym##); \
 const struct module_symbol __ksymtab_##sym __attribute__((section("__ksymtab"))) = \
    { (unsigned long)&##orig, __kstrtab_##sym };
	/*
	 * floating point math emulator support.
	 * These symbols will never change their calling convention...
	 */
EXPORT_SYMBOL_ALIAS(kern_fp_enter,fp_enter);
EXPORT_SYMBOL_ALIAS(fp_printk,printk);
EXPORT_SYMBOL_ALIAS(fp_send_sig,send_sig);

#ifdef CONFIG_CPU_26
EXPORT_SYMBOL(fpundefinstr);
EXPORT_SYMBOL(ret_from_exception);
#endif

	/* platform dependent support */
EXPORT_SYMBOL(dump_thread);
EXPORT_SYMBOL(dump_fpu);
EXPORT_SYMBOL(udelay);
EXPORT_SYMBOL(xchg_str);
EXPORT_SYMBOL(local_bh_count);
EXPORT_SYMBOL(local_irq_count);
#ifdef CONFIG_CPU_32
EXPORT_SYMBOL(__ioremap);
EXPORT_SYMBOL(iounmap);
#endif
EXPORT_SYMBOL(kernel_thread);

EXPORT_SYMBOL(enable_irq);
EXPORT_SYMBOL(disable_irq);

	/* processor dependencies */
EXPORT_SYMBOL(processor);
EXPORT_SYMBOL(__machine_arch_type);

	/* networking */
EXPORT_SYMBOL(csum_partial_copy);
EXPORT_SYMBOL(__csum_ipv6_magic);

	/* io */
EXPORT_SYMBOL(outsb);
EXPORT_SYMBOL(outsw);
EXPORT_SYMBOL(outsl);
EXPORT_SYMBOL(insb);
EXPORT_SYMBOL(insw);
EXPORT_SYMBOL(insl);

EXPORT_SYMBOL(_memcpy_fromio);
EXPORT_SYMBOL(_memcpy_toio);
EXPORT_SYMBOL(_memset_io);

	/* address translation */
#ifndef __virt_to_phys__is_a_macro
EXPORT_SYMBOL(__virt_to_phys);
#endif
#ifndef __phys_to_virt__is_a_macro
EXPORT_SYMBOL(__phys_to_virt);
#endif
#ifndef __virt_to_bus__is_a_macro
EXPORT_SYMBOL(__virt_to_bus);
#endif
#ifndef __bus_to_virt__is_a_macro
EXPORT_SYMBOL(__bus_to_virt);
#endif

#ifndef CONFIG_NO_PGT_CACHE
EXPORT_SYMBOL(quicklists);
#endif
EXPORT_SYMBOL(__bad_pmd);
EXPORT_SYMBOL(__bad_pmd_kernel);

	/* string / mem functions */
EXPORT_SYMBOL_NOVERS(strcpy);
EXPORT_SYMBOL_NOVERS(strncpy);
EXPORT_SYMBOL_NOVERS(strcat);
EXPORT_SYMBOL_NOVERS(strncat);
EXPORT_SYMBOL_NOVERS(strcmp);
EXPORT_SYMBOL_NOVERS(strncmp);
EXPORT_SYMBOL_NOVERS(strchr);
EXPORT_SYMBOL_NOVERS(strlen);
EXPORT_SYMBOL_NOVERS(strnlen);
EXPORT_SYMBOL_NOVERS(strspn);
EXPORT_SYMBOL_NOVERS(strpbrk);
EXPORT_SYMBOL_NOVERS(strtok);
EXPORT_SYMBOL_NOVERS(strrchr);
EXPORT_SYMBOL_NOVERS(memset);
EXPORT_SYMBOL_NOVERS(memcpy);
EXPORT_SYMBOL_NOVERS(memmove);
EXPORT_SYMBOL_NOVERS(memcmp);
EXPORT_SYMBOL_NOVERS(memscan);
EXPORT_SYMBOL_NOVERS(memzero);

	/* user mem (segment) */
#if defined(CONFIG_CPU_32)
EXPORT_SYMBOL(__arch_copy_from_user);
EXPORT_SYMBOL(__arch_copy_to_user);
EXPORT_SYMBOL(__arch_clear_user);
EXPORT_SYMBOL(__arch_strlen_user);
#elif defined(CONFIG_CPU_26)
EXPORT_SYMBOL(uaccess_kernel);
EXPORT_SYMBOL(uaccess_user);
#endif

	/* gcc lib functions */
EXPORT_SYMBOL_NOVERS(__gcc_bcmp);
EXPORT_SYMBOL_NOVERS(__ashldi3);
EXPORT_SYMBOL_NOVERS(__ashrdi3);
EXPORT_SYMBOL_NOVERS(__cmpdi2);
EXPORT_SYMBOL_NOVERS(__divdi3);
EXPORT_SYMBOL_NOVERS(__divsi3);
EXPORT_SYMBOL_NOVERS(__lshrdi3);
EXPORT_SYMBOL_NOVERS(__moddi3);
EXPORT_SYMBOL_NOVERS(__modsi3);
EXPORT_SYMBOL_NOVERS(__muldi3);
EXPORT_SYMBOL_NOVERS(__negdi2);
EXPORT_SYMBOL_NOVERS(__ucmpdi2);
EXPORT_SYMBOL_NOVERS(__udivdi3);
EXPORT_SYMBOL_NOVERS(__udivmoddi4);
EXPORT_SYMBOL_NOVERS(__udivsi3);
EXPORT_SYMBOL_NOVERS(__umoddi3);
EXPORT_SYMBOL_NOVERS(__umodsi3);

	/* bitops */
EXPORT_SYMBOL(set_bit);
EXPORT_SYMBOL(test_and_set_bit);
EXPORT_SYMBOL(clear_bit);
EXPORT_SYMBOL(test_and_clear_bit);
EXPORT_SYMBOL(change_bit);
EXPORT_SYMBOL(test_and_change_bit);
EXPORT_SYMBOL(find_first_zero_bit);
EXPORT_SYMBOL(find_next_zero_bit);

	/* elf */
EXPORT_SYMBOL(armidlist);
EXPORT_SYMBOL(armidindex);
EXPORT_SYMBOL(elf_platform);

	/* syscalls */
EXPORT_SYMBOL(sys_write);
EXPORT_SYMBOL(sys_read);
EXPORT_SYMBOL(sys_lseek);
EXPORT_SYMBOL(sys_open);
EXPORT_SYMBOL(sys_exit);
EXPORT_SYMBOL(sys_wait4);

	/* semaphores */
EXPORT_SYMBOL_NOVERS(__down_failed);
EXPORT_SYMBOL_NOVERS(__down_interruptible_failed);
EXPORT_SYMBOL_NOVERS(__up_wakeup);

