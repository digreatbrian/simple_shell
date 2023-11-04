#include <stdlib.h>
#include "main.h"

/**
 * str_sub - build substring from a string
 * @string_array: array of chars | string
 * @start_index: index to start from
 * @end_index: index to stop at
 * Return: array of splitted strings | NULL
 */
char *str_sub(const char *string_array, int start_index, int end_index)
{
	int ln, i;
	int _str_len = str_len((char *)string_array);
	char *st;

	ln = end_index - start_index;
	st = malloc(sizeof(char) * (ln + 2)); /* including null byte */

	if (st == NULL)
	{
		return (NULL);
	}

	if ((_str_len < ln) || (end_index > _str_len - 1) || (ln < 1))
	{
		if (ln == 0)
		{
			/* one char is requested */
			st[0] = string_array[start_index];
			st[1] = '\0';
			return (st);
		}
		return (NULL);
	}

	else if ((start_index > _str_len - 1))
	{
		return (NULL);
	}

	for (i = 0; start_index <= end_index; (start_index++, i++))
	{
		st[i] = string_array[start_index];
	}

	st[i] = '\0'; /* ending the string */

	return (st);
}
