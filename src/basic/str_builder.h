#pragma once

#ifndef STR_BUILDER_H
#define STR_BUILDER_H

#include "string.h"
#include "array.h"

typedef array(char) StrBuilder;

/* For init and deinit use array.h */
void str_builder_add(StrBuilder *sb, const str *string);
str str_builder_get(StrBuilder *sb);

#endif // STR_BUILDER_H
