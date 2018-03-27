#pragma once

#include <stdlib.h>

struct parsed_data { size_t count; char** array; }
parse(char* string, char delimiter);

