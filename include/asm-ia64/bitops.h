#ifndef _ASM_IA64_BITOPS_H
#define _ASM_IA64_BITOPS_H

/*
 * Copyright (C) 1998-2000 Hewlett-Packard Co
 * Copyright (C) 1998-2000 David Mosberger-Tang <davidm@hpl.hp.com>
 *
 * 02/04/00 D. Mosberger  Require 64-bit alignment for bitops, per suggestion from davem
 */

#include <asm/system.h>

/*
 * These operations need to be atomic.  The address must be "long"
 * aligned.
 *
 * bit 0 is the LSB of addr; bit 32 is the LSB of (addr+1).
 */

extern __inline__ void
set_bit (int nr, volatile void *addr)
{
	__u64 bit, old, new;
	volatile __u64 *m;
	CMPXCHG_BUGCHECK_DECL

	m = (volatile __u64 *) addr + (nr >> 6);
	bit = 1UL << (nr & 63);
	do {
		CMPXCHG_BUGCHECK(m);
		old = *m;
		new = old | bit;
	} while (cmpxchg(m, old, new) != old);
}

extern __inline__ void
clear_bit (int nr, volatile void *addr)
{
	__u64 mask, old, new;
	volatile __u64 *m;
	CMPXCHG_BUGCHECK_DECL

	m = (volatile __u64 *) addr + (nr >> 6);
	mask = ~(1UL << (nr & 63));
	do {
		CMPXCHG_BUGCHECK(m);
		old = *m;
		new = old & mask;
	} while (cmpxchg(m, old, new) != old);
}

extern __inline__ void
change_bit (int nr, volatile void *addr)
{
	__u64 bit, old, new;
	volatile __u64 *m;
	CMPXCHG_BUGCHECK_DECL

	m = (volatile __u64 *) addr + (nr >> 6);
	bit = (1UL << (nr & 63));
	do {
		CMPXCHG_BUGCHECK(m);
		old = *m;
		new = old ^ bit;
	} while (cmpxchg(m, old, new) != old);
}

extern __inline__ int
test_and_set_bit (int nr, volatile void *addr)
{
	__u64 bit, old, new;
	volatile __u64 *m;
	CMPXCHG_BUGCHECK_DECL

	m = (volatile __u64 *) addr + (nr >> 6);
	bit = 1UL << (nr & 63);
	do {
		CMPXCHG_BUGCHECK(m);
		old = *m;
		new = old | bit;
	} while (cmpxchg(m, old, new) != old);
	return (old & bit) != 0;
}

extern __inline__ int
test_and_clear_bit (int nr, volatile void *addr)
{
	__u64 mask, old, new;
	volatile __u64 *m;
	CMPXCHG_BUGCHECK_DECL

	m = (volatile __u64 *) addr + (nr >> 6);
	mask = ~(1UL << (nr & 63));
	do {
		CMPXCHG_BUGCHECK(m);
		old = *m;
		new = old & mask;
	} while (cmpxchg(m, old, new) != old);
	return (old & ~mask) != 0;
}

extern __inline__ int
test_and_change_bit (int nr, volatile void *addr)
{
	__u64 bit, old, new;
	volatile __u64 *m;
	CMPXCHG_BUGCHECK_DECL

	m = (volatile __u64 *) addr + (nr >> 6);
	bit = (1UL << (nr & 63));
	do {
		CMPXCHG_BUGCHECK(m);
		old = *m;
		new = old ^ bit;
	} while (cmpxchg(m, old, new) != old);
	return (old & bit) != 0;
}

extern __inline__ int
test_bit (int nr, volatile void *addr)
{
	return 1UL & (((const volatile __u64 *) addr)[nr >> 6] >> (nr & 63));
}

/*
 * ffz = Find First Zero in word. Undefined if no zero exists,
 * so code should check against ~0UL first..
 */
extern inline unsigned long
ffz (unsigned long x)
{
	unsigned long result;

	__asm__ ("popcnt %0=%1" : "=r" (result) : "r" (x & (~x - 1)));
	return result;
}

#ifdef __KERNEL__

/*
 * Find the most significant bit that is set (undefined if no bit is
 * set).
 */
static inline unsigned long
ia64_fls (unsigned long x)
{
	double d = x;
	long exp;

	__asm__ ("getf.exp %0=%1" : "=r"(exp) : "f"(d));
	return exp - 0xffff;
}
/*
 * ffs: find first bit set. This is defined the same way as
 * the libc and compiler builtin ffs routines, therefore
 * differs in spirit from the above ffz (man ffs).
 */
#define ffs(x)	__builtin_ffs(x)

/*
 * hweightN: returns the hamming weight (i.e. the number
 * of bits set) of a N-bit word
 */
extern __inline__ unsigned long
hweight64 (unsigned long x)
{
	unsigned long result;
	__asm__ ("popcnt %0=%1" : "=r" (result) : "r" (x));
	return result;
}

#define hweight32(x) hweight64 ((x) & 0xfffffffful)
#define hweight16(x) hweight64 ((x) & 0xfffful)
#define hweight8(x)  hweight64 ((x) & 0xfful)

#endif /* __KERNEL__ */

/*
 * Find next zero bit in a bitmap reasonably efficiently..
 */
extern inline int
find_next_zero_bit (void *addr, unsigned long size, unsigned long offset)
{
	unsigned long *p = ((unsigned long *) addr) + (offset >> 6);
	unsigned long result = offset & ~63UL;
	unsigned long tmp;

	if (offset >= size)
		return size;
	size -= result;
	offset &= 63UL;
	if (offset) {
		tmp = *(p++);
		tmp |= ~0UL >> (64-offset);
		if (size < 64)
			goto found_first;
		if (~tmp)
			goto found_middle;
		size -= 64;
		result += 64;
	}
	while (size & ~63UL) {
		if (~(tmp = *(p++)))
			goto found_middle;
		result += 64;
		size -= 64;
	}
	if (!size)
		return result;
	tmp = *p;
found_first:
	tmp |= ~0UL << size;
found_middle:
	return result + ffz(tmp);
}

/*
 * The optimizer actually does good code for this case..
 */
#define find_first_zero_bit(addr, size) find_next_zero_bit((addr), (size), 0)

#ifdef __KERNEL__

#define ext2_set_bit                 test_and_set_bit
#define ext2_clear_bit               test_and_clear_bit
#define ext2_test_bit                test_bit
#define ext2_find_first_zero_bit     find_first_zero_bit
#define ext2_find_next_zero_bit      find_next_zero_bit

/* Bitmap functions for the minix filesystem.  */
#define minix_set_bit(nr,addr)			test_and_set_bit(nr,addr)
#define minix_clear_bit(nr,addr)		test_and_clear_bit(nr,addr)
#define minix_test_bit(nr,addr)			test_bit(nr,addr)
#define minix_find_first_zero_bit(addr,size)	find_first_zero_bit(addr,size)

#endif /* __KERNEL__ */

#endif /* _ASM_IA64_BITOPS_H */
