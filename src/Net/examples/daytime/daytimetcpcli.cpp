#include "Error.h"
#include "Sock.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <unistd.h>

#define MAXLINE 4096

using namespace yak;

int
main(int argc, char **argv)
{
    int	sockfd, n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;

    if (argc != 2)
        Error::err_quit("usage: %s <IPaddress>", argv[0]);

    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        Error::err_sys("socket error");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(13);	/* daytime server */
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        Error::err_quit("inet_pton error for %s", argv[1]);

    if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
        Error::err_sys("connect error");

    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;	/* null terminate */
        if (fputs(recvline, stdout) == EOF)
            Error::err_sys("fputs error");
    }
    if (n < 0)
        Error::err_sys("read error");

    exit(0);
}
