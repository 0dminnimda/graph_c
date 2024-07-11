#if !defined(CONCAT_) && !defined(CONCAT)
#define CONCAT_(x, y) x ## y
#define CONCAT(x, y) CONCAT_(x, y)
#endif

#if !defined(STR_) && !defined(STR)
#define STR_(x) #x
#define STR(x) STR_(x)
#endif
