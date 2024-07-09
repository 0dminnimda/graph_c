#pragma once

#ifndef STR_H
#define STR_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    char *data;
    size_t length;
} str;

void str_init(str *self, const char *data);
void str_init_known_length(str *self, const char *data, size_t length);
void str_init_slice(str *self, char *data, size_t length);
#define str_lit(lit) (str){lit, sizeof(lit)}
void str_deinit(str *self);
#define PRI_str "%.*s"
#define FMT_str(self) (int)(self)->length, (self)->data
void str_copy(const str *self, str *copy);
int str_compare(const str *self, const str *other);
bool str_startswith(const str *self, const str *other);

#endif  // STR_H
