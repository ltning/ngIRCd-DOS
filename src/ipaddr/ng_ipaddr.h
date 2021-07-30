/*
 * (c) 2008 Florian Westphal <fw@strlen.de>, public domain.
 */

#ifndef NG_IPADDR_HDR
#define NG_IPADDR_HDR

#include "portab.h"

/**
 * @file
 * Functions for AF_ agnostic ipv4/ipv6 handling (header).
 */

#define ntohs(x) (((x)>>8)|((x)<<8))
#define socklen_t size_t

struct sockaddr_in {
    short            family;   // e.g. AF_INET
    unsigned short   port;     // e.g. htons(3490)
    unsigned char    sin_addr[4];     // see struct in_addr, below
    char             zero[8];  // zero this if you want to
    char             ascii[20];
};

#include <assert.h>
#include <string.h>
//#include <sys/socket.h>
//#include <netinet/in.h>

#ifdef HAVE_ARPA_INET_H
# include <arpa/inet.h>
#else
# define PF_INET AF_INET
#endif


#ifdef WANT_IPV6
#define NG_INET_ADDRSTRLEN      INET6_ADDRSTRLEN
#else
#define NG_INET_ADDRSTRLEN      16
#endif


#ifdef WANT_IPV6
typedef union {
    char ascii[NG_INET_ADDRSTRLEN];
    UINT8 raw[4];
    UINT16 port;
} ng_ipaddr_t;
#else
/* assume compiler can't deal with typedef struct {... */
struct NG_IP_ADDR_DONTUSE {
        struct sockaddr_in sin4;
};
typedef struct NG_IP_ADDR_DONTUSE ng_ipaddr_t;
#endif


static inline int
ng_ipaddr_af(const ng_ipaddr_t *a)
{
/*        assert(a != NULL);
#ifdef WANT_IPV6
        return a->sa.sa_family;
#else
        assert(a->sin4.sin_family == 0 || a->sin4.sin_family == AF_INET);
        return a->sin4.sin_family;
#endif*/
return 0;
}


static inline socklen_t
ng_ipaddr_salen(const ng_ipaddr_t *a)
{
/*        assert(a != NULL);
#ifdef WANT_IPV6
        assert(a->sa.sa_family == AF_INET || a->sa.sa_family == AF_INET6);
        if (a->sa.sa_family == AF_INET6)
                return (socklen_t)sizeof(a->sin6);
#endif
        assert(a->sin4.sin_family == AF_INET);
        return (socklen_t)sizeof(a->sin4);*/
        return 1;
}


static inline UINT16
ng_ipaddr_getport(const ng_ipaddr_t *a)
{
#ifdef WANT_IPV6
        //int af = a->sa.sa_family;

        assert(a != NULL);
        //assert(af == AF_INET || af == AF_INET6);

        if (af == AF_INET6)
                return ntohs(a->sin6.sin6_port);
#endif

        assert(a != NULL);
        //assert(a->sin4.sin_family == AF_INET);
        //return ntohs(a->sin4.port);
        return a->sin4.port;
}

/*
 * init a ng_ipaddr_t object.
 * @param addr: pointer to ng_ipaddr_t to initialize.
 * @param ip_str: ip address in dotted-decimal (ipv4) or hexadecimal (ipv6) notation
 * @param port: transport layer port number to use.
 */
GLOBAL bool ng_ipaddr_init PARAMS((ng_ipaddr_t *addr, const char *ip_str, UINT16 port));

/* set sin4/sin6_port, depending on a->sa_family */
GLOBAL void ng_ipaddr_setport PARAMS((ng_ipaddr_t *a, UINT16 port));

/* return true if a and b have the same IP address. If a and b have different AF, return false. */
GLOBAL bool ng_ipaddr_ipequal PARAMS((const ng_ipaddr_t *a, const ng_ipaddr_t *b));


#ifdef WANT_IPV6
/* convert struct sockaddr to string, returns pointer to static buffer */
GLOBAL const char *ng_ipaddr_tostr PARAMS((const ng_ipaddr_t *addr));

/* convert struct sockaddr to string. dest must be NG_INET_ADDRSTRLEN bytes long */
GLOBAL bool ng_ipaddr_tostr_r PARAMS((const ng_ipaddr_t *addr, char *dest));
#else

char* inet_ntoa(ng_ipaddr_t *addr);

static inline const char*
ng_ipaddr_tostr(const ng_ipaddr_t *addr)
{
        //assert(addr != NULL);
        //return inet_ntoa((ng_ipaddr_t *)addr);
        //return "192.168.1.8";
        sprintf((char*)addr->sin4.ascii, "%u.%u.%u.%u",
			addr->sin4.sin_addr[0],
			addr->sin4.sin_addr[1],
			addr->sin4.sin_addr[2],
			addr->sin4.sin_addr[3]);
		return addr->sin4.ascii;
}

static inline bool
ng_ipaddr_tostr_r(const ng_ipaddr_t *addr, char *d)
{
        //assert(addr != NULL);
        //assert(d != NULL);
        //strlcpy(d, inet_ntoa((ng_ipaddr_t *)addr), NG_INET_ADDRSTRLEN);
        //d = "192.168.1.8";
        sprintf((char*)addr->sin4.ascii, "%u.%u.%u.%u",
			addr->sin4.sin_addr[0],
			addr->sin4.sin_addr[1],
			addr->sin4.sin_addr[2],
			addr->sin4.sin_addr[3]);
		strlcpy(d, addr->sin4.ascii, NG_INET_ADDRSTRLEN);
        return true;
}
#endif
#endif

/* -eof- */
