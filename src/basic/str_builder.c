#include "str_builder.h"

void str_builder_add(StrBuilder *sb, const str *string) {
    array_extend_from(sb, string);
}

str str_builder_get(StrBuilder *sb) {
    *array_add(sb) = '\0';
    --sb->length;

    str result;
    str_init(&result, sb->data, sb->length);

    return result;
}
