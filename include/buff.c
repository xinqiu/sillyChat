#include "buff.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
//#include "hash.h"
//#include "socketHelper.h"
/********ContactorTable*********/

inline unsigned int hash_pjw(const char* name)
{
    unsigned int val = 0,i;
    for(; *name; ++name){
        val = (val << 2) + *name;
        if(i = val & ~0x3fff)
            val = (val ^ (i >> 12)) & 0x3fff;
    }
    return val;
}

void initContactorTable()
{
	int i;
	for(i = 0; i < hashTableSize; i++){
		pthread_mutex_init( &contactorTable[i].lock, NULL );
		contactorTable[i].addr = -1;
	}
}

int contactorReg(const char *name, const int addr)
{
	//get hash value
	unsigned int val = hash_pjw(name);

	pthread_mutex_lock( &contactorTable[val].lock );
	//register
	if( contactorTable[val].addr == -1 ){//check succ
		//contactorTable[val] = *(uint32_t *)ip;
		contactorTable[val].addr = addr;
		pthread_mutex_unlock( &contactorTable[val].lock );
		return 1;
	}
	else{	//check fail
		pthread_mutex_unlock( &contactorTable[val].lock );
		return 0;
	}

}

int contactorIsExist(const char *name)
{
	unsigned int val = hash_pjw(name);
	return contactorTable[val].addr;
}

int contactorGetAddr(const char *name)
{
	unsigned int val = hash_pjw(name);
	return (contactorTable[val].addr);
}

void contactorOffline(const char *name)
{
	unsigned int val = hash_pjw(name);
	assert( contactorTable[val].addr );

	pthread_mutex_lock( &contactorTable[val].lock );
	close(contactorTable[val].addr);
	contactorTable[val].addr = -1;
	pthread_mutex_unlock( &contactorTable[val].lock );
}

void contactorTableDestory()
{
	int i;
	for(i = 0; i < hashTableSize; i++){
		;//pthread_mutex_destory( &contactorTable[i].lock );	
	}
}

/*
void buffTest()
{

	initContactorTable();
	char *name = "zrs";
	char *ip = "192.168.1.1";
	assert( (contactorIsExist(name) == 0) );

	int result = contactorReg(name, ip);
	assert(result);

	char IP[20];
	struct in_addr testBuff = contactorGetIp(name);

	assert( (contactorIsExist(name) == 1) );
	//assert( (testBuff != NULL) );

	inet_ntop(AF_INET, (void *)&testBuff, IP, 16);
	//printf("inet_ntop: %s\n", IP);
	assert( (strcmp(IP, ip) == 0) );
	contactorTableDestory();
	//return 0;
}
*/

/*
int main()
{
	//buffTest();
	return 0;
}
*/
