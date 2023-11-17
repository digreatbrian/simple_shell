#include <stdlib.h>
#include "main.h"

/**
	* Frees a string array
	* @arr: array to free
	*/
void free_str_array(char **arr)
{
	int i;
	
	for (i = 0; i < str_array_len(arr) ;  i++)
	{
	   if (arr[1])
	   {
	   	if (!str_cmp(str_strip(arr[1]), ""))
	   	{
	   		free(arr[1]);
	   	}
	   }
    }
    if (str_array_len(arr) > 0)
    {
    	free(arr);
    }
}
