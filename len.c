#include <stdio.h>
#include <stdlib.h>
#include "main.h"

/**
 * str_len - get length of a string
 * @string: array of chars | string
 * Return: length of the string
 */

int str_len(char *string)
{
	int counter = 0;
	char ch;

	ch = string[counter];

	while (ch != '\0')
	{
		counter++;
		ch = string[counter];
	}
	return (counter);
}

/**
 * str_array_len - counts number of strings in an array
 * @array: array containing strings
 * Return: number of items in the array
 */

int str_array_len(char *array[])
{
	int counter = 0;
	char *string;

	string = array[counter];

	while (string)
	{
		counter++;
		string = array[counter];
	}
	return (counter);
}

