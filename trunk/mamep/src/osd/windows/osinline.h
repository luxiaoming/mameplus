//============================================================
//
//  osinline.h
//
//  x86 inline implementations for MSVC compiler.
//
//  Copyright (c) 1996-2007, Nicola Salmoria and the MAME Team.
//  Visit http://mamedev.org for licensing and usage restrictions.
//
//============================================================

#ifndef __OSINLINE__
#define __OSINLINE__

#ifdef PTR64
#include <intrin.h>
#pragma intrinsic(_BitScanReverse)
#else
#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchangeAdd)
#endif


/***************************************************************************
    INLINE MATH FUNCTIONS
***************************************************************************/

/*-------------------------------------------------
    mul_32x32 - perform a signed 32 bit x 32 bit
    multiply and return the full 64 bit result
-------------------------------------------------*/

#ifndef PTR64
#define mul_32x32 _mul_32x32
INLINE INT64 _mul_32x32(INT32 a, INT32 b)
{
	// in theory this should work, but it is untested
    __asm
    {
        mov   eax,a
        imul  b
        // leave results in edx:eax
    }
}
#endif


/*-------------------------------------------------
    mulu_32x32 - perform an unsigned 32 bit x
    32 bit multiply and return the full 64 bit
    result
-------------------------------------------------*/

#ifndef PTR64
#define mulu_32x32 _mulu_32x32
INLINE UINT64 _mulu_32x32(UINT32 a, UINT32 b)
{
	// in theory this should work, but it is untested
    __asm
    {
        mov   eax,a
        mul   b
        // leave results in edx:eax
    }
}
#endif


/*-------------------------------------------------
    mul_32x32_hi - perform a signed 32 bit x 32 bit
    multiply and return the upper 32 bits of the
    result
-------------------------------------------------*/

#ifndef PTR64
#define mul_32x32_hi _mul_32x32_hi
INLINE INT32 _mul_32x32_hi(INT32 a, INT32 b)
{
	INT32 result;

    __asm
    {
        mov   eax,a
        imul  b
        mov   result,edx
    }

	return result;
}
#endif


/*-------------------------------------------------
    mulu_32x32_hi - perform an unsigned 32 bit x
    32 bit multiply and return the upper 32 bits
    of the result
-------------------------------------------------*/

#ifndef PTR64
#define mulu_32x32_hi _mulu_32x32_hi
INLINE UINT32 _mulu_32x32_hi(UINT32 a, UINT32 b)
{
	INT32 result;

    __asm
    {
        mov   eax,a
        mul   b
        mov   result,edx
    }

	return result;
}
#endif


/*-------------------------------------------------
    mul_32x32_shift - perform a signed 32 bit x
    32 bit multiply and shift the result by the
    given number of bits before truncating the
    result to 32 bits
-------------------------------------------------*/

#ifndef PTR64
#define mul_32x32_shift _mul_32x32_shift
INLINE INT32 _mul_32x32_shift(INT32 a, INT32 b, UINT8 shift)
{
	INT32 result;

    __asm
    {
        mov   eax,a
        imul  b
        mov   cl,shift
        shrd  eax,edx,cl
        mov   result,eax
    }

	return result;
}
#endif


/*-------------------------------------------------
    mulu_32x32_shift - perform an unsigned 32 bit x
    32 bit multiply and shift the result by the
    given number of bits before truncating the
    result to 32 bits
-------------------------------------------------*/

#ifndef PTR64
#define mulu_32x32_shift _mulu_32x32_shift
INLINE UINT32 _mulu_32x32_shift(UINT32 a, UINT32 b, UINT8 shift)
{
	INT32 result;

    __asm
    {
        mov   eax,a
        mul   b
        mov   cl,shift
        shrd  eax,edx,cl
        mov   result,eax
    }

	return result;
}
#endif


/*-------------------------------------------------
    div_64x32 - perform a signed 64 bit x 32 bit
    divide and return the 32 bit quotient
-------------------------------------------------*/

/* TBD */


/*-------------------------------------------------
    divu_64x32 - perform an unsigned 64 bit x 32 bit
    divide and return the 32 bit quotient
-------------------------------------------------*/

/* TBD */


/*-------------------------------------------------
    div_32x32_shift - perform a signed divide of
    two 32 bit values, shifting the first before
    division, and returning the 32 bit quotient
-------------------------------------------------*/

#ifndef PTR64
#define div_32x32_shift _div_32x32_shift
INLINE INT32 _div_32x32_shift(INT32 a, INT32 b, UINT8 shift)
{
	INT32 result;

    __asm
    {
        mov   eax,a
        cdq
        mov   cl,shift
        shld  edx,eax,cl
        shl   eax,cl
        idiv  b
        mov   result,eax
    }

	return result;
}
#endif


/*-------------------------------------------------
    divu_32x32_shift - perform an unsigned divide of
    two 32 bit values, shifting the first before
    division, and returning the 32 bit quotient
-------------------------------------------------*/

