#ifndef SIMPLE_SHELL_H
#define SIMPLE_SHELL_H
#define my_environment  environ

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>

extern char **environ;

/**
 * struct str_index - struct for index range of a string
 * @start: starting index for target string
 * @end: ending index for target string
 */
struct str_index
{
	int start;
	int end;
};
typedef struct str_index str_index;
typedef struct str_index sep_index;

sep_index *get_sep_indexes(char *string, char *sep, int max_indexes);
int count_sep_indexes(sep_index *sep_indexes);

int str_array_memcpy(char *_dest[], char *_src[]);
void free_str_array(char **arr);
int is_file(const char *filepath);
int is_dir(char *path);
int set_env(const char *env_variable, const char *env_value);
int unset_env(const char *env_variable);
char *get_env(const char *env_variable);
int _putchar(int c);
int _err_putchar(int c);
void _print(char *string);
void _err_print(char *string);
void str_array_print(char **arr, char *sep);
char *str_add(char *a, char *b);
void str_array_print(char **arr, char *sep);
int str_cmp(char *a, char *b);
int str_len(char *string);
int str_array_len(char **array);
char *str_sub(const char *string_array, int start_index, int end_index);
char **str_split(char *string, char *sep, int max_splits);
char *get_absolute_executable_path(char *executable);
int exec_command(char *cmd);
char *str_lower(const char *string);
char *str_strip_start(char *string);
char *str_strip_end(char *string);
char *str_strip(char *string);
int str_to_int(char *string);

#endif
