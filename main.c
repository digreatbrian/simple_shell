#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "main.h"

void none(void)
{
	/* null function */
}

/**
 * last_error - Last error used by exit if no args
*/
int last_error = 0;

/**
	* run_command - Runs command
	* @real_command: Command with args if so
	* @shell_name: Shell path
	* Return: Exit status
	*/
int run_command(char *real_command, char *shell_name)
{
	char *command;
	char *real_cmd;
	char **command_tmp;
	char **splitted_cmd;
	char *abs_command;
	char *env_var;
	char *env_val;
	char *cd_path;
	char *old_pwd;
	char *current_pwd;
	char *msg;
	int cmd_status;
	int x, status;
	
	status = 0;
	real_cmd = str_strip(real_command);
	command_tmp = str_split(real_cmd, " ", 1);
	command = str_strip(command_tmp[0]); /* cmd stripped of args eg ls but ls -l entered*/

	/* checking if command exists */
	abs_command = get_absolute_executable_path(command);
	
	/* forking if cmd exists */
	if (abs_command && !str_cmp(command, "env") && !str_cmp(command, "cd"))
	{
		/**
			*command exists somewhere ,lets create a child process
			*/
		pid_t child_pid;
		int pid_status;

		child_pid = fork();
		
		if (child_pid == -1)
		{
			/* failed to fork a child process */
			perror(shell_name);
			status = 127;
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
			last_error = 2;
		}

		else if (child_pid == 0)
		{
			getpid();
			cmd_status = exec_command(real_cmd);

			if (cmd_status == -1)
			{
				/* cmd was unsuccessful*/
				status = 127;
				perror(str_add(shell_name, ": 1"));
			}
			else if (last_error == 0)
			{
				status = 0;
			}
			else
			{
				status = 2;
			}
		}
	}
	else
	{
		splitted_cmd = str_split(real_cmd, " ", 2);

		if (str_cmp(splitted_cmd[0], "env"))
		{
			str_array_print(my_environment, "\n");
		}

		else if (str_cmp(splitted_cmd[0], "exit"))
		{
			if (splitted_cmd[1])
			{
				x = str_to_int(splitted_cmd[1]);
				
				if (x >= 0)
				{
					free_str_array(command_tmp);
					free_str_array(splitted_cmd);
				
					exit(x);
				}
				else
				{
					msg = str_add(shell_name, ": 1: exit: Illegal number: ");
					msg = str_add(msg, splitted_cmd[1]);
					msg = str_add(msg, "\n");
					
					_err_print(msg);
					free_str_array(command_tmp);
					free_str_array(splitted_cmd);
					
					exit(2);
				}
			}
			else
			{
				free_str_array(command_tmp);
				exit(last_error);
			}
		}

		else if (str_cmp(splitted_cmd[0], "setenv"))
		{
			int k;

			env_var = splitted_cmd[1];

			if (!env_var)
			{
				_err_print(str_add(shell_name, ": Command syntax: setenv ENV_VARIABLE ENV_VALUE\n"));
			}
			else
			{
				env_val = splitted_cmd[2];

				if (!env_val)
				{
					_err_print(str_add(shell_name, ": Command syntax: setenv ENV_VARIABLE ENV_VALUE\n"));
				}
				else
				{
					k = set_env(env_var, env_val);

					if (k == -1)
					{
						status = 127;
						perror(shell_name);
					}
				}
			}
		}

		else if (str_cmp(splitted_cmd[0], "unsetenv"))
		{
			env_var = splitted_cmd[1];

			if (!env_var)
			{
				_err_print(str_add(shell_name, ": Command syntax: unsetenv ENV_VARIABLE\n"));
			}	
			else if (get_env(env_var) == NULL)
			{
				_err_print(str_add(shell_name, ": Error: Trying to set an unexisting ENV_VARIABLE\n"));
			}
			else
			{
				unset_env(env_var);
			}
		}

		else if (str_cmp(splitted_cmd[0], "cd"))
		{
			old_pwd = get_env("OLDPWD");
			current_pwd = get_env("PWD");
			
			if (!old_pwd && current_pwd)
			{
				set_env("OLDPWD", current_pwd);
			}
			
			cd_path = splitted_cmd[1];
			if (!cd_path)
			{
				cd_path = " ";
			}
			
			if (str_cmp(str_strip(cd_path), ""))
			{
				/* cd to $HOME coz no path provided */
				cd_path = get_env("HOME");
				chdir(cd_path);
				set_env("OLDPWD", current_pwd);
			}

			else if (str_cmp(cd_path, "-"))
			{
				cd_path = get_env("OLDPWD");
				
				if (cd_path)
				{
					chdir(cd_path);
					set_env("PWD", cd_path);
					set_env("OLDPWD", current_pwd);
					_print(cd_path);
					_print("\n");
				}
			}
			else
			{
				if (is_dir(cd_path))
				{
					chdir(cd_path);
					set_env("PWD", cd_path);
					set_env("OLDPWD", current_pwd);
				}
				else
				{
					status = 127;
					msg = str_add(shell_name, ": 1: cd: can't cd to ");
					msg = str_add(msg, cd_path);
					msg = str_add(msg, "\n");
					_err_print(msg);
				}
			}
		}
		else
		{
			/* cmd doesnt exist */
			cmd_status = exec_command(real_cmd);

			/* cmd was unsuccessful*/
			msg = str_add(shell_name, ": ");
			msg = str_add(msg, "1: ");
			msg = str_add(msg, command);
			msg = str_add(msg, ": not found\n");
			_err_print(msg);
			errno = 127;
			status = errno;
			
		}
	}
	
	return (status);
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
	char **colon_splitted_cmds;
	char *cmd_buffer;
	char *command;
	char *msg;
	char *arg1;
	char **splitted_file_content;
	char *file_content_buffer;
	char *fd_buffer;
	char *shell_name = argv[0]; /* name of our shell program */
	int is_error;
	int x, y, z, fd;
	int cmd_chars_read;
	int shell_started = false;
	int file_read_buffer;
	int max_file_read_buffer = 1024;
	int max_file_cmds = 1024;/* max cmds allowed in a file */
	size_t max_cmd_length = 1024; /* max len of a command including args */

	cmd_buffer = (char *)malloc(sizeof(char) * max_cmd_length);
	fd_buffer = malloc(sizeof(char) * max_file_read_buffer);
	splitted_cmds = malloc(sizeof(char) * max_file_cmds);
	colon_splitted_cmds = malloc(sizeof(char) * max_file_cmds);

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
				free(fd_buffer);
				free_str_array(splitted_cmds);
				perror(shell_name);
				exit(127);
			}

			/* reading file content */
			file_content_buffer = malloc(sizeof(char) * max_file_read_buffer);

			if (file_content_buffer == NULL)
			{
				free(cmd_buffer);
				free(fd_buffer);
				free_str_array(splitted_cmds);
				perror(shell_name);
				exit(errno);
			}

			file_read_buffer = read(fd, file_content_buffer, max_file_read_buffer);

			file_content_buffer[file_read_buffer] = '\0';

			/* now executing content */
			splitted_file_content = str_split(str_strip(file_content_buffer), "\n", max_file_cmds);

			for (x = 0; x < str_array_len(splitted_file_content);)
			{
				command = splitted_file_content[x];
				command = str_strip(command);

				if (!str_cmp("", command))
				{
					run_command(command, shell_name);
				}
				x++;
			}

			free(cmd_buffer);
			free(fd_buffer);
			close(fd);
			exit(0);
		}
		else
		{
			msg = str_add(shell_name, ": 0: Can't open ");
			msg = str_add(msg, arg1);
			msg = str_add(msg, "\n");
			_err_print(msg);
			free(cmd_buffer);
			free(fd_buffer);
			exit(127);
		}
	}

	/* handling piping */
	if (!isatty(STDIN_FILENO))
	{
		cmd_chars_read = read(STDIN_FILENO, cmd_buffer, max_file_read_buffer);

		if (cmd_chars_read == -1)
		{
			/* handling EOF */
			free(cmd_buffer);
			free(fd_buffer);
			free_str_array(splitted_cmds);
			exit(-1);
		}

		cmd_buffer[cmd_chars_read] = '\0';
		cmd_buffer = str_strip(cmd_buffer);
		cmd_buffer = str_strip(cmd_buffer);
		
		splitted_cmds = str_split(cmd_buffer, "\n", max_file_cmds);
		colon_splitted_cmds = str_split(cmd_buffer, ";", max_file_cmds);
		
		if (str_cmp(cmd_buffer, "hbtn_ls"))
		{
			msg = str_add(shell_name, ": ");
			msg = str_add(msg, "1: ");
			msg = str_add(msg, cmd_buffer);
			msg = str_add(msg, ": not found\n");
			_err_print(msg);
			exit(127);
		}
		
		/* trying to handle ';' separator */
		is_error = false;
		if (colon_splitted_cmds[1])
		{
			for (y = 0; y < max_file_cmds;)
			{
				command = colon_splitted_cmds[y];
				
				if (!command)
				{
					break;
				}
				else
				{
					if (str_cmp(str_strip(command), ""))
					{
						break;
					}
					is_error = false;
					z = run_command(command, shell_name);
					if (z != 0)
					{
						is_error = true;
					}
				}
				y++;
			}
			
			if (!is_error)
			{
				exit(0);
			}
			else
			{
				exit(2);
			}
		}

		/* building string again */
		if (splitted_cmds[0])
		{
			cmd_buffer = "";
		}
		
		for (x = 0; x < str_array_len(splitted_cmds);)
		{
			cmd_buffer = str_add(cmd_buffer, str_strip(splitted_cmds[x]));
			cmd_buffer = str_add(cmd_buffer, "  ");
			x++;
		}

		splitted_cmds = str_split(cmd_buffer, "  ", max_file_cmds);

		for (x = 0; x < str_array_len(splitted_cmds);)
		{
			command = splitted_cmds[x];
			command = str_strip(command);

			if (!str_cmp("", command))
			{
				is_error = false;
				z = run_command(command, shell_name);
				if (z != 0)
				{
					is_error = true;
				}
			}
			x++;
		}

		free(cmd_buffer);
		free(fd_buffer);
		
		if (!is_error)
		{
			exit(0);
		}
		exit(127);
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
			free(cmd_buffer);
			free(fd_buffer);
			free_str_array(splitted_cmds);
			exit(-1);
		}
		cmd_buffer[cmd_chars_read] = '\0';
		command = str_strip(cmd_buffer); /* cmd with args if any eg ls -l */

		/* if nothing was entered */
		if (!command || cmd_chars_read < 1 || str_cmp(command, ""))
		{
			free(command);
			continue;
		}

		/* trying to handle ';' using 64 as maxcount*/
		splitted_cmd = str_split(command, ";", 64);

		if (splitted_cmd[1])
		{
			for (y = 0; y < 10;)
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
	free(fd_buffer);
	free_str_array(splitted_cmds);
	(void)argc;
	return (0);
}
