#if !defined(TOKENPASTE_) && !defined(TOKENPASTE)
#define TOKENPASTE_(x, y) x ## y
#define TOKENPASTE(x, y) TOKENPASTE_(x, y)
#endif

#if !defined(STR_) && !defined(STR)
#define STR_(x) #x
#define STR(x) STR_(x)
#endif
