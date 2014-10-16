#ifndef __SOCKETHELPER_H__
#define __SOCKETHELPER_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//int readn( int fd, char *bp, size_t len);

inline void send_wrapper(int sockfd, void *buf, unsigned int size, int flag)
{
	if( send(sockfd, buf, size, flag) < 0) {
		perror("send msg error");
        exit(-1);
    }
}

inline int socket_wrapper(int af, int type, int protocol)
{
	int sockfd;
	if( (sockfd = socket (af, type, protocol)) < 0 ) {
		perror("error in create socket");
		exit(-1);
	}
	else{
		return sockfd;	
	}
}

inline int accept_wrapper(int fd, struct sockaddr* addr, socklen_t* len)
{
	int sockfd;
	if( (sockfd = accept (fd, addr, len)) < 0 ) {
		perror("error in accept");
		exit(-1);
	}
	else{
		return sockfd;
	}
}

inline void recv_wrapper(int fd, void* buf, int len, int flag)
{
	int n = -1;
	if( (n =recv(fd, buf, len, 0)) < 0) {
		perror("error in recv");
		exit(-1);  
	} 
}

inline void inet_pton_wrapper(int af, const char *src, void *dst)
{
	if( inet_pton(af, src, dst) <=0 ){
		perror("error in inet_pton");
		exit(-1);
	}
}

inline void connect_wrapper(int sockfd, struct sockaddr * serv_addr, int addrlen)
{
	if ( connect(sockfd, serv_addr, addrlen) <0 ){
		perror("error in connect");
		exit(-1);
	}
}



#endif

