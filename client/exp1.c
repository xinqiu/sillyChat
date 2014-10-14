#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdint.h>

#include "../include/socketWrapper.h"

#define BUFFSIZE sizeof(sendAndRecvBuf)
#define DATASIZE 512
#define SERV_IP "114.212.191.33"	/*server ip*/
#define SERV_PORT 6666 /*port*/

const uint16_t initCity = 0x3100;
const uint16_t hasCity = 0x3300;
const uint16_t noCity = 0x3400;

struct sendAndRecvBuf
{
	uint16_t flag;
	char date[DATASIZE];
	//sendAndRecvBuf() : date(0) {}
};
typedef struct sendAndRecvBuf sendAndRecvBuf;

inline void setCity(sendAndRecvBuf *buf)
{

	buf->flag = htons(initCity);
	scanf("%s", buf->date);
	/*
	struct tmpBuf
	{
		char flag[2];
		char cityName[1022];
		tmpBuf() flag(initCity) {}
	} sendCityBuf;
	
	char cityName[1022];
	scanf("%s", cityName);
	//strcpy(sendBuf, sendCityBuf);
	//printf("%s\n", city);
	sprintf(sendBuf,"%c%c%s", initCity[0], initCity[1],cityName);
	*/
}



int main(int argc, char const *argv[])
{
	/* code */
	int sockfd;
	struct sockaddr_in servaddr;
	//char sendline[MAXLINE], recvline[MAXLINE];
	sendAndRecvBuf sendBuff, recvBuff;

	//basic check of arguments
	if(argc != 1){
		perror("error in arguments");
		exit(-1);
	}

	//create socket for the client
	//if sockfd < 0 there was an error in the creation of the socket
	sockfd = socket_wrapper (AF_INET, SOCK_STREAM, 0);

	//init
	memset(&servaddr, 0 ,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton_wrapper(AF_INET, SERV_IP, &servaddr.sin_addr);
		

	//connect to server
	connect_wrapper(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)); 

	setCity(&sendBuff);
	send_wrapper(sockfd, &sendBuff, BUFFSIZE, 0);
	//printf("%X %s\n", (sendline[0]), &(sendline[1]));

	while(1);
	return 0;
}


