#include "buff.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "socketHelper.h"
/********ContactorTable*********/


void initContactorTable()
{
	int i;
	for(i = 0; i < hashTableSize; i++){
		pthread_mutex_init( &contactorTable[i].lock, NULL );
		contactorTable[i].addr = NULL;
	}
}

int contactorReg(char *name, const char *ip)
{
	//get hash value
	unsigned int val = hash_pjw(name);

	pthread_mutex_lock( &contactorTable[val].lock );
	//register
	if( contactorTable[val].addr == NULL ){//check succ
		//contactorTable[val] = *(uint32_t *)ip;
		struct in_addr *tmp = (struct in_addr*) malloc(sizeof(struct in_addr));
		assert(tmp != NULL); 
		inet_pton_wrapper(AF_INET, ip, tmp);
		contactorTable[val].addr = tmp;
		pthread_mutex_unlock( &contactorTable[val].lock );
		return 1;
	}
	else{	//check fail
		pthread_mutex_unlock( &contactorTable[val].lock );
		return 0;
	}

}

int contactorIsExist(char *name)
{
	unsigned int val = hash_pjw(name);
	return contactorTable[val].addr == NULL? 0:1;
}

struct in_addr contactorGetIp(char *name)
{
	unsigned int val = hash_pjw(name);
	return *(contactorTable[val].addr);
}

void contactorOffline(char *name)
{
	unsigned int val = hash_pjw(name);
	assert( contactorTable[val].addr != NULL );

	pthread_mutex_lock( &contactorTable[val].lock );
	free(contactorTable[val].addr);
	contactorTable[val].addr = NULL;
	pthread_mutex_unlock( &contactorTable[val].lock );
}

void contactorTableDestory()
{
	int i;
	for(i = 0; i < hashTableSize; i++){
		pthread_mutex_destory( &contactorTable[i].lock );	
	}
}

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


/*
int main()
{
	buffTest();
	return 0;
}
*/