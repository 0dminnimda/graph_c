#include "names.h"

bool names_find(const Names *self, const str *string, size_t *index) {
    str *result;
    array_find(self, str, string, ==, result);
    if (result == NULL) {
        return false;
    }
    *index = result - self->data;
    return true;
}

bool names_insert(Names *self, const str *string, size_t *index) {
    if (names_find(self, string, index)) {
        return true;
    }

    str *next = array_add(self);
    *next = str_copy_and_return(string);
    *index = next - self->data;
    return false;
}

void names_copy(const Names *self, Names *copy) {
    array_copy_with_item(self, str, copy);
}

void names_fprint_debug(const Names *self, FILE *stream) {
    fprintf(stream, "Names(%zu):\n", self->length);

    str *it;
    array_for(self, it) {
        fprintf(stream, "[%zu]" PRI_str "\n", it_index, FMT_str(it));
    }
}
