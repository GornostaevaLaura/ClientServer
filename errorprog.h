#ifndef ERRORPROG_H
#define  ERRORPROG_H

#include <sys/types.h>
#include <sys/socket.h>



//Обертки для исключения ошибки

int Socket(int domain, int type, int protocol);

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

int Listen(int sockfd, int backlog);

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

int Connect(int clientfd, struct sockaddr *addr, socklen_t addrlen);

void Inet_pton(int af, const char *src, void *dst);


#endif
