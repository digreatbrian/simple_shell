#include "main.h"

/**
 * bfree - Function frees a pointer while it NULLs the address
 * @ptr: Pointer to free
 *
 * Return: Free 1, 0 otherwise
 */

int bfree(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}

/**
 * alloc - Block of memory to be reallocated
 * @ptr: Pointer to previous block
 * @prev_size: Previous block's byte size
 * @curr_size: Current block's byte size
 *
 * Return: Pointer to the previous block
 */

void *alloc(void *ptr, unsigned int prev_size, unsigned int curr_size)
{
	char *pn;

	if (!ptr)
		return (malloc(curr_size));

	if (!curr_size)
		return (free(ptr), NULL);

	if (curr_size == prev_size)
		return (ptr);

	pn = malloc(curr_size);

	if (!pn)
		return (NULL);

	prev_size = prev_size < curr_size ? prev_size : curr_size;

	while (prev_size--)
		pn[prev_size] = ((char *)ptr)[prev_size];
	free(ptr);
	return (pn);
}

/**
 * memorySet - Memory with constant byte
 * @p: Pointer to memory area
 * @bt: Bytes to fill *p with
 * @nb: Bytes to be filled
 *
 * Return: Pointer(p) to the memory area p
 */

char *memorySet(char *p, char bt, unsigned int nb)
{
	unsigned int j;

	for (j = 0; j < nb; j++)
		p[j] = bt;
	return (p);
}

