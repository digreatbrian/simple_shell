#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "main.h"

int main(int argc, char **argv)
{
	char *command;
	char **command_tmp;
	char *real_command;
	char *abs_command;
	char *cmd_buffer;
	int cmd_status;
	char *shell_name = argv[0]; /* name of our shell program */
	ssize_t cmd_chars_read;		/* Characters read by getline */
	size_t max_cmd_length = 64; /* max length for a command including arguments */
	int shell_started = false;
	char **splitted_cmd;
	int x;
	int is_eof = false;
	int was_eof = false;
	char *env_var;
	char *env_val;
	char *cd_path;

	cmd_buffer = (char *)malloc(sizeof(char) * max_cmd_length);

	if (cmd_buffer == NULL)
	{
		perror(shell_name);
	}

	while (true)
	{
		if (!shell_started)
		{
			_print("\n");
		}

		fflush(stdin);
		fflush(stderr);
		fflush(stdout);

		if (!is_eof || !shell_started)
		{
			shell_started = true;

			if (was_eof)
			{
				was_eof = false;
				cmd_status = exec_command("^[[D"); 

				if (cmd_status == -1)
				{
					/* cmd was unsuccessful*/
					perror(shell_name);
				}
				continue;
			}
			else
			{
				_print("$ ");
			}
		}
		else
		{
			_print("^[[D");
			was_eof = true;
		}

		cmd_chars_read = getline(&cmd_buffer, &max_cmd_length, stdin);

		if (cmd_chars_read == -1)
		{
			/* handling EOF */
			is_eof = true;
			continue;
		}

		is_eof = false;
		real_command = str_strip(cmd_buffer); /* cmd with args if any eg ls -l */

		/* if nothing was entered */
		if (!real_command || cmd_chars_read < 1 || str_cmp(real_command, ""))
		{
			free(real_command);
			continue;
		}

		command_tmp = str_split(real_command, " ", 1);
		command = command_tmp[0]; /* cmd stripped of args eg ls but ls -l entered*/

		/* checking if command exists */
		abs_command = get_absolute_executable_path(command);

		if (abs_command)
		{
			/**
			 *  command exists somewhere ,lets create a child process
			 *
			 */
			pid_t child_pid;
			int pid_status;

			child_pid = fork();

			if (child_pid == -1)
			{
				/* failed to fork a child process */
				free(command_tmp);
				free(real_command);
				continue;
			}

			else if (child_pid != 0)
			{
				/* lets wait a bit */
				pid_status = waitpid(child_pid, NULL, WNOHANG);
				waitpid(child_pid, NULL, 0);

				if (pid_status != 0)
				{
					child_pid = fork();
				}
			}

			else if (child_pid == 0)
			{
				getpid();
				cmd_status = exec_command(real_command);
				
				if (cmd_status == -1)
				{
					/* cmd was unsuccessful*/
					perror(shell_name);
				}
			}
			free(command_tmp);
			free(real_command);
		}
		else
		{
			splitted_cmd = str_split(real_command, " ", 2);
			
			if (str_cmp(splitted_cmd[0], "exit"))
			{
				if (splitted_cmd[1])
				{
					x = str_to_int(splitted_cmd[1]);
					free(command_tmp);
					free(real_command);
					free(splitted_cmd);
					exit(x);
				}
				else
				{
					free(command_tmp);
					free(real_command);
					free(splitted_cmd);
					exit(0);
				}
			}

			else if (str_cmp(splitted_cmd[0], "setenv"))
			{
				int k;
				env_var = splitted_cmd[1];
				env_val = splitted_cmd[2];

				if (!env_var || !env_val)
				{
					free(command_tmp);
					free(real_command);
					free(splitted_cmd);
					continue;
				}

				k = set_env(env_var, env_val);
				free(command_tmp);
				free(real_command);
				free(splitted_cmd);

				if (k == -1)
				{
					perror(shell_name);
				}
				continue;
			}

			else if (str_cmp(splitted_cmd[0], "unsetenv"))
			{
				env_var = splitted_cmd[1];

				if (!env_var)
				{
					free(command_tmp);
					free(real_command);
					free(splitted_cmd);
					continue;
				}

				unset_env(env_var);
				free(command_tmp);
				free(real_command);
				free(splitted_cmd);
				continue;
			}

			else if (str_cmp(splitted_cmd[0], "cd"))
			{
				cd_path = splitted_cmd[1];

				if (!cd_path)
				{
					/* cd to $HOME coz no path provided */
					cd_path = get_env("HOME");
				}
				chdir(cd_path);
				set_env("PWD", cd_path);
				free(command_tmp);
				free(real_command);
				free(splitted_cmd);
				continue;
			}

			/*Else run it as any other command */
			cmd_status = exec_command(real_command);

			free(command_tmp);
			free(real_command);
			free(splitted_cmd);

			if (cmd_status == -1)
			{
				/* cmd was unsuccessful*/
				perror(shell_name);
			}
		}
	}

	if (abs_command)
	{
		free(abs_command);
	}
	free(cmd_buffer);
	(void)argc;
	return (0);
}
