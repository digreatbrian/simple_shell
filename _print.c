#include "main.h"

/**
 * _print - Implementation of own printf without formatting function
 * @string: string to print
 * Return: Always 0 (Success)
 */
void _print(char *string)
{
	int counter = 0;
	char ch;

	if (string)
	{
		ch = string[counter];

		while (ch != '\0')
		{
			_putchar(ch);
			counter++;
			ch = string[counter];
		}
	}
}
