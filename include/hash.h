#ifndef hash_Header_h
#define hash_Header_h

#define hashTableSize 16384
inline unsigned int hash_pjw(char* name)
{
    unsigned int val = 0,i;
    for(; *name; ++name){
        val = (val << 2) + *name;
        if(i = val & ~0x3fff)
            val = (val ^ (i >> 12)) & 0x3fff;
    }
    return val;
}

#endif