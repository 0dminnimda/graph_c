#include "str.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "types.h"

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

void str_init(str *self, char *data, size_t length) {
    self->length = length;
    self->data = data;
}

void str_cstr_view(str *self, char *data) {
    self->data = data;
    self->length = strlen(data);
}

void str_deinit(str *self) { free(self->data); }

void str_copy_known_length(str *self, const char *data, size_t length) {
    self->length = length;
    self->data = string_duplicate_known_length(data, self->length);
}

void str_copy_cstr(str *self, const char *data) {
    str_copy_known_length(self, data, strlen(data));
}

void str_copy(const str *self, str *copy) {
    str_copy_known_length(copy, self->data, self->length);
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
    } else if (self->length == 0) {
        return 0;
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

/* TODO: find, replace, count, etc.
 * SEE:
 *   https://github.com/lattera/glibc/blob/master/string/strstr.c
 *   https://github.com/python/cpython/tree/main/Objects/stringlib
 *   https://stackoverflow.com/questions/3183582/what-is-the-fastest-substring-search-algorithm
 *   https://smart-tool.github.io/smart/
 *   https://web.archive.org/web/20200420024210/http://www.dmi.unict.it/~faro/smart/algorithms.php
 *   https://www-igm.univ-mlv.fr/~lecroq/string/index.html
 *   https://web.archive.org/web/20170829195535/http://www.stupros.com/site/postconcept/Breslauer-Grossi-Mignosi.pdf
 *   https://www.cs.haifa.ac.il/%7Eoren/Publications/bpsm.pdf
 *   https://www.cs.utexas.edu/~moore/publications/sustik-moore.pdf
 */


void str_debug_fprint(const str *self, FILE *stream) {
    char c;
    str_for(self, c) {
        fprintf(stream, "[%zu] = '%c' (" PRI_u8 ")\n", c_index, c, (u8)c);
    }
}
