#include "../headers/system.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include "../headers/parser.h"
#include "../headers/error.h"

char* get_cur_dir() {

	// init buffer for $cwd of size 4	
	size_t size = 4;
	char* buffer = malloc(size * sizeof(char));
	if (buffer == 0) {
		critical(OUT_OF_MEMORY);
	}
	
	// enladge buffer until $cwd fits it
	while (syscall(SYS_getcwd, buffer, size) == -1) {
		free(buffer); 
		size *= 2; 
		if (0 == (buffer = malloc(size * sizeof(char)))) {
			critical(OUT_OF_MEMORY);
		}
	}
	return buffer;
}

int max_size_path = 0;
char* cur_dir;
char* path;
char* home;

void launch(struct parsed_data args, char** envp, struct parsed_data pathes) {
	
	const pid_t pid = fork();
	
	// if can not fork
	if (pid == -1) {
		error("Unable to fork"); 
		return;
	}
	
	if (!pid) { 
		// child

		// storage for exit code of executee
		int error_code = 0;

		// if there is some sort of path
		if (index(args.array[0], '/')) { 
			// make no '$path' attempts
			
			execve(args.array[0], args.array, envp);
			error_code = errno;
		} else { 
			// attempt to suit 'path' substrings

			// allocate big enough buffer to contain every 'path/command'
			const size_t requested_size = max_size_path + strlen(args.array[0]);
			// 1 more char for '/'; 1 more char for null-terminal
			char* temp = malloc((2 + requested_size) * sizeof(char));
			if (temp == 0) {
				error(OUT_OF_MEMORY);
				critical("Try not to use $PATH");
			}

			// try to execute command by every path from $path 
			for (int i = 0; i < pathes.count; i++) {
				strcpy(temp, pathes.array[i]);
				strcat(temp, "/");
				strcat(temp, args.array[0]);
				execve(temp, args.array, envp);
				error_code = errno;
			}

			free(temp);
		}
		free(args.array);

		// never reaches this if any suitable path is found
		critical(strerror(error_code));
	} else {    
		// host
		
		// storage for exit code of child (and its executee)
		int exit_code;

		// wait for the child to end
		if (waitpid(pid, &exit_code, WUNTRACED | WCONTINUED) == 0) {
			int error_code = errno;
			error(strerror(error_code));
		}

		// print exit code if something seems to be wrong
		if (exit_code) {
			printf("exit code: %d\n", exit_code);
		}
	}
}

char* get_env_var(char* variable, char** envp) {

	// init search iterator 
	char** varp = envp; 
	// size of 'variable'
	int size = strlen(variable);

	// iterate through the environment to find 'variable'
	while (strncmp(*varp, variable, size) || (*varp)[size] != '=') {
		varp++;
	}

	// return pointer to 'variable' stored in 'envp'
	// with 'variable=' is skipped
	return *varp + size + 1;
}

void cd(struct parsed_data args) {

	// cd requires 1 argument
	if (args.count < 2) {
		error("argument is missing");
		return;
	}

	// if home directory is required
	if (!strcmp(args.array[1], "~")) {
		args.array[1] = home; 
	}

	// try to change directory
	if (chdir(args.array[1])) {
		// failure
		error("no such directory");
	} else {
		// success
		
		// write to 'cur_dir' new path to cwd
		free(cur_dir);
		cur_dir = get_cur_dir();
	}
}


