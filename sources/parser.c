#include "../headers/parser.h"

#include "../headers/error.h"

// are constant through the whole parsing cycle
// made not local to provide more space in stack for other data
char* _parse_source;
size_t* _parse_count;
char _parse_delimiter;
char** _parse_temp;

char** _parse() {

	// check for null pointer
	if (_parse_source == 0) {
		return 0;
	}

	// remove all the delimiters with null-terminal
	while (*_parse_source == _parse_delimiter) {
		*_parse_source = 0;
		_parse_source++;
	}

	// end of recursion allocates buffer to store data
	if (*_parse_source == 0 || *_parse_source == '\n') {

		// delete '\n' at the end of the input
		*_parse_source = 0;

		// storage for resulting pointers
		_parse_temp = malloc((*_parse_count + 1) * sizeof(char*));
		if (_parse_temp == 0) {
			error(OUT_OF_MEMORY);
			return 0;
		}

		// last pointer is null by contract
		_parse_temp[*_parse_count] = 0;

		// return pointer to the previous (empty by the moment) datum
		return _parse_temp + *_parse_count - 1;
	}

	// save datum (one of the pointers expected to be returned)
	char* cur = _parse_source;
	// encount one more parsed substring
	(*_parse_count)++;
	
	// skip found substring
	while (
		*_parse_source != _parse_delimiter && 
		*_parse_source != '\n' && 
		*_parse_source != 0
	) {
		_parse_source++;
	}
	
	// continue recursion 
	// (by the time it has:
	//		+1 count of found substrings
	//		a bit less partially computed source (computed part is skipped))
	_parse_temp = _parse();

	// check for null pointer (out of memory case)
	if (_parse_temp == 0) {
		return 0;
	}

	// restore saved pointer in the buffer
	_parse_temp[0] = cur;

	// push up buffer through the recursion
	return _parse_temp - 1;
}

struct parsed_data parse(char* source, char delimiter) {
	
	// init arguments for '_parse'
	struct parsed_data result;
	result.count = 0;
	_parse_count = &result.count;
	_parse_source = source;
	_parse_delimiter = delimiter;

	// use the core algorithm of 'parse'
	result.array = _parse(source, &result.count, delimiter);
	if (result.array == 0) {
		result.count = 0;
		result.array = 0;
	}

	// get rid of specifities of implemetation '_parse'
	result.array = result.array + 1;
	return result;
}


