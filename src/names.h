#pragma once

#ifndef NAMES_H
#define NAMES_H

#include "basic/array.h"
#include "basic/types.h"
#include "basic/str.h"

typedef array(str) Names;

/* For init use array's init functions */
#define names_deinit(self) array_deinit_with_item(self, str)
str *names_find(const Names *self, const str *string);
str *names_insert(Names *self, const str *string);
void names_copy(const Names *self, Names *copy);

#endif // NAMES_H
