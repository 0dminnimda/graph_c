#pragma once

#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdlib.h>

#if !defined(TOKENPASTE_) && !defined(TOKENPASTE)
#define TOKENPASTE_(x, y) x ## y
#define TOKENPASTE(x, y) TOKENPASTE_(x, y)
#endif

#define array(T) struct { \
    T *data; \
    size_t length; \
    size_t capacity; \
}

#define ARRAY_APPEND_INIT_CAP 128

#define array_item_size(self) sizeof(*(self)->data)

#define array_init(self) ( \
    (self)->data = NULL, \
    (self)->length = 0, \
    (self)->capacity = 0 \
)

#define array_init_with_capacity(self, cap) ( \
    (self)->data = malloc((cap) * array_item_size((self))), \
    (self)->length = 0, \
    (self)->capacity = (cap) \
)

#define array_deinit(self) free((self)->data)


#define array_expand(self, cap) ( \
    (self)->data = realloc((self)->data, (cap) * array_item_size((self))), \
    (self)->capacity = (cap) \
)

#define array_reserve(self, cap) ( \
    ((self)->capacity < (cap)) ? \
        array_expand((self), (cap)): \
        0 \
)

#define array_resize(self, len) ( \
    array_reserve((self), (len)), \
    (self)->length = (len) \
)

#define array_add(self) ( \
    (((self)->length >= (self)->capacity)? \
	    array_expand((self), \
            (self)->capacity? \
                (self)->capacity * 2: \
                ARRAY_APPEND_INIT_CAP): \
        0), \
    &(self)->data[(self)->length++] \
)

#define array_remove_unordered(self, index) array_replace_by_last(self, index)
#define array_replace_by_last(self, index) ( \
    (self)->length--, \
    (index != (self)->length)? \
        ((self)->data[index] = (self)->data[(self)->length], 0): 0 \
)

#define array_for(self, name) \
    if ((name = (self)->data), 1) \
        for (size_t TOKENPASTE(name, _index) = 0; \
            TOKENPASTE(name, _index) < (self)->length; \
            ++TOKENPASTE(name, _index), ++name)

#define array_for__remove(self, name) \
    array_replace_by_last((self), TOKENPASTE(name, _index)); \
    --name; \
    --TOKENPASTE(name, _index); \
    continue;

#endif // ARRAY_H
