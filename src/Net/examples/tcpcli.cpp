#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct sockaddr SA;
int tcp_connect(const char *host, const char *serv);
static
struct addrinfo* host_serv(const char *host, const char *serv,
			   int family, int socktype);
#define MAXLINE 1024

int main(int argc, char *argv[])
{
    int	sockfd, n;
    char recvline[MAXLINE+1];
    socklen_t len;
    struct sockaddr_storage ss;

    if (argc != 4) {
        printf("usage: ./tcpcli <hostname/IPaddress> <service/port> <raw message>\n");
        return 0;
    }

    sockfd = tcp_connect(argv[1], argv[2]);

    if (sockfd < 0)
        return 1;
    
    len = sizeof(ss);
    getpeername(sockfd, (SA *)&ss, &len);
    //printf("connected to %s\n", sock_ntop_host((SA *)&ss, len));

    printf("send raw message:\n%s\n", argv[3]);
    write(sockfd, argv[3], strlen(argv[3]));
    printf("recv message:\n");
    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;	
        fputs(recvline, stdout);
    }
    exit(0);
}

int tcp_connect(const char *host, const char *serv)
{
    int sockfd, n;
    struct addrinfo *res, *ressave;

    res = host_serv(host, serv, AF_UNSPEC, SOCK_STREAM);
    if(res == NULL){
	fprintf(stderr, "tcp_connect error\n");
	return -1;
    }

    ressave = res;
    do{
	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if(sockfd < 0)
	    continue;

	if(connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
	    break;

	close(sockfd);
    }
    while((res = res->ai_next) != NULL);

    if(res == NULL){
	fprintf(stderr, "tcp_connect error for %s, %s", host, serv);
	return -1;
    }
    

    freeaddrinfo(ressave);

    return sockfd;
}

static
struct addrinfo* host_serv(const char *host, const char *serv,
			   int family, int socktype)
{
    int n;
    struct addrinfo hints, *res;

    bzero(&hints, sizeof(hints));
    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = family;
    hints.ai_socktype = socktype;

    if((n = getaddrinfo(host, serv, &hints, &res)) != 0)
	return (NULL);

    return res;
}
