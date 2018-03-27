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

char* get_cur_dir(char** env) {
	size_t size = 1;
	char* buffer = malloc(size * sizeof(char));
	if (buffer == 0) {
		critical("Out of memory");
	}
	while (syscall(SYS_getcwd, buffer, size) == -1) {
		free(buffer); 
		size*=2; 
		if (0 == (buffer = malloc(size * sizeof(char)))) {
			critical("Out of memory");
		}
	}
	return buffer;
}
int max_size_path = 0;
char* cur_dir;
char* path;
char* home;

void launch(struct parsed_data args, char** envp, struct parsed_data pathes) {
	pid_t pid = fork();
	
	if (pid == -1) {
		error("Unable to fork"); 
		return;
	}
	
	if (!pid) { // child
		int error_code = 0;
		if (index(args.array[0], '/')) { // no 'path' attempts
			execve(args.array[0], args.array, envp);
			error_code = errno;
		} else { // attempt to suit 'path' substrings
			char* temp = malloc((2 + max_size_path + strlen(args.array[0])) * sizeof(char));
			if (temp == 0) {
				critical("Out of memory. Try not to use $PATH");
			}
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
		critical(strerror(error_code));
	} else {    // host
		int exit_code;
		if (waitpid(pid, &exit_code, WUNTRACED | WCONTINUED) == 0) {
			int error_code = errno;
			error(strerror(error_code));
		}
		if (exit_code) {
			printf("exit code: %d\n", exit_code);
		}
	}
}

char* get_env_var(char* variable, char** envp) {
	char** varp = envp; 
	int size = strlen(variable);

	while (strncmp(*varp, variable, size) || (*varp)[size] != '=') {
		varp++;
	}
	return *varp + size + 1;
}

void cd(struct parsed_data args, char** env) {
	if (args.count < 2) {
		error("argument is missing");
		return;
	}

	if (!strcmp(args.array[1], "~")) {
		args.array[1] = home; 
	}
	if (chdir(args.array[1])) {
		error("no such directory");
	} else {
		free(cur_dir);
		cur_dir = get_cur_dir(env);
	}
	return;
}


