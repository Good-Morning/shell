#include "../headers/parser.h"

#include "../headers/error.h"

char* _parse_source;
size_t* _parse_count;
char _parse_delimiter;
char** _parse_temp;
char** _parse() {
	if (_parse_source == 0) {
		return 0;
	}

	while (*_parse_source == _parse_delimiter) {
		*_parse_source = 0;
		_parse_source++;
	}
	if (*_parse_source == 0 || *_parse_source == '\n') {
		*_parse_source = 0;
		_parse_temp = malloc((*_parse_count + 1) * sizeof(char*));
		if (_parse_temp == 0) {
			error(OUT_OF_MEMORY);
			return 0;
		}
		_parse_temp[*_parse_count] = 0;
		return _parse_temp + *_parse_count - 1;
	}
	char* cur = _parse_source;
	while (
		*_parse_source != _parse_delimiter && 
		*_parse_source != '\n' && 
		*_parse_source != 0
	) {
		_parse_source++;
	}
	(*_parse_count)++;
	_parse_temp = _parse();
	if (_parse_temp == 0) {
		return 0;
	}
	_parse_temp[0] = cur;
	return _parse_temp - 1;
}

struct parsed_data parse(char* source, char delimiter) {
	struct parsed_data result;
	result.count = 0;
	_parse_count = &result.count;
	_parse_source = source;
	_parse_delimiter = delimiter;
	result.array = _parse(source, &result.count, delimiter);
	if (result.array == 0) {
		result.count = 0;
		result.array = 0;
	}
	result.array = result.array + 1;
	return result;
}


