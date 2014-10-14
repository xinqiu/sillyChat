#ifndef BUFF_Header_h
#define BUFF_Header_h

#include <pthread.h>
#include <stdint.h>
#include "hash.h"

/********ContactorTable*********/
//struct in_addr *contactorTable[hashTableSize];
struct _contactorNode
{
	pthread_mutex_t lock;
	struct in_addr *addr;
	//struct _contactorNode(): lock(PTHREAD_MUTEX_INITIALIZER), addr(NULL) {};
	//struct contactorNode() : lock(PTHREAD_MUTEX_INITIALIZER), addr(NULL) {};
};
typedef struct _contactorNode contactorNode;
contactorNode contactorTable[hashTableSize];

void initContactorTable();
int contactorReg(char *name, const char *ip);
int contactorIsExist(char *name);
struct in_addr contactorGetIp(char *name);
void contactorOffline(char *name);
void contactorTableDestory();

//test
void buffTest();

/*************MSG**************/

#define MSGREG 		0x00	//注册
#define MSGOFFLINE	0x01	//下线
#define MSGCHAT		0x02	//交流
typedef struct 
{
	uint8_t type;
	char src[32];
	char des[32];
	char data[512];
} MSGBuff;




#endif