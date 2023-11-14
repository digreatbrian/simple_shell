#include <unistd.h>

/**
 * _err_putchar - putchar to STDERR
 * @c: character to print
 * Return: Always 0 (Success)
 */
int _err_putchar(int c)
{
	return (write(STDERR_FILENO, &c, 1));
}
