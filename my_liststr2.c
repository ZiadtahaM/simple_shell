#include "shell.h"

/**
 * my_list_len - determines the length of the linked list
 * @my_h: pointer to the first node
 *
 * Return: size of list
 */
size_t my_list_len(const list_t *my_h)
{
    size_t i = 0;

    while (my_h)
    {
	my_h = my_h->next;
	i++;
    }
    return (i);
}

/**
 * my_list_to_strings - returns an array of strings of the list->str
 * @my_head: pointer to the first node
 *
 * Return: array of strings
 */
char **my_list_to_strings(list_t *my_head)
{
    list_t *my_node = my_head;
    size_t i = my_list_len(my_head), j;
    char **my_strs;
    char *my_str;

    if (!my_head || !i)
	return (NULL);
    my_strs = malloc(sizeof(char *) * (i + 1));
    if (!my_strs)
	return (NULL);
    for (i = 0; my_node; my_node = my_node->next, i++)
    {
	my_str = malloc(my_strlen(my_node->str) + 1);
	if (!my_str)
	{
	for (j = 0; j < i; j++)
	free(my_strs[j]);
	free(my_strs);
	return (NULL);
	}

	my_str = my_strcpy(my_str, my_node->str);
	my_strs[i] = my_str;
    }
    my_strs[i] = NULL;
    return (my_strs);
}

/**
 * my_print_list - prints all elements of a list_t linked list
 * @my_h: pointer to the first node
 *
 * Return: size of list
 */
size_t my_print_list(const list_t *my_h)
{
    size_t i = 0;

    while (my_h)
    {
	my_puts(my_convert_number(my_h->num, 10, 0));
	my_putchar(':');
	my_putchar(' ');
	my_puts(my_h->str ? my_h->str : "(nil)");
	my_puts("\n");
	my_h = my_h->next;
	i++;
	}
    return (i);
}

/**
 * my_node_starts_with - returns the node whose string starts with a prefix
 * @my_node: pointer to the list head
 * @my_prefix: string to match
 * @my_c: the next character after the prefix to match
 *
 * Return: matching node or null
 */
list_t *my_node_starts_with(list_t *my_node, char *my_prefix, char my_c)
{
    char *my_p = NULL;

    while (my_node)
    {
	my_p = my_starts_with(my_node->str, my_prefix);
	if (my_p && ((my_c == -1) || (*my_p == my_c)))
	return (my_node);
	my_node = my_node->next;
    }
    return (NULL);
}

/**
 * my_get_node_index - gets the index of a node
 * @my_head: pointer to the list head
 * @my_node: pointer to the node
 *
 * Return: index of the node or -1
 */
ssize_t my_get_node_index(list_t *my_head, list_t *my_node)
{
    size_t i = 0;

    while (my_head)
    {
	if (my_head == my_node)
	return (i);
	my_head = my_head->next;
	i++;
    }
    return (-1);
}

