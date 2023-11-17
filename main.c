#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "main.h"

void brian(void);

/**
 * run_command - Runs our commands
 * @shell_name: Name of our shell
 * @real_command: Real command
 */

void run_command(char *real_command, char *shell_name)
{
	char *command;
	char *real_cmd;
	char **command_tmp;
	char **splitted_cmd;
	char *abs_command;
	char *env_var;
	char *env_val;
	char *cd_path;
	int cmd_status;
	int x;

	real_cmd = str_strip(real_command);
	command_tmp = str_split(real_cmd, " ", 1);
	command = str_strip(command_tmp[0]); /**
					      * cmd stripped of args eg ls but
					      * ls -l entered
					      */

	/* checking if command exists */
	abs_command = get_absolute_executable_path(command);

	if (abs_command)
	{
		/**
		 * command exists somewhere ,lets create a child process
		 */

		pid_t child_pid;
		int pid_status;

		child_pid = fork();

		if (child_pid == -1)
		{
			/* failed to fork a child process */
			free(command_tmp);
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
			cmd_status = exec_command(real_cmd);

			if (cmd_status == -1)
			{
				/* cmd was unsuccessful*/
				perror(shell_name);
			}
		}
		free(command_tmp);
	}
	else
	{
		splitted_cmd = str_split(real_cmd, " ", 2);

		if (str_cmp(splitted_cmd[0], "exit"))
		{
			if (splitted_cmd[1])
			{
				x = str_to_int(splitted_cmd[1]);
				free(command_tmp);
				free(splitted_cmd);
				exit(x);
			}
			else
			{
				free(command_tmp);
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
				free(splitted_cmd);
				_err_print(str_add(shell_name,
			": Command syntax: setenv ENV_VARIABLE ENV_VALUE\n"));
			}
			else
			{
				k = set_env(env_var, env_val);
				free(command_tmp);
				free(splitted_cmd);

				if (k == -1)
				{
					perror(shell_name);
				}
			}
		}

		else if (str_cmp(splitted_cmd[0], "unsetenv"))
		{
			env_var = splitted_cmd[1];

			if (!env_var)
			{
				free(command_tmp);
				free(splitted_cmd);
				_err_print(str_add(shell_name,
				": Command syntax: unsetenv ENV_VARIABLE\n"));
			}
			else if (get_env(env_var) == NULL)
			{
				free(command_tmp);
				free(splitted_cmd);
				_err_print(str_add(shell_name,
			": Error: Trying to set an unexisting ENV_VARIABLE\n"));
			}
			else
			{
				unset_env(env_var);
				free(command_tmp);
				free(splitted_cmd);
			}
		}

		else if (str_cmp(splitted_cmd[0], "cd"))
		{
			cd_path = splitted_cmd[1];

			if (!cd_path)
			{
				/* cd to $HOME coz no path provided */
				cd_path = get_env("HOME");
			}

			else if (str_cmp(cd_path, "-"))
			{
				cd_path = get_env("OLDPWD");
				if (cd_path)
				{
					chdir(cd_path);
					set_env("PWD", cd_path);
					_print(cd_path);
					_print("\n");
				}
				else
				{
					_print("");
				}
			}
			else
			{
				chdir(cd_path);
				set_env("PWD", cd_path);
			}
			free(command_tmp);
			free(splitted_cmd);
		}
		else
		{
			/* Run it as any other command */
			cmd_status = exec_command(real_cmd);

			free(command_tmp);
			free(splitted_cmd);

			if (cmd_status == -1)
			{
				/* cmd was unsuccessful*/
				perror(shell_name);
			}
		}
	}
}

/**
 * main - Entry point
 * @argc: Argument count
 * @argv: Arguments List
 * Return: 0
 */
