#include "buff.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "socketHelper.h"
/********ContactorTable*********/


void initContactorTable()
{
	memset(contactorTable, 0, sizeof(struct in_addr) * hashTableSize);
}


int contactorReg(char *name, const char *ip)
{
	//get hash value
	unsigned int val = hash_pjw(name);

	//register
	if( contactorTable[val] == NULL ){//check succ
		//contactorTable[val] = *(uint32_t *)ip;
		struct in_addr *tmp = (struct in_addr*) malloc(sizeof(struct in_addr));
		assert(tmp != NULL); 
		inet_pton_wrapper(AF_INET, ip, tmp);
		contactorTable[val] = tmp;
		return 1;
	}
	else{	//check fail
		return 0;
	}

}

int contactorIsExist(char *name)
{
	unsigned int val = hash_pjw(name);
	return contactorTable[val] == NULL? 0:1;
}

struct in_addr contactorGetIp(char *name)
{
	unsigned int val = hash_pjw(name);
	return *contactorTable[val];
}

void contactorOffline(char *name)
{
	unsigned int val = hash_pjw(name);
	assert( contactorTable[val] != NULL );
	free(contactorTable[val]);

	contactorTable[val] = NULL;
}

int main()
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
	printf("inet_ntop: %s\n", IP);

	return 0;
}