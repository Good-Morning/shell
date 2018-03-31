#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "../headers/parser.h"
#include "../headers/system.h"
#include "../headers/string.h"

// boolean constants
typedef ssize_t bool;
const bool true = -1;
const bool false = 0;

// get maximum of 2 integers
ssize_t max(ssize_t a, ssize_t b) {
	return a < b ? b : a;
}

int main (int argc, char *argv[], char *envp[]) {
	
	// pointers to environment constants
	path = get_env_var("PATH", envp);
	home = get_env_var("HOME", envp);
	
	// storage for pathes from $path
	char* const path_data = strdup(path);

	// getting pathes from $path and their maximum sizes
	struct parsed_data pathes = parse(path_data, ':');
	for (size_t i = 0; i < pathes.count; i++) {
		max_size_path = max(max_size_path, strlen(pathes.array[i]));
	}

	// if 'true' then terminate 'cmd'
	bool terminated = false;

	// to store user's command
	char *st = 0; 

	// storage for parsed arguments and programme name
	struct parsed_data args;
	args.array = 0;
	
	// current path to cwd
	cur_dir = get_cur_dir();

	// main loop
	while (!terminated) {	

		//free argument storage before using it
		if (args.array) {
			free(st);
			free(args.array);
			args.array = 0;
		}
		
		// welcome user to type command
		printf("L:%s> ", cur_dir);
		fflush(stdout);
		if ((st = get_line()) == 0) {
			terminated = true;
			continue;
		}

		// extract substrings from 'st' separated by ' '(space)
		args = parse(st, ' ');

		// if the input is empty
		if (args.count == 0) {
			continue;
		}
		
		// if 'exit' command occurred
		if (!strcmp("exit", args.array[0])) {
			terminated = true;
			continue;
		}

		// if 'cd'(change directory) command occurred
		if (!strcmp("cd", args.array[0])) {
			cd(args);
			continue;
		}
		
		// if no special command occurred
		// then try to execute
		launch(args, envp, pathes);
	}

	// free resources
	free((void*)path_data);
	free((void*)pathes.array);
	free((void*)cur_dir);
	if (args.array) {
		free(args.array);
		free(st);
	}
	
	// everything is OK
	return EXIT_SUCCESS;
}

