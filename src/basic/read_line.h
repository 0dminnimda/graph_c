#pragma once

#ifndef READ_LINE_H
#define READ_LINE_H

#include <stdio.h>

#include "str.h"

str fread_line(FILE *stream);
str read_line(void);

#endif  // READ_LINE_H
