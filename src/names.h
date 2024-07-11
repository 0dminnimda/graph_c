#pragma once

#ifndef NAMES_H
#define NAMES_H

#include "basic/array.h"
#include "basic/types.h"
#include "basic/str.h"

typedef array(str) Names;

/* For init use array's init functions */
#define names_deinit(self) array_deinit_with_item(self, str)
bool names_find(const Names *self, const str *string, size_t *index);
bool names_insert(Names *self, const str *string, size_t *index);
void names_copy(const Names *self, Names *copy);

#endif // NAMES_H
