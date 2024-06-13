// (c) 2024 - Dylan Leifer-Ives
// Version 0.1 10/13/2024
// - Supports consistent types with sizes using stdint.h
// - Supports 128 bit integers if available
#ifndef __WARTLESS_H_
#define __WARTLESS_H_

#ifndef __WARTLESS_V_0_1
#error "Expecting wartless.h version 0.1, or forward definition for __WARTLESS_V_0_1"
#else
#include <stdint.h>

/// Type definitions
/// u{8,16,32,64,128} - unsigned integer types
/// s{8,16,32,64,128} - signed integer types
/// f{32,64} - floating point types
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float f32;
typedef double f64;

#if defined(__SIZEOF_INT128__)
typedef __uint128_t u128;
typedef __int128_t s128;
#endif // __SIZEOF_INT128__

#endif // __WARTLESS_V_0_1
#endif // __WARTLESS_H_
