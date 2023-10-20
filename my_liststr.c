#include "shell.h"

/**
 * my_add_node - adds a node to the start of the list
 * @my_head: address of pointer to head node
 * @my_str: str field of node
 * @my_num: node index used by history
 *
 * Return: size of list
 */
list_t *my_add_node(list_t **my_head, const char *my_str, int my_num)
{
    list_t *my_new_head;

    if (!my_head)
        return (NULL);
    my_new_head = malloc(sizeof(list_t));
    if (!my_new_head)
        return (NULL);
    my_memset((void *)my_new_head, 0, sizeof(list_t));
    my_new_head->num = my_num;
    if (my_str)
    {
        my_new_head->str = my_strdup(my_str);
        if (!my_new_head->str)
        {
            free(my_new_head);
            return (NULL);
        }
    }
    my_new_head->next = *my_head;
    *my_head = my_new_head;
    return (my_new_head);
}

/**
 * my_add_node_end - adds a node to the end of the list
 * @my_head: address of pointer to head node
 * @my_str: str field of node
 * @my_num: node index used by history
 *
 * Return: size of list
 */
list_t *my_add_node_end(list_t **my_head, const char *my_str, int my_num)
{
    list_t *my_new_node, *my_node;

    if (!my_head)
        return (NULL);

    my_node = *my_head;
    my_new_node = malloc(sizeof(list_t));
    if (!my_new_node)
        return (NULL);
    my_memset((void *)my_new_node, 0, sizeof(list_t));
    my_new_node->num = my_num;
    if (my_str)
    {
        my_new_node->str = my_strdup(my_str);
        if (!my_new_node->str)
        {
            free(my_new_node);
            return (NULL);
        }
    }
    if (my_node)
    {
        while (my_node->next)
            my_node = my_node->next;
        my_node->next = my_new_node;
    }
    else
        *my_head = my_new_node;
    return (my_new_node);
}

/**
 * my_print_list_str - prints only the str element of a list_t linked list
 * @my_h: pointer to the first node
 *
 * Return: size of list
 */
size_t my_print_list_str(const list_t *my_h)
{
    size_t i = 0;

    while (my_h)
    {
        my_puts(my_h->str ? my_h->str : "(nil)");
        my_puts("\n");
        my_h = my_h->next;
        i++;
    }
    return (i);
}

/**
 * my_delete_node_at_index - deletes node at the given index
 * @my_head: address of pointer to the first node
 * @my_index: index of the node to delete
 *
 * Return: 1 on success, 0 on failure
 */
int my_delete_node_at_index(list_t **my_head, unsigned int my_index)
{
    list_t *my_node, *my_prev_node;
    unsigned int i = 0;

    if (!my_head || !*my_head)
        return (0);

    if (!my_index)
    {
        my_node = *my_head;
        *my_head = (*my_head)->next;
        free(my_node->str);
        free(my_node);
        return (1);
    }
    my_node = *my_head;
    while (my_node)
    {
        if (i == my_index)
        {
            my_prev_node->next = my_node->next;
            free(my_node->str);
            free(my_node);
            return (1);
        }
        i++;
        my_prev_node = my_node;
        my_node = my_node->next;
    }
    return (0);
}

/**
 * my_free_list - frees all nodes of a list
 * @my_head_ptr: address of pointer to the head node
 *
 * Return: void
 */
void my_free_list(list_t **my_head_ptr)
{
    list_t *my_node, *my_next_node, *my_head;

    if (!my_head_ptr || !*my_head_ptr)
        return;
    my_head = *my_head_ptr;
    my_node = my_head;
    while (my_node)
    {
        my_next_node = my_node->next;
        free(my_node->str);
        free(my_node);
        my_node = my_next_node;
    }
    *my_head_ptr = NULL;
}

