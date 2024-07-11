#include "type_operations.h"

#define MAKE_CMP(T) \
int T##_compare(const T *a, const T *b) { \
    T A = *a, B = *b; \
    return (A > B) - (A < B); \
}

#define MAKE_CMP_REV(T) \
int T##_compare_reversed(const T *a, const T *b) { \
    T A = *a, B = *b; \
    return (A < B) - (A > B); \
}

MAKE_CMP(u8)
MAKE_CMP(u16)
MAKE_CMP(u32)
MAKE_CMP(u64)

MAKE_CMP(s8)
MAKE_CMP(s16)
MAKE_CMP(s32)
MAKE_CMP(s64)

MAKE_CMP_REV(u8)
MAKE_CMP_REV(u16)
MAKE_CMP_REV(u32)
MAKE_CMP_REV(u64)

MAKE_CMP_REV(s8)
MAKE_CMP_REV(s16)
MAKE_CMP_REV(s32)
MAKE_CMP_REV(s64)
