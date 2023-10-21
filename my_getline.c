#include "shell.h"

/**
 * my_input_buf - buffers chained commands
 * @my_info: parameter struct
 * @my_buf: address of buffer
 * @my_len: address of len var
 *
 * Return: bytes read
 */
ssize_t my_input_buf(info_t *my_info, char **my_buf, size_t *my_len)
{
    ssize_t r = 0;
    size_t len_p = 0;

    if (!*my_len) /* if nothing left in the buffer, fill it */
    {
        /*bfree((void **)my_info->my_cmd_buf);*/
        free(*my_buf);
        *my_buf = NULL;
        signal(SIGINT, my_sigintHandler);
#if USE_GETLINE
        r = getline(my_buf, &len_p, stdin);
#else
        r = my_getline(my_info, my_buf, &len_p);
#endif
        if (r > 0)
        {
            if ((*my_buf)[r - 1] == '\n')
            {
                (*my_buf)[r - 1] = '\0'; /* remove trailing newline */
                r--;
            }
            my_info->linecount_flag = 1;
            my_remove_comments(*my_buf);
            my_build_history_list(my_info, *my_buf, my_info->histcount++);
            /* if (_strchr(*my_buf, ';')) is this a command chain? */
            {
                *my_len = r;
                my_info->my_cmd_buf = my_buf;
            }
        }
    }
    return (r);
}

/**
 * my_get_input - gets a line minus the newline
 * @my_info: parameter struct
 *
 * Return: bytes read
 */
ssize_t my_get_input(info_t *my_info)
{
    static char *my_buf; /* the ';' command chain buffer */
    static size_t i, j, len;
    ssize_t r = 0;
    char **my_buf_p = &(my_info->my_arg), *p;

    _putchar(BUF_FLUSH);
    r = my_input_buf(my_info, &my_buf, &len);
    if (r == -1) /* EOF */
        return (-1);
    if (len) /* we have commands left in the chain buffer */
    {
        j = i; /* init new iterator to the current buf position */
        p = my_buf + i; /* get a pointer for return */

        my_check_chain(my_info, my_buf, &j, i, len);
        while (j < len) /* iterate to semicolon or end */
        {
            if (my_is_chain(my_info, my_buf, &j))
                break;
            j++;
        }

        i = j + 1; /* increment past nulled ';'' */
        if (i >= len) /* reached the end of the buffer? */
        {
            i = len = 0; /* reset position and length */
            my_info->cmd_buf_type = CMD_NORM;
        }

        *my_buf_p = p; /* pass back a pointer to the current command position */
        return (my_strlen(p)); /* return the length of the current command */
    }

    *my_buf_p = my_buf; /* else not a chain, pass back the buffer from my_getline() */
    return (r); /* return the length of the buffer from my_getline() */
}

/**
 * my_read_buf - reads a buffer
 * @my_info: parameter struct
 * @my_buf: buffer
 * @my_i: size
 *
 * Return: r
 */
ssize_t my_read_buf(info_t *my_info, char *my_buf, size_t *my_i)
{
    ssize_t r = 0;

    if (*my_i)
        return 0;
    r = read(my_info->readfd, my_buf, READ_BUF_SIZE);
    if (r >= 0)
        *my_i = r;
    return r;
}

/**
 * my_getline - gets the next line of input from STDIN
 * @my_info: parameter struct
 * @my_ptr: address of the pointer to the buffer, preallocated or NULL
 * @my_length: size of the preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int my_getline(info_t *my_info, char **my_ptr, size_t *my_length)
{
    static char buf[READ_BUF_SIZE];
    static size_t i, len;
    size_t k;
    ssize_t r = 0, s = 0;
    char *p = NULL, *new_p = NULL, *c;

    p = *my_ptr;
    if (p && my_length)
        s = *my_length;
    if (i == len)
        i = len = 0;

    r = my_read_buf(my_info, buf, &len);
    if (r == -1 || (r == 0 && len == 0))
        return -1;

    c = my_strchr(buf + i, '\n');
    k = c ? 1 + (unsigned int)(c - buf) : len;
    new_p = my_realloc(p, s, s ? s + k : k + 1);
    if (!new_p) /* MALLOC FAILURE! */
        return (p ? free(p), -1 : -1);

    if (s)
        my_strncat(new_p, buf + i, k - i);
    else
        my_strncpy(new_p, buf + i, k - i + 1);

    s += k - i;
    i = k;
    p = new_p;

    if (my_length)
        *my_length = s;
    *my_ptr = p;
    return s;
}

/**
 * my_sigintHandler - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void my_sigintHandler(__attribute__((unused))int sig_num)
{
    my_puts("\n");
    my_puts("$ ");
    _putchar(BUF_FLUSH);
}

