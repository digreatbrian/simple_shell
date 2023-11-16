#include "main.h"

/**
 * char_to_int - converts char to integer
 * @ch: int in form of char
 * Return: int
 */

int char_to_int(char ch)
{
	char c;
	int counter = 0;

	if (ch >= '0' || ch <= '9')
	{
		for (c = '0'; c <= '9'; c++)
		{
			if (c == ch)
			{
				break;
			}
			counter++;
		}
	}
	return (counter);
}

/**
 * str_to_int - converts string to integer
 * @string: int in form of a string
 * Return: int
 */

int str_to_int(char *string)
{
	int new_int = 0;
	int stringlen = str_len(string);
	int i, j;
	int pow;
	char c;

	for (i = 0; i < stringlen; i++)
	{
		c = string[i];
		if (c >= '0' || c <= '9')
		{
			pow = 1;
			for (j = i; j < stringlen - 1; j++)
			{
				pow = pow * 10;
			}
			new_int += (pow) * char_to_int(c);
		}
	}

	return (new_int);
}

