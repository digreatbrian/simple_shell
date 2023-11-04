#ifndef SIMPLE_SHELL_H
#define SIMPLE_SHELL_H
int is_file(const char *filepath);
int is_dir(char *path);
int set_env(const char *env_variable, const char *env_value);
int unset_env(const char *env_variable);
char *get_env(const char *env_variable);
int _putchar(int c);
void _print(char *string);
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
