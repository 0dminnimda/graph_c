#include <limits.h>

#define I8_MAX 127
#define I8_MIN -128
#define U8_MAX 255

#define I16_MAX 32767
#define I16_MIN -32768
#define U16_MAX 65535

#define I32_MAX 2147483647
#define I32_MIN −2147483648
#define U32_MAX 4294967295

#define I64_MAX 9223372036854775807
#define I64_MIN −9223372036854775808
#define U64_MAX 18446744073709551615

#if __has_include(<stdckdint.h>)
#  include <stdckdint.h>
#  define cheked_s_add(T, R, A, B) ckd_add ((R), (A), (B))
#  define cheked_s_sub(T, R, A, B) ckd_sub ((R), (A), (B))
#  define cheked_s_mul(T, R, A, B) ckd_mul ((R), (A), (B))
#  define cheked_u_add(T, R, A, B) ckd_add ((R), (A), (B))
#  define cheked_u_sub(T, R, A, B) ckd_sub ((R), (A), (B))
#  define cheked_u_mul(T, R, A, B) ckd_mul ((R), (A), (B))
#elif defined(__GNUC__)  /* gcc and clagn have this */
#  define cheked_s_add(T, R, A, B) __builtin_add_overflow ((A), (B), (R))
#  define cheked_s_sub(T, R, A, B) __builtin_sub_overflow ((A), (B), (R))
#  define cheked_s_mul(T, R, A, B) __builtin_mul_overflow ((A), (B), (R))
#  define cheked_u_add(T, R, A, B) __builtin_add_overflow ((A), (B), (R))
#  define cheked_u_sub(T, R, A, B) __builtin_sub_overflow ((A), (B), (R))
#  define cheked_u_mul(T, R, A, B) __builtin_mul_overflow ((A), (B), (R))
#else
// For mcvs/intel: https://stackoverflow.com/questions/69565333/are-there-overflow-check-math-functions-for-msvc

#  define cheked_s_add(T, R, A, B) ( \
    (((B) > 0 && (A) > T##_MAX - (B)) || ((B) < 0 && (A) < T##_MIN - (B))) ? \
    true : ((*(R) = (A) + (B)), false) \
)
#  define cheked_u_add(T, R, A, B) ( \
    ((A) > T##_MAX - (B)) ? \
    true : ((*(R) = (A) + (B)), false) \
)
#  define cheked_s_sub(T, R, A, B) ( \
    (((B) < 0 && (A) > T##_MAX + (B)) || ((B) > 0 && (A) < T##_MIN + (B))) ? \
    true : ((*(R) = (A) - (B)), false) \
)
#  define cheked_u_sub(T, R, A, B) ( \
    ((A) < (B)) ? \
    true : ((*(R) = (A) - (B)), false) \
)
#  define cheked_s_mul(T, R, A, B) ( \
    ( \
        /* There may be a need to check for -1 for two's complement machines.
         * If one number is -1 and another is INT_MIN,
         * multiplying them we get abs(INT_MIN) which is 1 higher than INT_MAX */ \
        ((A) == -1 && (B) == T##_MIN) || ((B) == -1 && (A) == T##_MIN) \
        /* General case */ \
        || ((B) != 0 && (((A) > T##_MAX / (B)) || ((A) < T##_MIN / (B)))) \
    )? true : ((*(R) = (A) * (B)), false) \
)
#  define cheked_u_mul(T, R, A, B) ( \
    ((B) != 0 && ((A) > T##_MAX / (B)))? \
    true : ((*(R) = (A) * (B)), false) \
)
#endif

