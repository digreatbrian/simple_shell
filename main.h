#ifndef SIMPLE_SHELL_H
#define SIMPLE_SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* Reads/writes buffers */

#define READ_BUFF_SIZE          1024
#define WRITE_BUFF_SIZE         1024
#define BUFF_FLUSH              -1

/* When command chaining */

#define CMD_NORM                0
#define CMD_OR                  1
#define CMD_AND                 2
#define CMD_CHAIN               3

/* 1 if we are using system getline() */

#define USE_GETLINE             0
#define USE_STRTOK              0

extern char **environ;

/**
 * struct liststr - A singly linked list
 * @nf: Number field
 * @st: Field of node
 * @stri: String
 * @next: Next node
 */

typedef struct liststr
{
	int nf;
	char *stri;
	char *st;
	struct liststr *next;
} list_t;

/**
 * struct giveInfo - Pseudo args to give to a function
 * @arg: String made from getline containing args
 * @argv: Array of strings generated from args
 * @line_count: Error count
 * @linecount_flag: Counts this line of input if it's on
 * @history: History node
 * @status: Return status of the last executed command
 * @cmdBuffer: Pointer to cmdBuffer address, on if chaining
 * @cmdBufferType: CMD_type ||, &&, ;
 * @readfd: File descriptor from which to read line input
 * @histcount: History line number count
 */

typedef struct giveInfo
{
	char *arg;
	char **argv;
	unsigned int line_count;
	int linecount_flag;
	list_t *history;
	int status;
	int cmdBufferType;		/* CMD_type ||, &&, ; */
	char **cmdBuffer; /* cmd pointer, chain buffer, for memory management */
	int readfd;
	int histcount;
} info_t;

void _puts(char *);

char *strnCopy(char *, char *, int);
char *strnConcat(char *, char *, int);
char *strChar(char *, char);

int bfree(void **);

char *memorySet(char *, char, unsigned int);

char *strDuplicate(const char *);

list_t *addNodeEnd(list_t **, const char *, int);

void *alloc(void *, unsigned int, unsigned int);

void rmComments(char *);

ssize_t getsInput(info_t *);
int getsLine(info_t *, char **, size_t *);
void hintIntHandler(int);

int buildHistoryList(info_t *inf, char *buffer, int countLine);

int isChain(info_t *, char *, size_t *);
void checksChain(info_t *, char *, size_t *, size_t, size_t);

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
