#include <stdlib.h>
#include "main.h"

/**
 * str_lower - converts string to lowercase
 * @string: string to convert
 * Return: converted string
 */
char *str_lower(const char *string)
{
	int _str_len = str_len((char *)string);
	char *newstr;
	char ch;
	int i;

	newstr = malloc(sizeof(char) * _str_len + 1);

	if (newstr == NULL)
	{
		return ((char *)string);
	}

	for (i = 0; i < _str_len; i++)
	{
		ch = string[i];

		if (ch >= 'A' && ch <= 'Z')
		{
			newstr[i] = ch + 32;
		}
		else
		{
			newstr[i] = ch;
		}
	}
	newstr[i] = '\0';
	return ((char *)newstr);
}
