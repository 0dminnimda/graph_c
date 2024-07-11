#include "read_line.h"
#include "array.h"
#include "str.h"
#include "str_builder.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* 8 so we don't go a lot of allocations on start.
 * 1 for '\0'. */
#define INITIAL_RESERVE 8 + 1

void fread_line_use_buffer(FILE *stream, StrBuilder *sb) {
    array_reserve(sb, INITIAL_RESERVE);

    /* Those do not count the '\0',
     * because the logic in this function is easier like that */
    size_t len = sb->length;
    size_t cap = sb->capacity - 1;
    char *buf = sb->data;

    while (1) {
        for (; len < cap; ++len) {
            int c = buf[len] = fgetc(stream);
            if (c == EOF) {
                if (len == 0) goto empty;
                else goto finish;
            }
            if (c == '\n') goto finish;
        }

        size_t new_cap = cap * 2;
        char *new_buf = realloc(buf, (new_cap + 1) * sizeof(char));
        if (new_buf == NULL) goto empty;
        cap = new_cap;
        buf = new_buf;
    }

finish:
    /* Update the length - we successfully read it. */
    sb->length = len;
empty:
    /* Leave the length the same - got some errors. */

    /* Reserved space for '\0' */
    sb->capacity = cap + 1;

    /* Regardless, we may have extended this buffer. No reason to throw it out. */
    sb->data = buf;
}

str fread_line(FILE *stream) {
    StrBuilder sb = {0};
    fread_line_use_buffer(stream, &sb);
    return str_builder_get(&sb);
}

void read_line_use_buffer(StrBuilder *sb) {
    fread_line_use_buffer(stdin, sb);
}

str read_line(void) {
    return fread_line(stdin);
}
