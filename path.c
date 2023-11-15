#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include "main.h"

void brian(void);

/**
 * is_file - checks whether a file exists or not
 * @filepath: Path to file
 * Return: 0 (if false)  1 (if true)  Other int (unexpected error)
 */

int is_file(const char *filepath)
{
	int fd = open(filepath, O_RDONLY);

	if (fd == -1)
	{
		if ((errno == EACCES) | (errno == EROFS))
		{
			/**
			 * file exists but permission error
			 * or readonly error
			 */
			return (1);
		}

		else if ((errno == ENOENT) | (errno == EISDIR))
		{
			/**
			 * file doesnt exist or this is a directory
			 */
			return (0);
		}

		else
		{
			/* unexpected error */
			return (errno);
		}
	}
	close(fd);
	return (1);
}

/**
 * is_dir - checks whether a file exists or not
 * @path: Path to directory
 * Return: 0 (if false)  1 (if true)  Other int (unexpected error)
 */

int is_dir(char *path)
{
	DIR *dir = opendir(path);

	if (dir == NULL)
	{
		/**
		 * further analysis why we cant open the dir
		 * if only no such file or directory error ,
		 * then we return 0 else 1 or printing error and returning errno
		 */
		if (errno == EACCES)
		{
			/*directory exists but permission error*/
			return (1);
		}

		else if ((errno == ENOENT) | (errno == ENOTDIR))
		{
			/**
			 * directory doesnt exist or this is not a
			 * directory but a file
			 */
			return (0);
		}

		else
		{
			/* unexpected error */
			return (errno);
		}
	}
	closedir(dir);
	return (1);
}

/**
 * get_absolute_executable_path - gets absolute path for an executable
 * @executable: Executable name (eg "ls" )
 * Description: get abs path for an executable, uses PATH
 * Return: absolute path to an executable or null
 */

char *get_absolute_executable_path(char *executable)
{
	char *PATH_ENV;
	char **path_env_array;
	char *_path;
	char *path_env_sep = ":";
	char *path_slash_sep = "/";
	char *new_executable_path;
	char *_executable_temp;
	char *executable_exts[] = {"exe", NULL}; /* executable extensions */
	char *ext;				/* executable extension */
	int executable_exts_len = str_array_len(executable_exts);
	int path_env_max_splits = 100;
	int path_env_len;
	int i, j;

	PATH_ENV = get_env("PATH");

	if (!PATH_ENV)
	{
		return (NULL);
	}
	path_env_array = str_split(PATH_ENV, path_env_sep, path_env_max_splits);
	path_env_len = str_array_len(path_env_array);

	if (is_file(executable))
	{
		/* default executable path provided does exist */
		return (executable);
	}

	for (i = 0; i < path_env_len; i++)
	{
		_path = path_env_array[i];
		_path = str_add(_path, path_slash_sep);

	/* try to find if the executable exists without adding an extension */
		new_executable_path = str_add(_path, executable);

		if (is_file(new_executable_path))
		{
			return (new_executable_path);
		}

		else
		{
			/**
			 * try to find if the executable exists using executable
			 * with extension which means adding an extension to
			 * executable filename
			 */
			free(new_executable_path);

			for (j = 0; j < executable_exts_len; j++)
			{
				/* trying all executable extensions */
				ext = executable_exts[j];
				_executable_temp = str_add(executable, ".");
				_executable_temp = str_add(_executable_temp, ext);
				new_executable_path = str_add(_path, _executable_temp);

				if (is_file(new_executable_path))
				{
					return (new_executable_path);
				}

				free(_executable_temp);
				free(new_executable_path);
			}
			free(_path);
		}
	}
	/* return null coz we found nothing */
	return (NULL);
}

