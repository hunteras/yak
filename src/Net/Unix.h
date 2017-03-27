#ifndef UNIX_H
#define UNIX_H

#include "Error.h"

#include <unistd.h>
#include <stdio.h>

namespace yak
{
    pid_t
    Fork(void)
    {
	pid_t pid;

	if ( (pid = fork()) == -1)
            Error::err_sys("fork error");
	return(pid);
    }

    void
    Fclose(FILE *fp)
    {
	if (fclose(fp) != 0)
            Error::err_sys("fclose error");
    }

    FILE *
    Fdopen(int fd, const char *type)
    {
	FILE	*fp;

	if ( (fp = fdopen(fd, type)) == NULL)
            Error::err_sys("fdopen error");

	return(fp);
    }

    char *
    Fgets(char *ptr, int n, FILE *stream)
    {
	char	*rptr;

	if ( (rptr = fgets(ptr, n, stream)) == NULL && ferror(stream))
            Error::err_sys("fgets error");

	return (rptr);
    }

    FILE *
    Fopen(const char *filename, const char *mode)
    {
	FILE	*fp;

	if ( (fp = fopen(filename, mode)) == NULL)
            Error::err_sys("fopen error");

	return(fp);
    }

    void
    Fputs(const char *ptr, FILE *stream)
    {
	if (fputs(ptr, stream) == EOF)
            Error::err_sys("fputs error");
    }

}
#endif
