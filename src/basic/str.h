#pragma once

#ifndef STR_H
#define STR_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    char *data;
    size_t length;
} str;

/* Usually have null terminators, except for slices
 * or strings constructed from non-null-terminated strings.
 * On copy the null terminator is always added.
 * But it's never counted towards the length.
 */

#define PRI_str "%.*s"
#define FMT_str(self) (int)(self)->length, (self)->data

void str_init(str *self, char *data, size_t length);
#define str_lit(lit) (str){ (lit), sizeof((lit)) - 1 /* Don't count the \0 */ }
void str_deinit(str *self);
void str_copy_known_length(str *self, const char *data, size_t length);
void str_copy_cstr(str *self, const char *data);
void str_copy(const str *self, str *copy);
str str_copy_and_return(const str *self);
int str_compare(const str *self, const str *other);
bool str_startswith(const str *self, const str *other);

/* Returns a slice */
str str_strip(str *self, char to_strip);
str str_strip_whitespaces(str *self);

/* Returns slices. Several spaces are treated as one separator. */
void str_partition_whitespace(str *self, str *pre, str *post);

#define str_for(self, name) \
    for (size_t name##_index = 0; \
        name##_index < (self)->length; \
        ++name##_index, name = (self)->data[name##_index])

#endif  // STR_H
