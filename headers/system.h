#pragma once

#include "parser.h"

extern int max_size_path; // size of the longest path from $path
extern char* cur_dir;     // current path to cwd
extern char* path;        // pointer to $path
extern char* home;        // pointer to $home

// get $cwd
// returned value is expected to be 'free'ed
char* get_cur_dir();

// execute args[0] with arguments args[1..n] with environment 'envp'
// tries to find args[0] in pathes from 'pathes'
void launch(struct parsed_data args, char** envp, struct parsed_data pathes);

// find environmental constant in 'envp' with name 'variable'
char* get_env_var(char* variable, char** envp);

// change current directory
void cd(struct parsed_data args);

