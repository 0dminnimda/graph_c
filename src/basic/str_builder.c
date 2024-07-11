#include "str_builder.h"

void str_builder_add(StrBuilder *sb, const str *string) {
    array_extend_from(sb, string);
}

str str_builder_finish(StrBuilder *sb) {
    str result;
    str_init(&result, sb->data, sb->length);
    sb->data = NULL;
    sb->length = 0;
    sb->capacity = 0;
    return result;
}

