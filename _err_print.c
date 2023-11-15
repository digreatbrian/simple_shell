#include "main.h"

/**
 * _err_print - Prints to STDERR
 * @string: string to print
 * Return: Always 0 (Success)
 */

void _err_print(char *string)
{
	int counter = 0;
	char ch;

	if (string)
	{
		ch = string[counter];

		while (ch != '\0')
		{
			_err_putchar(ch);
			counter++;
			ch = string[counter];
		}
	}
}
