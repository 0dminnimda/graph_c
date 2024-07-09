#include "str_to_int.h"

#include <stdlib.h>
#include "cheked_arithmetic.h"

#define SIZE 8
#define MAX_DIGITS 3
#define MSD_MAX_VALUE 2
#define MSD_MULTIPLIER 100
#include "str_to_u_pattern_.c"
#undef SIZE
#undef MAX_DIGITS
#undef MSD_MAX_VALUE
#undef MSD_MULTIPLIER

#define SIZE 16
#define MAX_DIGITS 5
#define MSD_MAX_VALUE 6
#define MSD_MULTIPLIER 10000
#include "str_to_u_pattern_.c"
#undef SIZE
#undef MAX_DIGITS
#undef MSD_MAX_VALUE
#undef MSD_MULTIPLIER

#define SIZE 32
#define MAX_DIGITS 10
#define MSD_MAX_VALUE 4
#define MSD_MULTIPLIER 1000000000
#include "str_to_u_pattern_.c"
#undef SIZE
#undef MAX_DIGITS
#undef MSD_MAX_VALUE
#undef MSD_MULTIPLIER

#define SIZE 64
#define MAX_DIGITS 20
#define MSD_MAX_VALUE 1
#define MSD_MULTIPLIER 10000000000000000000
#include "str_to_u_pattern_.c"
#undef SIZE
#undef MAX_DIGITS
#undef MSD_MAX_VALUE
#undef MSD_MULTIPLIER

#define SIZE 8
#define MAX_DIGITS 3
#define MSD_MAX_VALUE 1
#define MSD_MULTIPLIER 100
#include "str_to_i_pattern_.c"
#undef SIZE
#undef MAX_DIGITS
#undef MSD_MAX_VALUE
#undef MSD_MULTIPLIER

#define SIZE 16
#define MAX_DIGITS 5
#define MSD_MAX_VALUE 3
#define MSD_MULTIPLIER 10000
#include "str_to_i_pattern_.c"
#undef SIZE
#undef MAX_DIGITS
#undef MSD_MAX_VALUE
#undef MSD_MULTIPLIER

#define SIZE 32
#define MAX_DIGITS 10
#define MSD_MAX_VALUE 2
#define MSD_MULTIPLIER 1000000000
#include "str_to_i_pattern_.c"
#undef SIZE
#undef MAX_DIGITS
#undef MSD_MAX_VALUE
#undef MSD_MULTIPLIER

#define SIZE 64
#define MAX_DIGITS 19
#define MSD_MAX_VALUE 9
#define MSD_MULTIPLIER 1000000000000000000
#include "str_to_i_pattern_.c"
#undef SIZE
#undef MAX_DIGITS
#undef MSD_MAX_VALUE
#undef MSD_MULTIPLIER
