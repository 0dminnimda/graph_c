#include "read_line.h"
#include "str.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

str fread_line(FILE *stream) {
    str result;

    // those do not count the '\0'
    size_t len = 0;
    size_t cap = 8;
    char *buf = malloc((cap + 1) * sizeof(char));

    while (1) {
        for (; len < cap; ++len) {
            int c = buf[len] = fgetc(stream);
            if (c == EOF) {
                if (len == 0) goto empty;
                else goto finish;
            }
            if (c == '\n') goto finish;
        }

        cap *= 2;
        char *new_buf = realloc(buf, (cap + 1) * sizeof(char));
        if (new_buf == NULL) goto empty;
        buf = new_buf;
    }

finish:
    buf[len] = '\0';
    str_init_slice(&result, buf, len);
    return result;

empty:
    free(buf);
    str_init_slice(&result, NULL, 0);
    return result;
}

str read_line(void) { return fread_line(stdin); }
