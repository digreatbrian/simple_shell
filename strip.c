#include <stdlib.h>
#include <stdbool.h>
#include "main.h"

/**
 * str_strip_start - strips whitespaces from start
 * from start of a string including tabs/newlines
 * @string: String to strip
 * Return: stripped string
 */
char *str_strip_start(char *string)
{
	char *new_string;
	int ln = str_len(string);
	int is_start_of_string = true;
	int counter;
	int i;
	char c;

	new_string = malloc(sizeof(char) * (ln + 2));
	counter = 0;

	if (new_string == NULL)
	{
		new_string = string;
		return (new_string);
	}

	/* first strip ,from left to right (ltr) */
	for (i = 0; i < ln; i++)
	{
		c = string[i]; /*character*/

		if (c != ' ' && c != '\n' && c != '\t' && is_start_of_string)
		{
			/**
			 * lets strip the spaces because this is the start of the string
			 */
			new_string[counter] = c;
			counter++;
			is_start_of_string = false;
		}

		else if (!is_start_of_string)
		{
			/**
			 * record as it is , this is not the start of the string ,the spaces
			 * might be needed for string to make sense, it can be spaces in
			 * between a string ,we dont need to remove that
			 */
			new_string[counter] = c;
			counter++;
		}
	}

	new_string[counter] = '\0';
	return (new_string);
}

/**
 * str_strip_end - strips whitespaces from from end
 * from end of a string including tabs, newlines
 * @string: string to strip
 * Return: stripped string
 */
char *str_strip_end(char *string)
{
	char *new_string;
	int ln = str_len(string);
	int end_index = 0;
	int i;
	char c;

	if (ln > 0)
		for (i = ln - 1; i >= 0; i--)
		{
			c = string[i];

			if (c == ' ' || c == '\n' || c == '\t')
			{
				end_index = i - 1;
			}

			else
			{
				/**
				 * the moment we come across a valid char then stop ,
				 * thats where we will end up when building new string
				 */
				end_index = i;
				break;
			}
		}

	if (end_index < 0)
	{
		/* no string was inputted or blank string */
		end_index = 0;
	}

	new_string = malloc(sizeof(char) * (end_index + 2));

	if (new_string == NULL)
	{
		new_string = string;
		return (new_string);
	}
	new_string = str_sub(string, 0, end_index);
	return (new_string);
}

/**
 * str_strip - strips whitespaces/tabs/newlines from a string
 * @string: string to strip
 * Return: stripped string
 */
char *str_strip(char *string)
{
	char *first_strip = str_strip_start(string);
	char *second_strip = str_strip_end(first_strip);

	return (second_strip);
}
