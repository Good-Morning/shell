#include "../headers/string.h"

#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/error.h"

char buffer[256];
char* pointer = buffer;
size_t available = 0;

char get_char() {
	if (available == 0) {
		ssize_t size_read = read(0, buffer, 255);
		if (size_read == -1) {
			error(strerror(errno));
			return '\0';
		}
		if (size_read == 0) {
			return '\04';
		}
		pointer = buffer;
		available = size_read;
	}

	available--;
	return *pointer++;
}

char* get_line() {
	size_t size_buffer = 0;
	size_t capacity_buffer = 32;
	char* buffer = malloc(capacity_buffer * sizeof(char));

	for (char c = ' '; c != '\0' && c != '\n';) {
		c = get_char();
		if (c == '\04') {
			free(buffer);
			return 0;
		}
		if (size_buffer == capacity_buffer - 1) {
			char* temp = malloc(capacity_buffer * 2 * sizeof(char));
			memcpy(temp, buffer, size_buffer * sizeof(char));
			free(buffer);
			buffer = temp;
			capacity_buffer *= 2;
		}
		buffer[size_buffer++] = c;
	}
	if (buffer[size_buffer] == '\n') {
		buffer[size_buffer + 1] = 0;
	}

	return buffer;
}

