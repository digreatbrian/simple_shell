#include "main.h"

/**
 * bufferInput - Buffers chained commands
 * @inf: Struct parameter
 * @buffer: Buffer's address
 * @leng: Length variable's address
 *
 * Return: Amount of bytes read
 */

ssize_t bufferInput(info_t *inf, char **buffer, size_t *leng)
{
	ssize_t a = 0;
	size_t leng_p = 0;

	if (!*leng) /* When there's nothing left in the buffer, fill it */
	{
		/*bfree((void **)inf->cmd_buf);*/
		free(*buffer);
		*buffer = NULL;
		signal(SIGINT, hintIntHandler);
#if USE_GETLINE
		a = getline(buffer, &leng_p, stdin);
#else
		a = getsLine(inf, buffer, &leng_p);
#endif
		if (a > 0)
		{
			if ((*buffer)[a - 1] == '\n')
			{
				(*buffer)[a - 1] = '\0'; /* Removes all trailing newline */
				a--;
			}
			inf->linecount_flag = 1;
			rmComments(*buffer);
			buildHistoryList(inf, *buffer, inf->histcount++);
			/* if (strChar(*buffer, ';')) is this a command chain? */
			{
				*leng = a;
				inf->cmdBuffer = buffer;
			}
		}
	}
	return (a);
}

/**
 * getsInput - This function gets a line minus the newline
 * @inf: Struct parameter
 *
 * Return: Amount of bytes read
 */

ssize_t getsInput(info_t *inf)
{
	static char *buffer; /* ';' command chain buffer */
	static size_t b, c, leng;
	ssize_t a = 0;
	char **buff_p = &(inf->arg), *p;

	_putchar(BUFF_FLUSH);
	a = bufferInput(inf, &buffer, &leng);

	if (a == -1) /* EOF */
		return (-1);

	if (leng) /* If there are commands left in the chain buffer */
	{
		c = b; /* init new iterator to on-going buffer's position */
		p = buffer + b; /* get pointer for return */

		checksChain(inf, buffer, &c, b, leng);
		while (c < leng) /* iterate to semicolon or end */
		{
			if (isChain(inf, buffer, &c))
				break;
			c++;
		}

		b = c + 1; /* increment past nulled ';'' */
		if (b >= leng) /* reached end of buffer? */
		{
			b = leng = 0; /* reset position and length */
			inf->cmdBufferType = CMD_NORM;
		}

		*buff_p = p; /* pass back pointer to current command position */
		return (str_len(p)); /* return length of current command */
	}

	*buff_p = buffer; /* else not a chain, pass back buffer from getLine() */
	return (a); /* return length of buffer from getLine() */
}

/**
 * readsBuffer - Function reads a buffer
 * @inf: Struct parameter
 * @buffer: Buffer
 * @s: Size
 *
 * Return: a
 */

ssize_t readsBuffer(info_t *inf, char *buffer, size_t *s)
{
	ssize_t a = 0;

	if (*s)
		return (0);
	a = read(inf->readfd, buffer, READ_BUFF_SIZE);

	if (a >= 0)
		*s = a;
	return (a);
}

/**
 * getsLine - Next line of input from STDIN
 * @inf: Struct parameter
 * @ptr: Pointer to buffer's address
 * @len: ptr buffer's size if not NULL
 *
 * Return: a
 */

int getsLine(info_t *inf, char **ptr, size_t *len)
{
	static char buffer[READ_BUFF_SIZE];
	static size_t b, leng;
	size_t c;
	ssize_t i = 0, a = 0;
	char *p = NULL, *curr_p = NULL, *d;

	p = *ptr;

	if (p && len)
		a = *len;

	if (b == leng)
		b = leng = 0;

	i = readsBuffer(inf, buffer, &leng);

	if (i == -1 || (i == 0 && leng == 0))
		return (-1);

	d = strChar(buffer + i, '\n');
	c = d ? 1 + (unsigned int)(d - buffer) : leng;
	curr_p = alloc(p, a, a ? a + c : c + 1);

	if (!curr_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (a)
		strnConcat(curr_p, buffer + i, c - i);
	else
		strnCopy(curr_p, buffer + i, c - i + 1);

	a += c - i;
	i = c;
	p = curr_p;

	if (len)
		*len = a;
	*ptr = p;
	return (a);
}

/**
 * hintIntHandler - Function stops ctrl-C from working
 * @hint_num: Signaled number
 *
 * Return: void
 */

void hintIntHandler(__attribute__((unused))int hint_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUFF_FLUSH);
}

