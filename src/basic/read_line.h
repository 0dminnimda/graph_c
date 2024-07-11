#pragma once

#ifndef READ_LINE_H
#define READ_LINE_H

#include <stdio.h>

#include "str.h"
#include "str_builder.h"

void fread_line_use_buffer(FILE *stream, StrBuilder *sb);
str fread_line(FILE *stream);
void read_line_use_buffer(StrBuilder *sb);
str read_line(void);

#endif  // READ_LINE_H
