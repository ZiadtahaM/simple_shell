#include "shell.h"

/**
 * my_clear_info - initializes info_t struct
 * @my_info: struct address
 */
void my_clear_info(info_t *my_info)
{
    my_info->my_arg = NULL;
    my_info->my_argv = NULL;
    my_info->my_path = NULL;
    my_info->my_argc = 0;
}

/**
 * my_set_info - initializes info_t struct
 * @my_info: struct address
 * @my_av: argument vector
 */
void my_set_info(info_t *my_info, char **my_av)
{
    int i = 0;

    my_info->my_fname = my_av[0];
    if (my_info->my_arg)
    {
	my_info->my_argv = strtow(my_info->my_arg, " \t");
	if (!my_info->my_argv)
	{
	my_info->my_argv = malloc(sizeof(char *) * 2);
	if (my_info->my_argv)
	{
	my_info->my_argv[0] = my_strdup(my_info->my_arg);
	my_info->my_argv[1] = NULL;
	}
	}
for (i = 0; my_info->my_argv && my_info->my_argv[i]; i++)
	;
my_info->my_argc = i;

my_replace_alias(my_info);
my_replace_vars(my_info);
}
}

/**
 * my_free_info - frees info_t struct fields
 * @my_info: struct address
 * @my_all: true if freeing all fields
 */
void my_free_info(info_t *my_info, int my_all)
{
    ffree(my_info->my_argv);
    my_info->my_argv = NULL;
    my_info->my_path = NULL;
    if (my_all)
    {
if (!my_info->my_cmd_buf)
free(my_info->my_arg);
if (my_info->my_env)
free_list(&(my_info->my_env));
if (my_info->my_history)
free_list(&(my_info->my_history));
if (my_info->my_alias)
free_list(&(my_info->my_alias));
ffree(my_info->my_environ);
my_info->my_environ = NULL;
bfree((void **)my_info->my_cmd_buf);
if (my_info->my_readfd > 2)
close(my_info->my_readfd);
 _putchar(BUF_FLUSH);
}
}

