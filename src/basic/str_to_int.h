#pragma once

#ifndef STR_TO_INT_H
#define STR_TO_INT_H

#include "types.h"
#include "str.h"

typedef enum {
    S2I_OK,
    S2I_NOT_FOUND,
    S2I_OUT_OF_RANGE
}  S2I_Result;

/* Treats as much characters as digits as it can.
 * Makes a slice to the string after the parsed integer. */

S2I_Result str_to_u8(str *string, u8 *value);
S2I_Result str_to_u16(str *string, u16 *value);
S2I_Result str_to_u32(str *string, u32 *value);
S2I_Result str_to_u64(str *string, u64 *value);

S2I_Result str_to_s8(str *string, s8 *value);
S2I_Result str_to_s16(str *string, s16 *value);
S2I_Result str_to_s32(str *string, s32 *value);
S2I_Result str_to_s64(str *string, s64 *value);

#endif // STR_TO_INT_H
