#pragma once

// print 'error_str' to stderr
void error(const char* error_str);

// print 'error_str' to stderr and interrupt execution of the programme
void critical(const char* error_str);

// common message to inform about a lack of memory
extern const char* OUT_OF_MEMORY;

