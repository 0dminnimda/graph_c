#include <stdio.h>

#include "basic/read_line.h"
#include "basic/str.h"

int main(void) {
    printf("Hello seamen\n");

    while (1) {
        str line = read_line();
        if (line.data == NULL || line.length == 0) {
            fprintf(stderr, "Error while reading the line\n");
            str_deinit(&line);
            return 1;
        }

        if (str_compare(&line, &str_lit("q")) == 0) {
            str_deinit(&line);
            break;
        }

        printf("Got string " PRI_str "\n", FMT_str(&line));

        str_deinit(&line);
    }

    return 0;
}
