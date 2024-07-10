#pragma once

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <inttypes.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;
typedef long double f80;

#define PRI_i8  "%" PRIi8
#define PRI_i16 "%" PRIi16
#define PRI_i32 "%" PRIi32
#define PRI_i64 "%" PRIi64

#define PRI_u8  "%" PRIu8
#define PRI_u16 "%" PRIu16
#define PRI_u32 "%" PRIu32
#define PRI_u64 "%" PRIu64

#endif  // TYPES_H
