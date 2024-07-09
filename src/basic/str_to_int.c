#include "str_to_int.h"

#include <stdlib.h>
#include "cheked_arithmetic.h"

#define SIZE 64
#define MAX_DIGITS 20
#define MSD_MAX_VALUE 1
#define MSD_MULTIPLIER 10000000000000000000
#include "str_to_u_pattern_.c"
#undef SIZE
#undef MAX_DIGITS
#undef MSD_MAX_VALUE
#undef MSD_MULTIPLIER

/*
S2I_Result str_to_signed(const str *s, void *v) {
}

S2I_Result str_to_u8(const str *string, u8 *value) {}
S2I_Result str_to_u16(const str *string, u16 *value) {}
S2I_Result str_to_u32(const str *string, u32 *value) {}
S2I_Result str_to_u64(const str *string, u64 *value) {
    u8 digits[20];
    u8 count = 0;
    u64 bigger = 0;

    for (; count < 20 && count < string->length; ++count) {
        char c = string->data[count];
        if ('0' <= c && c <= '9') {
            digits[20 - count - 1] = c - '0';
        } else {
            goto less_than_max_digits;
        }
    }
    if (count < string->length) {
        // Got more than max digits, definitely not in range.
        char c = string->data[count];
        if ('0' <= c && c <= '9') {
            return S2I_OUT_OF_RANGE;
        }
    }
    u8 msd = digits[20-1];
    if (msd > 1) {
        return S2I_OUT_OF_RANGE;
    }
    bigger = msd * 10000000000000000000;

less_than_max_digits:
    if (count == 0) return S2I_NOT_FOUND;

    u64 smaller = 0;
    u8 i = 20 - count;
    for (; i < ((count == 20)? 19 : 20); ++i) {
        smaller = smaller * 10 + digits[i];
    }
    if (count != 20) return S2I_OK;

    if (checked_u_add(U64, value, bigger, smaller)) {
        return S2I_OUT_OF_RANGE;
    }
    return S2I_OK;
}

S2I_Result str_to_i8(const str *string, i8 *value) {}
S2I_Result str_to_i16(const str *string, i16 *value) {}
S2I_Result str_to_i32(const str *string, i32 *value) {}
S2I_Result str_to_i64(const str *string, i64 *value) {}
*/
