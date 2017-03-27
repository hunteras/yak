#ifndef COMMON_H
#define COMMON_H

#define	LISTENQ	1024	/* 2nd argument to listen() */
/* Miscellaneous constants */
#define	MAXLINE	4096	/* max text line length */
#define	BUFFSIZE 8192	/* buffer size for reads and writes */

#define	SERV_PORT 9877			/* TCP and UDP */
#define	SERV_PORT_STR "9877"			/* TCP and UDP */

#include "Error.h"
#include "Utils.h"
#include "Unix.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <unistd.h>

using namespace yak;

static
void
str_echo(int sockfd)
{
    ssize_t n;
    char buf[MAXLINE];

  again:
    while ( (n = read(sockfd, buf, MAXLINE)) > 0)
        Writen(sockfd, buf, n);

    if (n < 0 && errno == EINTR)
        goto again;
    else if (n < 0)
        Error::err_sys("str_echo: read error");

}

static
void
str_cli(FILE *fp, int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    while (Fgets(sendline, MAXLINE, fp) != NULL) {
        Writen(sockfd, sendline, strlen(sendline));

        if (Readline(sockfd, recvline, MAXLINE) == 0)
            Error::err_quit("str_cli: server terminated prematurely");

        Fputs(recvline, stdout);
    }
}


#endif
