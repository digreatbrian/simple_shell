#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"

/**
 * set_env - set environment variable value
 * @env_variable: Environment variable
 * @env_value: Environment variable value
 * Return: 0 on success and -1 on error
 */
int set_env(const char *env_variable, const char *env_value)
{
	char **_env;
	char *env_data;
	char *env_var;
	char *new_env_data;
	char *tmp;
	int i;
	int env_len = str_array_len(my_environment);
	int env_exists = false;
	char **new_env = malloc(sizeof(char *) * (env_len + 2));

	if (new_env == NULL)
	{
		return (-1);
	}

	for (i = 0; i < env_len; i++)
	{
		env_data = my_environment[i];
		_env = str_split(env_data, "=", 1);
		env_var = _env[0];

		if (env_var)
		{
			if (str_cmp(env_var, (char *)env_variable))
			{
				tmp = str_add((char *)env_variable, "=");
				new_env_data = str_add(tmp, (char *)env_value);
				my_environment[i] = new_env_data;
				free(tmp);
				free(_env);
				env_exists = true;
				break;
			}
		}
		free(_env);
	}

	if (!env_exists)
	{
		/*env variable doesnt exist at all */
		tmp = str_add((char *)env_variable, "=");
		new_env_data = str_add(tmp, (char *)env_value);
		new_env[0] = new_env_data;
		
		for (i = 0; i < env_len; i++)
		{
			env_data = my_environment[i];
			new_env[i + 1] = malloc(str_len(env_data) * (sizeof(char) + 1));

			if (new_env[i] == NULL)
			{
				return (-1);
			}
			new_env[i + 1] = env_data;
		}

		new_env[i + 1] = NULL;
		str_array_memcpy(my_environment, new_env);
	}
	str_array_print(my_environment, "\n");
	return (0);
}

/**
 * unset_env - unset environment variable value
 * @env_variable: Environment variable
 * Return: 0 on success and -1 on error
 */
int unset_env(const char *env_variable)
{
	char **_env;
	char *env_var;
	char *env_data;
	int i, counter;
	int exclude_index = -1;
	int env_len = str_array_len(my_environment);

	for (i = 0; i < env_len;)
	{
		env_data = my_environment[i];
		env_data = str_strip(env_data);
		_env = str_split(env_data, "=", 1);
		env_var = _env[0];

		if (env_var)
		{
			if (str_cmp(env_var, (char *)env_variable))
			{
				exclude_index = i;
				free(_env);
				break;
			}
		}
		free(_env);
		i++;
	}

	/* iterating thru environ but excluding ENV VAR at exclude_index */
	counter = 0;

	if (exclude_index != -1)
	{
		char **new_env;
		
		new_env = malloc(sizeof(char *) * (env_len + 2));
		if (new_env == NULL)
		{
			return (-1);
		}

		/* iterating now */
		for (i = 0; i < env_len;)
		{
			if (i == exclude_index)
			{
				i++;
				continue;
			}
			env_data = my_environment[i];
			new_env[counter] = env_data;
			counter++;
			i++;
		}
		new_env[counter] = NULL;
		str_array_memcpy(my_environment, new_env);
	}
	else
	{
		/* NO ACTION TO PERFORM ,no ENV VAR to unset */
	}
	return (0);
}

/**
 * get_env - gets environment variable value
 * @env_variable: Environment variable
 * Return: Environment variable value or null on error
 */
char *get_env(const char *env_variable)
{
	char **_env;
	char *env_data;
	char *env_var;
	char *env_val;
	int i;
	int env_len = str_array_len(my_environment);

	for (i = 0; i < env_len; i++)
	{
		env_data = my_environment[i];
		_env = str_split(env_data, "=", 2);
		env_var = _env[0];
		env_val = _env[1];

		if (env_var)
		{
			if (str_cmp(env_var, (char *)env_variable))
			{
				return (env_val);
			}
		}
		free(_env);
	}
	return (NULL);
}
