#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#define BUFF 3

char *strjoin(char *left, char *right)
{
    char *res = NULL;
    size_t len = 0;
    if (!left && !right)
        return NULL;
    if (left)
        len = strlen(left);
    if (right)
        len += strlen(right);
    res = calloc(len + 1, sizeof(char));
    left &&strcpy(res, left);
    right &&strcpy(res + strlen(res), right);
    return res;
}

char *readline(int fd)
{
    static char *backup;
    char *res = NULL;
    char buff[BUFF + 1];
    memset(buff, 0, BUFF + 1);

    while (backup == NULL || !strchr(backup, '\n'))
    {
        int rd = read(fd, buff, BUFF);
        if (rd <= 0)
            break;
        char *tmp = strjoin(backup, buff);
        free(backup);
        backup = tmp;
        memset(buff, 0, BUFF + 1);
    }
    if (backup)
    {
        char *tmp = strchr(backup, '\n');
        if (tmp) // has \n
        {
            char *tmp1 = strdup(tmp + 1);
            size_t len = tmp - backup + 1;
            res = calloc(len + 1, sizeof(char));
            strncpy(res, backup, len);
            free(backup);
            backup = tmp1;
        }
        else
        {
            res = backup;
            backup = NULL;
        }
    }
    return res;
}

int main()
{
    int fd = open("file.txt", O_RDWR);

    char *str = readline(fd);
    while (str)
    {
        printf(">%s", str);
        str = readline(fd);
    }
}