#include "str.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

str str_copy_and_return(const str *self) {
    str copy;
    str_copy(self, &copy);
    return copy;
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

str str_strip(str *self, char to_strip) {
    char c;
    str_for(self, c) {
        if (c != to_strip)
            return (str){self->data + c_index, self->length - c_index};
    }
    return (str){self->data + self->length, 0};
}

str str_strip_whitespaces(str *self) {
    char c;
    str_for(self, c) {
        if (!isspace(c))
            return (str){self->data + c_index, self->length - c_index};
    }
    return (str){self->data + self->length, 0};
}

void str_partition_whitespace(str *self, str *pre, str *post) {
    char c;
    size_t pre_length = self->length;
    str_for(self, c) {
        if (isspace(c)) {
            pre_length = c_index;
            break;
        }
    }
    *pre = (str){self->data, pre_length};

    str rest = {self->data + pre_length, self->length - pre_length};
    if (rest.length == 0) {
        *post = rest;
        return;
    }

    size_t space_length = rest.length;
    str_for(&rest, c) {
        if (!isspace(c)) {
            space_length = c_index;
            break;
        }
    }
    *post = (str){rest.data + space_length, rest.length - space_length};
    return;
}
