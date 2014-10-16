#ifndef __BUFF_H_
#define __BUFF_H_

#include <pthread.h>
#include <stdint.h>


/********ContactorTable*********/
//struct in_addr *contactorTable[hashTableSize];
#define hashTableSize 16384
struct _contactorNode
{
	pthread_mutex_t lock;
	//struct in_addr *addr;
	int addr;
	//struct _contactorNode(): lock(PTHREAD_MUTEX_INITIALIZER), addr(NULL) {};
	//struct contactorNode() : lock(PTHREAD_MUTEX_INITIALIZER), addr(NULL) {};
};
typedef struct _contactorNode contactorNode;
contactorNode contactorTable[hashTableSize];

void initContactorTable();
int contactorReg(const char *name, const int addr);
int contactorIsExist(const char *name);
int contactorGetAddr(const char *name);
void contactorOffline(const char *name);
void contactorTableDestory();

//test
void buffTest();

/*************MSG**************/

#define MSGERROR_REG	0x00
#define MSGERROR_NOUSER 0x10
#define MSGOK		0x01
#define MSGREG 		0x02	//注册
#define MSGOFFLINE	0x03	//下线
#define MSGCHAT		0x04	//交流
#define MSGEMPTY 	0xFF	//空
typedef struct 
{
	uint8_t type;
	char src[32];
	char des[32];
	char data[512];
} MSGBuff;




#endif