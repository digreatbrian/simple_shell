#include <unistd.h>
#include <stdlib.h>
#include "main.h"

/**
 * exec_command - executes a command
 * @cmd: command with(out) arguments
 * this doesn't handle piping / pipes and && ,other similar opertions
 * Return: return value for the command
 */

int exec_command(char *cmd)
{
	int max_command_args = 10;
	char **cmd_array = str_split(cmd, " ", max_command_args);
	char *filename = cmd_array[0];
	char **envp = environ;

	if (!is_file(filename))
	{
		/* executable path not complete lets try to look for it */
		filename = get_absolute_executable_path(filename);
		if (filename)
		{
			cmd_array[0] = filename;
		}
	}
	return (execve(cmd_array[0], cmd_array, envp));
}

