#include "str_to_int.h"

#include <stdlib.h>
#include "checked_arithmetic.h"

#define MAKE_S2U(SIZE, MAX_DIGITS, MSD_MAX_VALUE, MSD_MULTIPLIER) \
S2I_Result str_to_u##SIZE(const str *string, u##SIZE *value) { \
    u8 digits[MAX_DIGITS]; \
    u8 count = 0; \
    u##SIZE bigger = 0; \
    u8 max_i = MAX_DIGITS; \
\
    for (; count < MAX_DIGITS && count < string->length; ++count) { \
        char c = string->data[count]; \
        if ('0' <= c && c <= '9') { \
            digits[MAX_DIGITS - count - 1] = c - '0'; \
        } else { \
            goto less_than_max_digits; \
        } \
    } \
    if (count < string->length) { \
        /* Got more than max digits, definitely not in range. */ \
        char c = string->data[count]; \
        if ('0' <= c && c <= '9') { \
            return S2I_OUT_OF_RANGE; \
        } \
    } \
    u8 msd = digits[MAX_DIGITS-1]; \
    if (msd > MSD_MAX_VALUE) { \
        return S2I_OUT_OF_RANGE; \
    } \
    bigger = msd * MSD_MULTIPLIER; \
    max_i = MAX_DIGITS - 1; \
\
less_than_max_digits: \
    if (count == 0) return S2I_NOT_FOUND; \
\
    u64 smaller = 0; \
    for (u8 i = MAX_DIGITS - count; i < max_i; ++i) { \
        smaller = smaller * 10 + digits[i]; \
    } \
    if (count != MAX_DIGITS) return S2I_OK; \
\
    if (cheked_u_add(U##SIZE, value, bigger, smaller)) { \
        return S2I_OUT_OF_RANGE; \
    } \
    return S2I_OK; \
}

#define MAKE_S2I(SIZE, MAX_DIGITS, MSD_MAX_VALUE, MSD_MULTIPLIER) \
S2I_Result str_to_i##SIZE(const str *string, i##SIZE *value) { \
    (void)string; \
    (void)value; \
    return S2I_NOT_FOUND; \
}

MAKE_S2U(8, 3, 2, 100u)
MAKE_S2U(16, 5, 6, 10000u)
MAKE_S2U(32, 10, 4, 1000000000u)
MAKE_S2U(64, 20, 1, 10000000000000000000u)

MAKE_S2I(8, 3, 1, 100u)
MAKE_S2I(16, 5, 3, 10000u)
MAKE_S2I(32, 10, 2, 1000000000u)
MAKE_S2I(64, 19, 9, 1000000000000000000u)
