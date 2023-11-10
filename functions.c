#include "main.h"

/**
 * rmComments - Discards first instance of '#' with '\0'
 * @buffer: String to be altered
 *
 * Return: Success 0
 */

void rmComments(char *buffer)
{
	int j;

	for (j = 0; buffer[j] != '\0'; j++)
		if (buffer[j] == '#' && (!j || buffer[j - 1] == ' '))
		{
			buffer[j] = '\0';
			break;
		}
}

/**
 * buildHistoryList - Function adds entry to a history linked list
 * @inf: Struct parameter
 * @buffer: Buffer
 * @countLine: History's linecount, histcount
 *
 * Return: Success 0
 */

int buildHistoryList(info_t *inf, char *buffer, int countLine)
{
	list_t *node = NULL;

	if (inf->history)
		node = inf->history;
	addNodeEnd(&node, buffer, countLine);

	if (!inf->history)
		inf->history = node;
	return (0);
}

/**
 * addNodeEnd - Adds node at the end of list
 * @h: Pointer to head node
 * @st: Field of node
 * @nf: Index node used by history
 *
 * Return: List size
 */

list_t *addNodeEnd(list_t **h, const char *st, int nf)
{
	list_t *curr_node, *node;

	if (!h)
		return (NULL);

	node = *h;
	curr_node = malloc(sizeof(list_t));

	if (!curr_node)
		return (NULL);
	memorySet((void *)curr_node, 0, sizeof(list_t));
	curr_node->nf = nf;

	if (st)
	{
		curr_node->st = strDuplicate(st);

		if (!curr_node->st)
		{
			free(curr_node);
			return (NULL);
		}
	}

	if (node)
	{
		while (node->next)
			node = node->next;
		node->next = curr_node;
	}
	else
		*h = curr_node;
	return (curr_node);
}

/**
 * _puts - Input string
 * @str: String to print
 *
 * Return: Void
 */

void _puts(char *str)
{
	int a = 0;

	if (!str)
		return;
	while (str[a] != '\0')
	{
		_putchar(str[a]);
		a++;
	}
}

