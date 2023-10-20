#include "shell.h"

/**
 * my_strcpy - copies a string
 * @my_dest: the destination
 * @my_src: the source
 *
 * Return: pointer to destination
 */
char *my_strcpy(char *my_dest, char *my_src)
{
	int i = 0;

	if (my_dest == my_src || my_src == 0)
	return (my_dest);
	while (my_src[i])
	{
	my_dest[i] = my_src[i];
	i++;
	}
	my_dest[i] = 0;
	return (my_dest);
}

/**
 * my_strdup - duplicates a string
 * @my_str: the string to duplicate
 *
 * Return: pointer to the duplicated string
 */
char *my_strdup(const char *my_str)
{
	int length = 0;
	char *my_ret;

	if (my_str == NULL)
	return (NULL);
	while (*my_str++)
	length++;
	my_ret = malloc(sizeof(char) * (length + 1));
	if (!my_ret)
	return (NULL);
	for (length++; length--;)
	my_ret[length] = *--my_str;
	return (my_ret);
}

/**
 * my_puts - prints an input string
 * @my_str: the string to be printed
 *
 * Return: Nothing
 */
void my_puts(char *my_str)
{
	int i = 0;

	if (!my_str)
	return;
	while (my_str[i] != '\0')
	{
	my_putchar(my_str[i]);
	i++;
	}
}

/**
 * my_putchar - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int my_putchar(char c)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
	write(1, buf, i);
	i = 0;
	}
	if (c != BUF_FLUSH)
	buf[i++] = c;
	return (1);
}

