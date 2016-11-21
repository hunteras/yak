#ifndef SOCK_H
#define SOCK_H

#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>

namespace yak
{
    typedef struct sockaddr SA;
    
    class Sock
    {
    public:
        Sock() = delete;
        ~Sock() = delete;

        static
        int
        bind_wild(int sockfd, int family) // bind the wildcard address and an ephemeral port to a socket
        {
            socklen_t len;

            switch (family) {
                case AF_INET: {
                    struct sockaddr_in sin;

                    bzero(&sin, sizeof(sin));
                    sin.sin_family = AF_INET;
                    sin.sin_addr.s_addr = htonl(INADDR_ANY);
                    sin.sin_port = htons(0);	/* bind ephemeral port */

                    if (bind(sockfd, (SA *) &sin, sizeof(sin)) < 0)
			return(-1);
                    len = sizeof(sin);
                    if (getsockname(sockfd, (SA *) &sin, &len) < 0)
			return(-1);
                    return(sin.sin_port);
                }
#ifdef	IPV6
                case AF_INET6: {
                    struct sockaddr_in6	sin6;

                    bzero(&sin6, sizeof(sin6));
                    sin6.sin6_family = AF_INET6;
                    sin6.sin6_addr = in6addr_any;
                    sin6.sin6_port = htons(0);	/* bind ephemeral port */

                    if (bind(sockfd, (SA *) &sin6, sizeof(sin6)) < 0)
			return(-1);
                    len = sizeof(sin6);
                    if (getsockname(sockfd, (SA *) &sin6, &len) < 0)
			return(-1);
                    return(sin6.sin6_port);
                }
#endif
            }
            return(-1);
        }

        static
        int
        cmp_addr(const struct sockaddr *sa1, const struct sockaddr *sa2,
                      socklen_t salen) // compares the address portion of two socket address structures
        {
            if (sa1->sa_family != sa2->sa_family)
		return(-1);

            switch (sa1->sa_family) {
                case AF_INET: {
                    return(memcmp( &((struct sockaddr_in *) sa1)->sin_addr,
                                   &((struct sockaddr_in *) sa2)->sin_addr,
                                   sizeof(struct in_addr)));
                }

#ifdef	IPV6
                case AF_INET6: {
                    return(memcmp( &((struct sockaddr_in6 *) sa1)->sin6_addr,
                                   &((struct sockaddr_in6 *) sa2)->sin6_addr,
                                   sizeof(struct in6_addr)));
                }
#endif

#ifdef	AF_UNIX
                case AF_UNIX: {
                    return(strcmp( ((struct sockaddr_un *) sa1)->sun_path,
                                   ((struct sockaddr_un *) sa2)->sun_path));
                }
#endif

#ifdef	HAVE_SOCKADDR_DL_STRUCT
                case AF_LINK: {
                    return(-1);		/* no idea what to compare here ? */
                }
#endif
            }
            return (-1);
        }

        static
        int
        cmp_port(const struct sockaddr *sa1, const struct sockaddr *sa2,
                      socklen_t salen) // compares the port number of two socket address structures.
        {
            if (sa1->sa_family != sa2->sa_family)
		return(-1);

            switch (sa1->sa_family) {
                case AF_INET: {
                    return( ((struct sockaddr_in *) sa1)->sin_port ==
                            ((struct sockaddr_in *) sa2)->sin_port);
                }

#ifdef	IPV6
                case AF_INET6: {
                    return( ((struct sockaddr_in6 *) sa1)->sin6_port ==
                            ((struct sockaddr_in6 *) sa2)->sin6_port);
                }
#endif

            }
            return (-1);
        }

        static
        int
        get_port(const struct sockaddr *sa, socklen_t salen) // returns just the port number
        {
            switch (sa->sa_family) {
                case AF_INET: {
                    struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

                    return(sin->sin_port);
                }

#ifdef	IPV6
                case AF_INET6: {
                    struct sockaddr_in6	*sin6 = (struct sockaddr_in6 *) sa;

                    return(sin6->sin6_port);
                }
#endif
            }

            return(-1);		/* ??? */
        }

