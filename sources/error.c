#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void error(const char* error_str) {
	if (-1 == write(2, error_str, strlen(error_str))) {
		// ignore errors occurring while printing errors
	}
	if (-1 == write(2, "\n", 1)) {
		// ignore errors occurring while printing errors
	}
}

void critical(const char* error_str) {
	error(error_str);
	exit(EXIT_FAILURE);
}

const char* OUT_OF_MEMORY = "Out of memory";


