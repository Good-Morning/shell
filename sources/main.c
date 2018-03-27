#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "../headers/parser.h"
#include "../headers/system.h"

const int true = -1;
const int false = 0;

int max(int a, int b) {
	return a < b ? b : a;
}

int main (int argc, char *argv[], char *envp[]) {
	path = get_env_var("PATH", envp);
	home = get_env_var("HOME", envp);
	
	char* path_data = strdup(path);
	struct parsed_data pathes = parse(path_data, ':');
	for (int i = 0; i < pathes.count; i++) {
		max_size_path = max(max_size_path, strlen(pathes.array[i]));
	}

	int terminated = false;
	char *st = 0;
	size_t size;
	struct parsed_data args;
	args.array = 0;
	
	cur_dir = get_cur_dir(envp);
	while (!terminated) {	
		if (args.array) {
			free(args.array);
			args.array = 0;
		}

		printf("L:%s> ", cur_dir);
		getline(&st, &size, stdin);

		args = parse(st, ' ');

		if (args.count == 0) {
			continue;
		}
		
		if (!strcmp("exit", args.array[0])) {
			terminated = true;
			continue;
		}
		if (!strcmp("cd", args.array[0])) {
			cd(args, envp);
			continue;
		}

		launch(args, envp, pathes);
	}

	free(path_data);
	free(pathes.array);
	free(cur_dir);
	cur_dir = 0;
	if (args.array) {
		free(args.array);
		args.array = 0;
	}
	
	return EXIT_SUCCESS;
}

