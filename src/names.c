#include "names.h"

str *names_find(const Names *self, const str *string) {
    str *result;
    array_find(self, str, string, ==, result);
    return result;
}

bool names_insert(Names *self, const str *string, size_t *result) {
    str *item = names_find(self, string);
    if (item != NULL) {
        *result = item - self->data;
        return true;
    }

    str *next = array_add(self);
    *next = str_copy_and_return(string);
    *result = next - self->data;
    return false;
}

void names_copy(const Names *self, Names *copy) {
    array_copy_with_item(self, str, copy);
}
