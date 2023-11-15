#include <unistd.h>

/**
 * _putchar - Implementation of own putchar
 * @c: character to print
 * Return: Always 0 (Success)
 */

int _putchar(int c)
{
	return (write(1, &c, 1));
}
