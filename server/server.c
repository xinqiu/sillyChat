#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>

#include "../include/buff.h"
#include "../include/socketHelper.h"


#define SERV_IP "192.168.1.1"
#define SERV_PORT 6666 /*port*/
#define LISTENQ 8

#define printfFuncName()	do { printf("in %s\n", __FUNCTION__); } while(0)

inline void serv_do_response_error(const int sockfd, uint8_t errorType, MSGBuff sendMsg)
{
	sendMsg.type = errorType;
	send_wrapper(sockfd, &sendMsg, sizeof(MSGBuff), 0);
	return;
}

inline void serv_do_response_OK(const int sockfd)
{
	MSGBuff sendMsg = {MSGOK};
	send_wrapper(sockfd, &sendMsg, sizeof(MSGBuff), 0);
	return;
}

int serv_do_register(const int fd, const MSGBuff msg)
{
	//printfFuncName();
	if( contactorReg(msg.src, fd) ){
		serv_do_response_OK(fd);
		return 1;
	}
	else{
		serv_do_response_error(fd, MSGERROR_REG, msg);
		return 0;
	}
}

inline void serv_do_offline(const char *username)
{
	//printfFuncName();
	assert( contactorIsExist(username) );
	contactorOffline(username);
	printf("user: %s offline\n", username);
}

void serv_do_chat(const MSGBuff msg)
{
	//printfFuncName();
	int fd = contactorGetAddr(msg.des);
	if ( fd < 0 ){
		fd = contactorGetAddr(msg.src);
		serv_do_response_error(fd, MSGERROR_NOUSER, msg);
	}
	else{
		send_wrapper(fd, (void *)&msg, sizeof(msg), 0);
	}
}

void *do_task(void* connfd)
{
	//printf("new task\n");
	int fd = *(int *)connfd;
	int authFlag = 0; //用户是否注册
	char username[32];
	//printf("in do task connfd is %d\n", fd);
	uint8_t msgtype;
	do{
		MSGBuff msg = {MSGEMPTY};
		recv_wrapper(fd, (void*)&msg, sizeof(MSGBuff), 0);
		msgtype = msg.type;

		switch (msgtype){
			case MSGREG : {
				if( serv_do_register(fd, msg) ){
					assert(authFlag == 0);
					strcpy(username, msg.src);
					authFlag = 1;
				} 
				break;
			}

			case MSGCHAT:	serv_do_chat(msg); break;
			case MSGEMPTY:
			case MSGOFFLINE: if(authFlag) serv_do_offline(username); break;
			default:	printf("error in do task switch type is %d\n", msg.type); exit(-1);
		}
	}while(msgtype != MSGEMPTY && msgtype != MSGOFFLINE);
	close(fd);
	pthread_exit(0);
}

void servReportRun(struct in_addr addr)
{
	char IP[20];
	inet_ntop(AF_INET, (void *)&addr, IP, 16);
	printf( "Server running in %s waiting for connections\n", IP);
}

int main(int argc, char **argv)
{

	int listenfd, connfd, n;
	//pthread_t threadQ[LISTENQ];
	pthread_attr_t attr;

	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	//create a socket
	listenfd = socket_wrapper(AF_INET, SOCK_STREAM, 0);

	//preparation of the socket address
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	//bind the socket
	bind( listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr) );

	//listen to the socket by creating a connection
	//queue, then wait for clients
	initContactorTable();
	listen(listenfd, LISTENQ);
	
	servReportRun(servaddr.sin_addr);

	while(1)
	{
		clilen = sizeof(cliaddr);

		//accept a connection
		connfd = accept_wrapper(listenfd, (struct sockaddr *) &cliaddr, &clilen);
		printf("%s\n", "Received request...");

		pthread_t tid;
		int ret = pthread_create(&tid, NULL, do_task, (void *)&connfd);
		if( ret != 0 ){
			perror("error in pthread_create");
		}
		else{
			pthread_detach(tid);
		}	
	}


	

	return 0;
}
