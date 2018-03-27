#pragma once

#include <stdlib.h>

// returns substrings of 'string' which were separated by 'delimiter'
// return value is expected to be 'free'ed
struct parsed_data { size_t count; char** array; }
parse(char* string, char delimiter);

