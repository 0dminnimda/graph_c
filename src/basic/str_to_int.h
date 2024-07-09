#pragma once

#ifndef STR_TO_INT_H
#define STR_TO_INT_H

#include "types.h"

typedef enum {
    S2I_OK,
    S2I_NOT_FOUND,
    S2I_OUT_OF_RANGE
}  S2I_Result;

S2I_Result str_to_u8(const str *string, u8 *value);
S2I_Result str_to_u16(const str *string, u16 *value);
S2I_Result str_to_u32(const str *string, u32 *value);
S2I_Result str_to_u64(const str *string, u64 *value);

S2I_Result str_to_i8(const str *string, i8 *value);
S2I_Result str_to_i16(const str *string, i16 *value);
S2I_Result str_to_i32(const str *string, i32 *value);
S2I_Result str_to_i64(const str *string, i64 *value);

#endif // STR_TO_INT_H