        static
        char *
        ntop_host(const struct sockaddr *sa, socklen_t salen)
        // converts just the host portion of a socket address structure to presentation format(not the port number) 
        {
            static char str[128];		/* Unix domain is largest */

            switch (sa->sa_family) {
                case AF_INET: {
                    struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

                    if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == NULL)
			return(NULL);
                    return(str);
                }

#ifdef	IPV6
                case AF_INET6: {
                    struct sockaddr_in6	*sin6 = (struct sockaddr_in6 *) sa;

                    if (inet_ntop(AF_INET6, &sin6->sin6_addr, str, sizeof(str)) == NULL)
			return(NULL);
                    return(str);
                }
#endif

#ifdef	AF_UNIX
                case AF_UNIX: {
                    struct sockaddr_un	*unp = (struct sockaddr_un *) sa;

                    /* OK to have no pathname bound to the socket: happens on
                       every connect() unless client calls bind() first. */
                    if (unp->sun_path[0] == 0)
			strcpy(str, "(no pathname bound)");
                    else
			snprintf(str, sizeof(str), "%s", unp->sun_path);
                    return(str);
                }
#endif

#ifdef	HAVE_SOCKADDR_DL_STRUCT
                case AF_LINK: {
                    struct sockaddr_dl	*sdl = (struct sockaddr_dl *) sa;

                    if (sdl->sdl_nlen > 0)
			snprintf(str, sizeof(str), "%*s",
                                 sdl->sdl_nlen, &sdl->sdl_data[0]);
                    else
			snprintf(str, sizeof(str), "AF_LINK, index=%d", sdl->sdl_index);
                    return(str);
                }
#endif
                default:
                    snprintf(str, sizeof(str), "sock_ntop_host: unknown AF_xxx: %d, len %d",
                             sa->sa_family, salen);
                    return(str);
            }
            return (NULL);
        }

        // sets just the address portion of a socket address structure to the value pointed to by ptr
        static
        void
        set_addr(struct sockaddr *sa, socklen_t salen, const void *addr)
        {
            switch (sa->sa_family) {
                case AF_INET: {
                    struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

                    memcpy(&sin->sin_addr, addr, sizeof(struct in_addr));
                    return;
                }

#ifdef	IPV6
                case AF_INET6: {
                    struct sockaddr_in6	*sin6 = (struct sockaddr_in6 *) sa;

                    memcpy(&sin6->sin6_addr, addr, sizeof(struct in6_addr));
                    return;
                }
#endif
            }

            return;
        }

        // sets just the port number of a socket address structure
        static
        void
        set_port(struct sockaddr *sa, socklen_t salen, int port)
        {
            switch (sa->sa_family) {
                case AF_INET: {
                    struct sockaddr_in	*sin = (struct sockaddr_in *) sa;

                    sin->sin_port = port;
                    return;
                }

#ifdef	IPV6
                case AF_INET6: {
                    struct sockaddr_in6	*sin6 = (struct sockaddr_in6 *) sa;

                    sin6->sin6_port = port;
                    return;
                }
#endif
            }

            return;
        }

        // sets the address portion of a socket address structure to the wildcard
        static
        void
        set_wild(struct sockaddr *sa, socklen_t salen)
        {
            const void	*wildptr;

            switch (sa->sa_family) {
                case AF_INET: {
                    static struct in_addr in4addr_any;

                    in4addr_any.s_addr = htonl(INADDR_ANY);
                    wildptr = &in4addr_any;
                    break;
                }

#ifdef	IPV6
                case AF_INET6: {
                    wildptr = &in6addr_any;
                    break;
                }
#endif

                default:
                    return;
            }
            set_addr(sa, salen, wildptr);
            return;
        }

    };
}
#endif
