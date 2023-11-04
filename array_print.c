#include "main.h"

/**
 * str_array_print - print strings in an array
 * @arr: array with strings
 * @sep: Separator to use when printing
 */
void str_array_print(char **arr, char *sep)
{
	int a;

	for (a = 0; a < str_array_len(arr); a++)
	{
		_print(arr[a]);
		if (sep)
		{
			_print(sep);
		}
	}
}