#ifndef PTR64
#define divu_32x32_shift _divu_32x32_shift
INLINE UINT32 _divu_32x32_shift(UINT32 a, UINT32 b, UINT8 shift)
{
	INT32 result;

    __asm
    {
        mov   eax,a
        xor   edx,edx
        mov   cl,shift
        shld  edx,eax,cl
        shl   eax,cl
        div   b
        mov   result,eax
    }

	return result;
}
#endif


/*-------------------------------------------------
    mod_64x32 - perform a signed 64 bit x 32 bit
    divide and return the 32 bit remainder
-------------------------------------------------*/

/* TBD */


/*-------------------------------------------------
    modu_64x32 - perform an unsigned 64 bit x 32 bit
    divide and return the 32 bit remainder
-------------------------------------------------*/

/* TBD */


/*-------------------------------------------------
    recip_approx - compute an approximate floating
    point reciprocal
-------------------------------------------------*/

#ifdef PTR64
#define recip_approx _recip_approx
INLINE float _recip_approx(float z)
{
	__m128 mz = _mm_set_ss(z);
	__m128 mooz = _mm_rcp_ss(mz);
	float ooz;
	_mm_store_ss(&ooz, mooz);
	return ooz;
}
#endif



/***************************************************************************
    INLINE BIT MANIPULATION FUNCTIONS
***************************************************************************/

/*-------------------------------------------------
    count_leading_zeros - return the number of
    leading zero bits in a 32-bit value
-------------------------------------------------*/

#define count_leading_zeros _count_leading_zeros
INLINE UINT8 _count_leading_zeros(UINT32 value)
{
#ifdef PTR64
	UINT32 index;
	return _BitScanReverse(&index, value) ? (index ^ 31) : 32;
#else
	INT32 result;

    __asm
    {
    	bsr   eax,value
    	jnz   skip
    	mov   eax,63
    skip:
    	xor   eax,31
        mov   result,eax
    }

	return result;
#endif
}


/*-------------------------------------------------
    count_leading_ones - return the number of
    leading one bits in a 32-bit value
-------------------------------------------------*/

#define count_leading_ones _count_leading_ones
INLINE UINT8 _count_leading_ones(UINT32 value)
{
#ifdef PTR64
	UINT32 index;
	return _BitScanReverse(&index, ~value) ? (index ^ 31) : 32;
#else
	INT32 result;

    __asm
    {
    	mov   eax,value
    	not   eax
    	bsr   eax,eax
    	jnz   skip
    	mov   eax,63
    skip:
    	xor   eax,31
        mov   result,eax
    }

	return result;
#endif
}



/***************************************************************************
    INLINE SYNCHRONIZATION FUNCTIONS
***************************************************************************/

/*-------------------------------------------------
    compare_exchange32 - compare the 'compare'
    value against the memory at 'ptr'; if equal,
    swap in the 'exchange' value. Regardless,
    return the previous value at 'ptr'.
-------------------------------------------------*/

#define compare_exchange32 _compare_exchange32
INLINE INT32 _compare_exchange32(INT32 volatile *ptr, INT32 compare, INT32 exchange)
{
	return _InterlockedCompareExchange(ptr, exchange, compare);
}


/*-------------------------------------------------
    compare_exchange64 - compare the 'compare'
    value against the memory at 'ptr'; if equal,
    swap in the 'exchange' value. Regardless,
    return the previous value at 'ptr'.
-------------------------------------------------*/

#ifdef PTR64
#define compare_exchange64 _compare_exchange64
INLINE INT64 _compare_exchange64(INT64 volatile *ptr, INT64 compare, INT64 exchange)
{
	return _InterlockedCompareExchange64(ptr, exchange, compare);
}
#endif


/*-------------------------------------------------
    atomic_exchange32 - atomically exchange the
    exchange value with the memory at 'ptr',
    returning the original value.
-------------------------------------------------*/

#define atomic_exchange32 _atomic_exchange32
INLINE INT32 _atomic_exchange32(INT32 volatile *ptr, INT32 exchange)
{
	return _InterlockedExchange(ptr, exchange);
}


/*-------------------------------------------------
    atomic_add32 - atomically add the delta value
    to the memory at 'ptr', returning the final
    result.
-------------------------------------------------*/

#define atomic_add32 _atomic_add32
INLINE INT32 _atomic_add32(INT32 volatile *ptr, INT32 delta)
{
	return _InterlockedExchangeAdd(ptr, delta) + delta;
}


/*-------------------------------------------------
    atomic_increment32 - atomically increment the
    32-bit value in memory at 'ptr', returning the
    final result.
-------------------------------------------------*/

#define atomic_increment32 _atomic_increment32
INLINE INT32 _atomic_increment32(INT32 volatile *ptr, INT32 delta)
{
	return _InterlockedIncrement(ptr);
}


/*-------------------------------------------------
    atomic_decrement32 - atomically decrement the
    32-bit value in memory at 'ptr', returning the
    final result.
-------------------------------------------------*/

#define atomic_decrement32 _atomic_decrement32
INLINE INT32 _atomic_decrement32(INT32 volatile *ptr, INT32 delta)
{
	return _InterlockedDecrement(ptr);
}


#endif /* __OSINLINE__ */
