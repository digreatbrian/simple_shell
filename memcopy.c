#include <stdlib.h>
#include "main.h"

/**
 * str_array_memcpy - copies array to another array
 * @_dest: destination mem
 * @_src: source mem
 * Return: 0 or -1 on error
 */
int str_array_memcpy(char *_dest[], char *_src[])
{
    char **dest = (char **)_dest;
    char **src = (char **)_src;
    char *src_str;
    int i = 0;

    while (1)
    {
        src_str = src[i];
        if (src_str)
        {
            dest[i] = src[i];
            i++;
        }
        else
        {
            break;
        }
        
    }
    dest[i] = NULL;
    return (0);
}
