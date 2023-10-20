#include "shell.h"

/**
 * my_is_cmd - determines if a file is an executable command
 * @my_info: the info struct
 * @my_path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int my_is_cmd(info_t *my_info, char *my_path)
{
    struct stat st;

    (void)my_info;
    if (!my_path || stat(my_path, &st))
        return (0);

    if (st.st_mode & S_IFREG)
    {
        return (1);
    }
    return (0);
}

/**
 * my_dup_chars - duplicates characters
 * @my_pathstr: the PATH string
 * @my_start: starting index
 * @my_stop: stopping index
 *
 * Return: pointer to a new buffer
 */
char *my_dup_chars(char *my_pathstr, int my_start, int my_stop)
{
    static char buf[1024];
    int i = 0, k = 0;

    for (k = 0, i = my_start; i < my_stop; i++)
        if (my_pathstr[i] != ':')
            buf[k++] = my_pathstr[i];
    buf[k] = 0;
    return (buf);
}

/**
 * my_find_path - finds this cmd in the PATH string
 * @my_info: the info struct
 * @my_pathstr: the PATH string
 * @my_cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *my_find_path(info_t *my_info, char *my_pathstr, char *my_cmd)
{
    int i = 0, curr_pos = 0;
    char *my_path;

    if (!my_pathstr)
        return (NULL);
    if ((my_strlen(my_cmd) > 2) && my_starts_with(my_cmd, "./"))
    {
        if (my_is_cmd(my_info, my_cmd))
            return (my_cmd);
    }
    while (1)
    {
        if (!my_pathstr[i] || my_pathstr[i] == ':')
        {
            my_path = my_dup_chars(my_pathstr, curr_pos, i);
            if (!*my_path)
                my_strcat(my_path, my_cmd);
            else
            {
                my_strcat(my_path, "/");
                my_strcat(my_path, my_cmd);
            }
            if (my_is_cmd(my_info, my_path))
                return (my_path);
            if (!my_pathstr[i])
                break;
            curr_pos = i;
        }
        i++;
    }
    return (NULL);
}

