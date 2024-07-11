#include "str_builder.h"
#include "array.h"

void str_builder_add(StrBuilder *sb, const str *string) {
    array_extend_from(sb, string);
}

str str_builder_get(StrBuilder *sb) {
    array_reserve(sb, sb->length + 1);
    sb->data[sb->length] = '\0';

    str result;
    str_init(&result, sb->data, sb->length);

    return result;
}
