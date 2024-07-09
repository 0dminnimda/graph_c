#include "str.h"

#include <stdlib.h>
#include <string.h>

char *string_duplicate(const char *source) {
    size_t len = strlen(source) + 1;
    char *copy = (char *)malloc(len * sizeof(char));
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, source, len);
    return copy;
}

char *string_duplicate_length(const char *source, size_t length) {
    char *copy = (char *)malloc((length + 1) * sizeof(char));
    if (copy == NULL) {
        return NULL;
    }
    strncpy(copy, source, length);
    copy[length] = '\0';
    return copy;
}

char *string_duplicate_known_length(const char *source, size_t length) {
    char *copy = (char *)malloc((length + 1) * sizeof(char));
    if (copy == NULL) {
        return NULL;
    }
    memcpy(copy, source, length);
    copy[length] = '\0';
    return copy;
}

#define min(a, b) ((a) < (b) ? (a) : (b))

void str_init(str *self, const char *data) {
    self->length = strlen(data);
    self->data = string_duplicate_known_length(data, self->length);
}

void str_init_known_length(str *self, const char *data, size_t length) {
    self->length = length;
    self->data = string_duplicate_known_length(data, self->length);
}

void str_init_slice(str *self, char *data, size_t length) {
    self->length = length;
    self->data = data;
}

void str_deinit(str *self) { free(self->data); }

void str_copy(const str *self, str *copy) {
    str_init_known_length(copy, self->data, self->length);
}

int str_compare(const str *self, const str *other) {
    if (self->length < other->length) {
        return -1;
    } else if (self->length > other->length) {
        return 1;
    } else {
        return memcmp(self->data, other->data, self->length);
    }
}

bool str_startswith(const str *self, const str *other) {
    if (self->length < other->length) {
        return false;
    }
    return memcmp(self->data, other->data, other->length) == 0;
}
