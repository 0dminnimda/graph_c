#pragma once

#ifndef TYPE_OPERATIONS_H
#define TYPE_OPERATIONS_H

#include "types.h"

int u8_compare(const u8 *a, const u8 *b);
int u16_compare(const u16 *a, const u16 *b);
int u32_compare(const u32 *a, const u32 *b);
int u64_compare(const u64 *a, const u64 *b);

int s8_compare(const s8 *a, const s8 *b);
int s16_compare(const s16 *a, const s16 *b);
int s32_compare(const s32 *a, const s32 *b);
int s64_compare(const s64 *a, const s64 *b);

int u8_compare_reversed(const u8 *a, const u8 *b);
int u16_compare_reversed(const u16 *a, const u16 *b);
int u32_compare_reversed(const u32 *a, const u32 *b);
int u64_compare_reversed(const u64 *a, const u64 *b);

int s8_compare_reversed(const s8 *a, const s8 *b);
int s16_compare_reversed(const s16 *a, const s16 *b);
int s32_compare_reversed(const s32 *a, const s32 *b);
int s64_compare_reversed(const s64 *a, const s64 *b);

#endif // TYPE_OPERATIONS_H
