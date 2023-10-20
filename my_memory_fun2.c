#include "shell.h"

/**
 * my_bfree - frees a pointer and NULLs the address
 * @my_ptr: address of the pointer to free
 *
 * Return: 1 if freed, otherwise 0.
 */
int my_bfree(void **my_ptr)
{
    if (my_ptr && *my_ptr)
    {
        free(*my_ptr);
        *my_ptr = NULL;
        return (1);
    }
    return (0);
}
