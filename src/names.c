#include "names.h"

str *names_find(const Names *self, const str *string) {
    str *result;
    array_find(self, str, string, ==, result);
    return result;
}

str *names_insert(Names *self, const str *string) {
    str *item = names_find(self, string);
    if (item != NULL) {
        return item;
    }

    str *next = array_add(self);
    *next = str_copy_and_return(string);
    return next;
}

void names_copy(const Names *self, Names *copy) {
    array_copy_with_item(self, str, copy);
}
