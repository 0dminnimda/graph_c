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

#endif  // TYPES_H
