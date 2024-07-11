#pragma once

#ifndef ARGV_H
#define ARGV_H

#include "array.h"
#include "str.h"

/* Command line arguments */
typedef array(str) Clargs;

#define MAIN_CLARGS_WRAPPER(inner) \
    Clargs clargs; \
    array_init_with_capacity_and_length(&clargs, argc, argc); \
    str *clarg; \
    array_for(&clargs, clarg) { \
        str_cstr_view(clarg, argv[clarg_index]); \
    } \
    inner \
    array_deinit(&clargs);

#define MAIN_BODY_WRAPPER(inner) { \
    int result; \
    inner \
    return result; \
}

#define DEFINE_MAIN int main(void) \
MAIN_BODY_WRAPPER( \
    result = Main(); \
)

#define DEFINE_MAIN_CLARGS int main(int argc, char *argv[]) \
MAIN_BODY_WRAPPER(MAIN_CLARGS_WRAPPER( \
    result = Main(&clargs); \
))

#endif // ARGV_H
