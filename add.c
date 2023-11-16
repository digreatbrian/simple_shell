#include <stdlib.h>
#include "main.h"

/**
 * str_add - add two strings
 * @a: first string
 * @b: second bstring
 * Return: Concatenated string
 */

char *str_add(char *a, char *b)
{
	int a_len = str_len(a);
	int b_len = str_len(b);
	int counter = 0;
	int i = 0, j = 0;
	int t = (a_len + b_len) * sizeof(char);
	char char_a, char_b;
	char *newstr = malloc(t + sizeof(char));
	/* adding a to newstr first */
	char_a = a[i];
	while (char_a != '\0')
	{
		newstr[counter] = char_a;
		counter++;
		i++;
		char_a = a[i];
	}

	/* adding b to newstr */
	char_b = b[j];
	while (char_b != '\0')
	{
		newstr[counter] = char_b;
		counter++;
		j++;
		char_b = b[j];
	}

	/* closing the string */
	newstr[counter] = '\0';
	return (newstr);
}

