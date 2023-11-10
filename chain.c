#include "main.h"

/**
 * checksChain - Looks if we should continue chaining according to last status
 * @inf: Struct parameter
 * @buffer: Character buffer
 * @ps: Address of on-going position in the buffer
 * @s: The starting point in buffer
 * @leng: Buffer's length
 *
 * Return: Void
 */

void checksChain(info_t *inf, char *buffer, size_t *ps, size_t s, size_t leng)
{
	size_t a = *ps;

	if (inf->cmdBufferType == CMD_AND)
	{
		if (inf->status)
		{
			buffer[s] = 0;
			a = leng;
		}
	}

	if (inf->cmdBufferType == CMD_OR)
	{
		if (!inf->status)
		{
			buffer[s] = 0;
			a = leng;
		}
	}

	*ps = a;
}

/**
 * isChain - test if current char in buffer is a chain delimeter
 * @inf: Struct parameter
 * @buffer: Character buffer
 * @ps: Address of on-going position in the buffer
 *
 * Return: When chain delimeter 1, otherwise 0
 */

int isChain(info_t *inf, char *buffer, size_t *ps)
{
	size_t j = *ps;

	if (buffer[j] == '|' && buffer[j + 1] == '|')
	{
		buffer[j] = 0;
		j++;
		inf->cmdBufferType = CMD_OR;
	}
	else if (buffer[j] == '&' && buffer[j + 1] == '&')
	{
		buffer[j] = 0;
		j++;
		inf->cmdBufferType = CMD_AND;
	}
	else if (buffer[j] == ';')
	{
		buffer[j] = 0; /* Discards semicolon ':' with null */
		inf->cmdBufferType = CMD_CHAIN;
	}
	else
		return (0);
	*ps = j;
	return (1);
}

