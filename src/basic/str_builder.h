#pragma once

#ifndef STR_BUILDER_H
#define STR_BUILDER_H

#include "str.h"
#include "array.h"

typedef array(char) StrBuilder;

/* For init and deinit use array.h */
void str_builder_add(StrBuilder *sb, const str *string);

/* Will make a slice of the data in the builder.
 * Will add the null terminator afted the last (at index length),
 * but it will not increase the length.
 * It's there just in case anyone will use data as cstr. */
str str_builder_get(StrBuilder *sb);

#endif // STR_BUILDER_H
