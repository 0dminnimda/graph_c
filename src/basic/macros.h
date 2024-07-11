#if !defined(TOKENPASTE_) && !defined(TOKENPASTE)
#define TOKENPASTE_(x, y) x ## y
#define TOKENPASTE(x, y) TOKENPASTE_(x, y)
#endif
