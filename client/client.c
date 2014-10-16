#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include "../include/buff.h"
#include "../include/socketHelper.h"

//#define SERV_IP "192.168.1.1"
#define SERV_IP "127.0.0.1"
#define SERV_PORT 6666 /*port*/

char username[32];

int client_do_register(int fd)
{
	printf("please enter your username: ");
	scanf("%s", username);

	MSGBuff sendMsg = {MSGREG};
	strcpy(sendMsg.src, username);
	send_wrapper(fd, &sendMsg, sizeof(MSGBuff), 0);

	MSGBuff recvMsg;
	recv_wrapper(fd, (void*)&recvMsg, sizeof(MSGBuff), 0);
	if( recvMsg.type == MSGOK ){
		printf("welcome %s\n",username);
		return 1;
	}
	else{
		assert(recvMsg.type == MSGERROR_REG);
		printf("error: username %s has already existed!!!!\n", username);
		return 0;
	}

}

void* client_do_recvMsg(void *argv)
{
	int fd = *(int *)argv;

	while(1){
		MSGBuff msg;
		recv_wrapper(fd, (void*)&msg, sizeof(MSGBuff), 0);
		switch(msg.type){
			case MSGCHATALL:
			case MSGCHAT : {
				printf("from %s\n%s\n", msg.src, msg.data);
				break;
			}
			case MSGGETALL	:{
				printf("%s\n", msg.data);
				break;
			}

			case MSGERROR_NOUSER : {
				assert( strcmp(username, msg.src) == 0);
				printf("%s no such user or user offline\n", msg.des);
				break;
			}

			default:	printf("error in do recv switch type is %d\n", msg.type); exit(-1);

		}
	}
}

void* client_do_sendMsg(void *argv)
{
	int fd = *(int *)argv;
	while(1)
	{
		printf("(a):talk to all users\n");
		printf("(s):talk to single user\n");
		printf("(g):get all users\n");
		printf("(e):exit\n");
		char type[2];
		scanf("%s",type);

		if(strcmp(type, "s") == 0){
			MSGBuff msg = {MSGCHAT};
			strcpy(msg.src, username);
			scanf("%s", msg.des);
			scanf("%s", msg.data);
			send_wrapper(fd, &msg, sizeof(MSGBuff), 0);
		}

		else if(strcmp(type, "a") == 0){
			MSGBuff msg = {MSGCHATALL};
			strcpy(msg.src, username);
			//scanf("%s", msg.des);
			scanf("%s", msg.data);
			send_wrapper(fd, &msg, sizeof(MSGBuff), 0);
		}

		else if(strcmp(type, "g") == 0){
			MSGBuff msg = {MSGGETALL};
			strcpy(msg.src, username);
			send_wrapper(fd, &msg, sizeof(MSGBuff), 0);
		}
		else if(strcmp(type, "e") == 0){
			MSGBuff msg = {MSGOFFLINE};
			strcpy(msg.src, username);
			send_wrapper(fd, &msg, sizeof(MSGBuff), 0);
			pthread_exit(0);
		}

		else{
			printf("error in client send type is %s\n",type);
		}
		sleep(1);
	}
}



int main(int argc, char const *argv[])
{
	/* code */
	int sockfd;
	struct sockaddr_in servaddr;
	char ip[20];
	//char sendline[MAXLINE], recvline[MAXLINE];

	//basic check of arguments
	if(argc != 1){
		perror("error in arguments");
		exit(-1);
	}
	//strcpy(ip, argv[1]);

	//create socket for the client
	sockfd = socket_wrapper (AF_INET, SOCK_STREAM, 0);

	//init
	memset(&servaddr, 0 ,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton_wrapper(AF_INET, SERV_IP, &servaddr.sin_addr);
		

	//connect to server
	connect_wrapper(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)); 

	//register
	while( client_do_register(sockfd) == 0 );

	pthread_t listenTid;
	int ret = pthread_create(&listenTid, NULL, client_do_recvMsg, (void *)&sockfd);
	if( ret != 0 ){
		perror("error in pthread_create listen");
	}
	

	pthread_t sendTid;
	ret = pthread_create(&sendTid, NULL, client_do_sendMsg, (void *)&sockfd);
	if( ret != 0 ){
		perror("error in pthread_create send");
	}
	
	pthread_join(sendTid, NULL);	
	//while(1);
	return 0;
}


