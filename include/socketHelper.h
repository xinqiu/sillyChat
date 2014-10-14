#ifndef socketHelper_Header_h
#define socketHelper_Header_h

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
		return sockfd;
	}
	else{
		perror("error in create socket");
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
	if ( connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) <0 ){
		perror("error in connect");
		exit(-1);
	}
}



#endif

