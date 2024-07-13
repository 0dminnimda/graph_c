#pragma once

#ifndef ARRAY_H
#define ARRAY_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "macros.h"

#define array(T) struct { \
    T *data; \
    size_t length; \
    size_t capacity; \
}

#define ARRAY_APPEND_INIT_CAP 128

#define array_it_ CONCAT(it__, __LINE__)
#define array_ind_ CONCAT(CONCAT(it__, __LINE__), _index)
#define array_i_ CONCAT(arr_i__, __LINE__)
#define array_tmp_ CONCAT(tmp__, __LINE__)

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

#define array_init_with_capacity_and_length(self, cap, len) ( \
    (self)->data = malloc((cap) * array_item_size((self))), \
    (self)->length = (len), \
    (self)->capacity = (cap) \
)

#define array_deinit(self) free((self)->data)

#define array_deinit_with_item(self, T) do { \
    T *array_it_; \
    array_for((self), array_it_) { \
        T##_deinit(array_it_); \
    } \
    array_deinit((self)); \
} while(0)

#define array_copy(self, other) ( \
    (other)->data = malloc((self)->length * array_item_size((self))), \
    (other)->length = (self)->length, \
    (other)->capacity = (self)->length, \
    memcpy((other)->data, (self)->data, (self)->length * array_item_size((self))) \
)

#define array_copy_with_item(self, T, other) do { \
    array_copy((self), (other)); \
    T *array_it_; \
    array_for((self), array_it_) { \
        T##_copy(&(other)->data[array_ind_], array_it_); \
    } \
} while(0)

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

#define array_pop(self) ( \
    assert((self)->length && "pop from an empty array"), \
    (self)->data[--(self)->length] \
)

#define array_extend_from(self, other) ( \
    (other)->length? ( \
        array_resize((self), (self)->length + (other)->length), \
        memcpy((self)->data + (self)->length, (other)->data, (other)->length * array_item_size((self))), \
        0 \
    ) : 0 \
)

#define array_remove_unordered(self, index) array_replace_by_last(self, index)
#define array_replace_by_last(self, index) ( \
    (self)->length--, \
    ((index) != (self)->length)? \
        ((self)->data[(index)] = (self)->data[(self)->length], 0): 0 \
)

/* result is a pointer to the item in the array */
#define array_find(self, T, item, op, result) do { \
    T *array_it_; \
    result = NULL; \
    array_for((self), array_it_) { \
        if (T##_compare((item), array_it_) op 0) { \
            result = array_it_; \
            break; \
        } \
    } \
} while(0)

#define array_sort(self, cmp) ( \
    ((self)->length > 1)? \
        (qsort((self)->data, (self)->length, array_item_size((self)), (int (*)(const void*, const void*))cmp), 0) \
        : 0 \
)

#define array_reverse(self, T) do { \
    for (size_t array_i_ = 0; array_i_ < (self)->length / 2; ++array_i_) { \
        T array_tmp_ = (self)->data[array_i_]; \
        (self)->data[array_i_] = (self)->data[(self)->length - 1 - array_i_]; \
        (self)->data[(self)->length - 1 - array_i_] = array_tmp_; \
    } \
} while(0)

#define array_for(self, name) \
    if ((name = (self)->data), 1) \
        for (size_t CONCAT(name, _index) = 0; \
            CONCAT(name, _index) < (self)->length; \
            ++CONCAT(name, _index), ++name)

#define array_for__remove(self, name) \
    array_replace_by_last((self), CONCAT(name, _index)); \
    --name; \
    --CONCAT(name, _index); \
    continue;

#endif // ARRAY_H
