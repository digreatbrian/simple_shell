#include "main.h"

/**
 **strnCopy - Function to copy a string
 *@dstn: Destination string to be copied to
 *@srce: Source string
 *@num: Number of characters to copy
 *
 *Return: String after concatinating
 */

char *strnCopy(char *dstn, char *srce, int num)
{
	int a, b;
	char *d = dstn;

	a = 0;

	while (srce[a] != '\0' && a < num - 1)
	{
		dstn[a] = srce[a];
		a++;
	}

	if (a < num)
	{
		b = a;
		while (b < num)
		{
			dstn[b] = '\0';
			b++;
		}
	}
	return (d);
}

/**
 **strnConcat - Function concatenates two strings
 *@dstn: First string
 *@srce: Second string
 *@num: Bytes to exactly use
 *
 *Return: Concatenated string
 */

char *strnConcat(char *dstn, char *srce, int num)
{
	int a, b;
	char *c = dstn;

	a = 0;
	b = 0;

	while (dstn[a] != '\0')
		a++;
	while (srce[b] != '\0' && b < num)
	{
		dstn[a] = srce[b];
		a++;
		b++;
	}

	if (b < num)
		dstn[a] = '\0';
	return (c);
}

/**
 **strChar - Looks for a character in a string
 *@str: String to parse
 *@chr: Character to search for
 *
 *Return: Pointer (str) to the memory area str
 */

char *strChar(char *str, char chr)
{
	do {
		if (*str == chr)
			return (str);
	} while (*str++ != '\0');

	return (NULL);
}

/**
 * strDuplicate - Duplicates  strings
 * @strn: String to be duplicated
 *
 * Return: A pointer to duplicated string
 */

char *strDuplicate(const char *strn)
{
	int len = 0;
	char *rt;

	if (strn == NULL)
		return (NULL);

	while (*strn++)
		len++;
	rt = malloc(sizeof(char) * (len + 1));

	if (!rt)
		return (NULL);

	for (len++; len--;)
		rt[len] = *--strn;
	return (rt);
}

