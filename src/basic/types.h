#pragma once

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <inttypes.h>

/* Signed numbers */
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

/* Unsigned numbers */
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

/* Floating point numbers */
typedef float f32;
typedef double f64;
typedef long double f80;

#define PRI_s8  "%" PRId8
#define PRI_s16 "%" PRId16
#define PRI_s32 "%" PRId32
#define PRI_s64 "%" PRId64

#define PRI_u8  "%" PRIu8
#define PRI_u16 "%" PRIu16
#define PRI_u32 "%" PRIu32
#define PRI_u64 "%" PRIu64

/* Just in case if it's gonna be handy.
 * You don't have to use them. */
#define PRI_f32 "%f"
#define PRI_f64 "%lf"
#define PRI_f80 "%Lf"

/* Their limits. Useful to have in one place. */
#define S8_MAX 127
#define S8_MIN -128
#define U8_MAX 255

#define S16_MAX 32767
#define S16_MIN -32768
#define U16_MAX 65535

#define S32_MAX 2147483647
#define S32_MIN −2147483648
#define U32_MAX 4294967295

#define S64_MAX 9223372036854775807
#define S64_MIN −9223372036854775808
#define U64_MAX 18446744073709551615

#endif  // TYPES_H
