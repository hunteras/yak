#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <errno.h>

#include "Error.h"

namespace yak
{
    const char *byte_order()
    {
        union {
            short s;
            char c[sizeof(short)];
        } un;

	un.s = 0x0102;

	if (sizeof(short) == 2) {
            if (un.c[0] == 1 && un.c[1] == 2)
                return "big-endian";
            else if (un.c[0] == 2 && un.c[1] == 1)
                return "little-endian";
            else
                return "unknown";
	} else
            return "error short size";
    }

    // Read n bytes from a descriptor.
    static
    ssize_t						
    readn(int fd, void *vptr, size_t n)
    {
	size_t nleft;
	ssize_t	nread;
	char *ptr;

	ptr = (char *)vptr;
	nleft = n;
	while (nleft > 0) {
            if ( (nread = read(fd, ptr, nleft)) < 0) {
                if (errno == EINTR)
                    nread = 0;		/* and call read() again */
                else
                    return(-1);
            } else if (nread == 0)
                break;				/* EOF */

            nleft -= nread;
            ptr += nread;
	}
	return(n - nleft);		/* return >= 0 */
    }

    // Write n bytes to a descriptor.
    static
    ssize_t
    writen(int fd, const void *vptr, size_t n)
    {
	size_t nleft;
	ssize_t	nwritten;
	const char *ptr;

	ptr = (const char*)vptr;
	nleft = n;
	while (nleft > 0) {
            if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
                if (nwritten < 0 && errno == EINTR)
                    nwritten = 0;		/* and call write() again */
                else
                    return(-1);			/* error */
            }

            nleft -= nwritten;
            ptr += nwritten;
	}
	return(n);
    }

    static
    void
    Close(int fd)
    {
	if (close(fd) == -1)
            Error::err_sys("close error");
    }

    static
    void
    Write(int fd, void *ptr, size_t nbytes)
    {
	if (write(fd, ptr, nbytes) != nbytes)
            Error::err_sys("write error");
    }

    static
    ssize_t
    Read(int fd, void *ptr, size_t nbytes)
    {
	ssize_t		n;

	if ( (n = read(fd, ptr, nbytes)) == -1)
            Error::err_sys("read error");
	return(n);
    }

    
}
#endif
