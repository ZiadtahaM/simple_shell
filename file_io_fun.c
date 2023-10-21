#include "shell.h"

/**
 * my_get_history_file - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containg history file
 */
char *my_get_history_file(info_t *info)
{
	char *my_buf, *my_dir;

	my_dir = my_getenv(info, "HOME=");
	if (!my_dir)
		return (NULL);
	my_buf = my_malloc(sizeof(char) * (my_strlen(my_dir) + my_strlen(HIST_FILE) + 2));
	if (!my_buf)
		return (NULL);
	my_buf[0] = 0;
	my_strcpy(my_buf, my_dir);
	my_strcat(my_buf, "/");
	my_strcat(my_buf, HIST_FILE);
	return (my_buf);
}

/**
 * my_write_history - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int my_write_history(info_t *info)
{
	ssize_t fd;
	char *filename = my_get_history_file(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = my_open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	my_free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
	{
		my_putsfd(node->str, fd);
		my_putfd('\n', fd);
	}
	my_putfd(BUF_FLUSH, fd);
	my_close(fd);
	return (1);
}

/**
 * my_read_history - reads history from file
 * @info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int my_read_history(info_t *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = my_get_history_file(info);

	if (!filename)
		return (0);

	fd = my_open(filename, O_RDONLY);
	my_free(filename);
	if (fd == -1)
		return (0);
	if (!my_fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = my_malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = my_read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (my_free(buf), 0);
	my_close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			my_build_history_list(info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		my_build_history_list(info, buf + last, linecount++);
	my_free(buf);
	info->histcount = linecount;
	while (info->histcount-- >= HIST_MAX)
		my_delete_node_at_index(&(info->history), 0);
	my_renumber_history(info);
	return (info->histcount);
}

/**
 * my_build_history_list - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int my_build_history_list(info_t *info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;
	my_add_node_end(&node, buf, linecount);

	if (!info->history)
		info->history = node;
	return (0);
}

/**
 * my_renumber_history - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int my_renumber_history(info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->histcount = i);
}

