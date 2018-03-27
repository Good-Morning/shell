#pragma once

#include "parser.h"

extern int max_size_path;
extern char* cur_dir;
extern char* path;
extern char* home;

char* get_cur_dir(char** envp);
void launch(struct parsed_data args, char** envp, struct parsed_data pathes);
char* get_env_var(char* variable, char** envp);
void cd(struct parsed_data args, char** envp);

