#include "shell.h"

/**
 * my_is_chain - test if the current character in the buffer is a chain delimiter
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of the current position in buf
 *
 * Return: 1 if a chain delimiter, 0 otherwise
 */
int my_is_chain(info_t *info, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		info->my_cmd_buf_type = CMD_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		info->my_cmd_buf_type = CMD_AND;
	}
	else if (buf[j] == ';') /* found the end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		info->my_cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * my_check_chain - checks whether we should continue chaining based on the last status
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of the current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void my_check_chain(info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (info->my_cmd_buf_type == CMD_AND)
	{
		if (info->my_status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (info->my_cmd_buf_type == CMD_OR)
	{
		if (!info->my_status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * my_replace_alias - replaces an alias in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int my_replace_alias(info_t *info)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(info->my_alias, info->my_argv[0], '=');
		if (!node)
			return (0);
		free(info->my_argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		info->my_argv[0] = p;
	}
	return (1);
}

/**
 * my_replace_vars - replaces variables in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int my_replace_vars(info_t *info)
{
	int i = 0;
	list_t *node;

	for (i = 0; info->my_argv[i]; i++)
	{
		if (info->my_argv[i][0] != '$' || !info->my_argv[i][1])
			continue;

		if (!my_strcmp(info->my_argv[i], "$?"))
		{
			my_replace_string(&(info->my_argv[i]),
				_strdup(convert_number(info->my_status, 10, 0)));
			continue;
		}
		if (!my_strcmp(info->my_argv[i], "$$"))
		{
			my_replace_string(&(info->my_argv[i]),
				_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(info->my_env, &info->my_argv[i][1], '=');
		if (node)
		{
			my_replace_string(&(info->my_argv[i]),
				_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		my_replace_string(&info->my_argv[i], _strdup(""));
	}
	return (0);
}

/**
 * my_replace_string - replaces a string
 * @old: address of the old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int my_replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