int main(int argc, char **argv)
{
	char **splitted_cmd;
	char **splitted_cmds;
	char *cmd_buffer;
	char *command;
	char *arg1;
	char **splitted_file_content;
	char *file_content_buffer;
	char *fd_buffer;
	char *shell_name = argv[0]; /* name of our shell program */
	int x, y, fd;
	int cmd_chars_read;
	int shell_started = false;
	int file_read_buffer;
	int max_file_read_buffer = 800;
	int max_file_cmds = 20;/* max cmds allowed in a file */
	size_t max_cmd_length = 64; /* max len of a command including args */

	cmd_buffer = (char *)malloc(sizeof(char) * max_cmd_length);
	fd_buffer = malloc(sizeof(char) * max_file_read_buffer + 1);
	splitted_cmds = malloc(sizeof(char) * max_file_cmds + 1);

	if (cmd_buffer == NULL || fd_buffer == NULL || splitted_cmds == NULL)
	{
		perror(shell_name);
	}

	/* checking if filename wasnt parsed as an arg */
	if (argc == 2)
	{
		arg1 = argv[1];

		if (is_file(arg1))
		{
			/* this is a file , its accessible */
			fd = open(arg1, O_RDONLY);

			if (fd == -1)
			{
				free(cmd_buffer);
				perror(shell_name);
				exit(-1);
			}

			/* reading file content */
			file_content_buffer = malloc(sizeof(char) *
					max_file_read_buffer + 1);

			if (file_content_buffer == NULL)
			{
				free(cmd_buffer);
				perror(shell_name);
				exit(-1);
			}

			file_read_buffer = read(fd, file_content_buffer,
					max_file_read_buffer);

			file_content_buffer[file_read_buffer] = '\0';

			/* now executing content */
			splitted_file_content = str_split(str_strip
				(file_content_buffer), "\n", max_file_cmds);

			for (x = 0 ; x < str_array_len(splitted_file_content) ;)
			{
				command = splitted_file_content[x];
				command = str_strip(command);

				if (!str_cmp("", command))
				{
					run_command(command, shell_name);
				}
				x++;
			}
			close(fd);
			return (0);
		}
	}

	/* handling piping */
	if (!isatty(STDIN_FILENO))
	{
		cmd_chars_read = getline(&cmd_buffer, &max_cmd_length, stdin);

		if (cmd_chars_read == -1)
		{
			/* handling EOF */
			exit(-1);
		}
		cmd_buffer[cmd_chars_read] = '\0';
		cmd_buffer = str_strip(cmd_buffer);
		splitted_cmd = str_split(cmd_buffer, "  ", max_file_cmds);

		for (x = 0; x < str_array_len(splitted_cmd);)
		{
			command = splitted_cmd[x];
			command = str_strip(command);

			if (!str_cmp("", command))
			{
				run_command(command, shell_name);
			}
			x++;
		}
		return (0);
	}

	while (true)
	{
		if (shell_started == false)
		{
			shell_started = true;
		}

		fflush(stdin);
		fflush(stdout);
		fflush(stderr);

		_print("$ ");

		cmd_chars_read = getline(&cmd_buffer, &max_cmd_length, stdin);

		if (cmd_chars_read == -1)
		{
			/* handling EOF */
			exit(-1);
		}
		cmd_buffer[cmd_chars_read] = '\0';
		command = str_strip(cmd_buffer); /**
						  * cmd with args if any eg ls
						  * -l
						  */

		/* if nothing was entered */
		if (!command || cmd_chars_read < 1 || str_cmp(command, ""))
		{
			free(command);
			continue;
		}

		/* trying to handle ';' using 10 as maxcount*/
		splitted_cmd = str_split(command, ";", 10);

		if (splitted_cmd[1])
		{
			for (y = 0 ; y < 10 ;)
			{
				command = splitted_cmd[y];
				if (!command)
				{
					break;
				}
				else
				{
					run_command(command, shell_name);
				}
				y++;
			}
		}
		else
		{
			run_command(command, shell_name);
		}
	}
	free(cmd_buffer);
	(void)argc;
	return (0);
}

