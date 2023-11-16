#include <stdbool.h>
#include "main.h"

/**
 * str_cmp - compare 2 strings
 * @a: first string
 * @b: second string
 * Return: 0 or 1
 */
int str_cmp(char *a, char *b)
{
	int is_equal = true;
	int i = 0;
	int a_len = str_len(a);
	int b_len = str_len(b);
	char char_a, char_b;

	if (a_len != b_len)
	{
		is_equal = false;
		return is_equal;
	}

	for (; i < a_len; i++)
	{
		char_a = a[i];
		char_b = b[i];
		if (char_a != char_b)
		{
			is_equal = false;
			break;
		}
	}

	return (is_equal);
}
