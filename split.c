#include <stdlib.h>
#include <stdbool.h>
#include "main.h"

sep_index *get_sep_indexes(char *string, char *sep, int max_indexes);
int count_sep_indexes(sep_index *sep_indexes);

/**
 * str_split - splits a string
 * @string: target string
 * @sep: separator
 * @max_splits: max number of splits
 * Return: array of strings | NULL
 */

char **str_split(char *string, char *sep, int max_splits)
{
	int i, string_len;
	int index_count;
	char **splitted;
	char *temp_str;
	sep_index *sep_indexes = get_sep_indexes(string, sep, max_splits);

	if (sep_indexes == NULL)
	{
		/* no single sep was discovered */
		splitted = (char **)malloc((sizeof(char *) + sizeof(NULL)));

		if (splitted == NULL)
		{
			return (NULL);
		}
		splitted[0] = string;
		splitted[1] = NULL;
		return (splitted);
	}

	string_len = str_len(string);
	index_count = count_sep_indexes(sep_indexes);
	splitted = malloc((sizeof(sep_index) * (index_count + 1)) + sizeof(char));

	for (i = 0; i <= index_count;)
	{
		sep_index target_index;
		int start, end;

		target_index = sep_indexes[i];

		if (target_index.start == -1 && target_index.end == -1)
		{
			/* we've reached end of indexes */
			start = sep_indexes[i - 1].end + 1;
			end = string_len - 1;

			if (start > end)
			{
				start--;
			}
		}
		else
		{
			if (target_index.start >= 1)
			{
				end = target_index.start - 1;
				if (i > 0)
				{
					start = sep_indexes[i - 1].end + 1;
				}
				else
				{
					start = 0;
				}

				if (start > end)
				{
					start--;
				}
			}
			else
			{
				end = target_index.start;
				start = target_index.start;
			}
		}

		if (end == start)
		{
			if (string[start] == sep[0])
			{
				temp_str = "";
			}
			else
			{
				temp_str = str_sub(string, start, end);
			}
		}
		else
		{
			temp_str = str_sub(string, start, end);
		}

		splitted[i] = temp_str;
		i++;
	}
	free(sep_indexes);
	splitted[i] = NULL;
	return (splitted);
}

/**
 * get_sep_indexes - gets indexes where a separator was found
 * @string: target string
 * @sep: separator
 * @max_indexes: max number of indexes
 * Return: array of separator indexes | NULL
 */

sep_index *get_sep_indexes(char *string, char *sep, int max_indexes)
{
	int sep_len = str_len(sep);
	int string_len = str_len(string);
	int counter = 0;
	int index_counter = 0;
	int i = 0;
	int sep_found = true;
	char cha;
	sep_index *indexes = malloc((sizeof(sep_index) * max_indexes) +
			sizeof(NULL));
	/* index to show we have finished adding indexes */
	sep_index closing_index;

	if (indexes == NULL || max_indexes < 1)
	{
		return (NULL);
	}

	for (; counter < string_len ; )
	{
		/* iterating every char in string */
		cha = string[counter];
		i = 0;

		if (index_counter == max_indexes)
		{
			break;
		}

		if (cha == sep[0])
		{
			/* checking if sep is found */
			sep_found = true;

			for (; i < sep_len ; i++)
			{
				if (sep[i] != string[counter + i])
				{
					sep_found = false;
				}
			}

			/* If sep is found */
			if (sep_found)
			{
				sep_index new_index;

				new_index.start = counter;
				new_index.end = counter + sep_len - 1;
				indexes[index_counter] = new_index;
				index_counter++;
				counter = counter + sep_len - 1;
				/* lets skip some chars */
			}
		}
		counter++;
	}

	if (index_counter == 0)
	{
		/* no sep was found in string */
		return (NULL);
	}

	closing_index.start = -1;
	closing_index.end = -1;
	indexes[index_counter] = closing_index;
	return (indexes);
}

/**
 * count_sep_indexes - count indexes sep_index array
 * @sep_indexes: array of sep_indexes
 * Return: number of sep indexes found
 */

int count_sep_indexes(sep_index *sep_indexes)
{
	int counter = 0;
	sep_index index;

	while (true)
	{
		index = sep_indexes[counter];
		if (index.start == -1 && index.end == -1)
		{
			/* we've reached end of indexes */
			break;
		}
		counter++;
	}
	return (counter);
}

