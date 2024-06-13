// (c) 2024 Dylan Leifer-Ives
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __WARTLESS_V_0_1
#include "wartless.h"

#define assert(CONDITIONAL,STRING,...) if(!(CONDITIONAL)){printf("Error: %s:%d\n",__FILE__,__LINE__);printf(STRING,##__VA_ARGS__);exit(1);}


typedef union{
	u32 w;
	u64 x;
}REG_GP_Integer_t;

typedef union{
	u8 b;
	u16 h;
	u32 f;
	u64 d;
	u64 q[2];
}REG_GP_Float_t;

void print_bytes_as_hex(u8 *buffer, u32 len){
	assert((buffer != NULL), "Buffer is NULL\n");
	for(u32 i = 0; i < len; i++){
		printf("%02X ", buffer[i]);
	}
	printf("\n");
}

s32 main(s32 argc,s8 **argv){
	REG_GP_Float_t f;
	f.q[0] = 0xEEEEEEEEEEEEEEEE;
	f.q[1] = 0xFFFFFFFFFFFFFFFF;
	f.d = (double)0xAAAAAAAAAAAAAAAA;
	f.f = (float)0xBBBBBBBB;
	f.h = 0xCCCC;
	f.b = 0xDD;

	print_bytes_as_hex((u8*)&f,sizeof(u64) * 2);
	return 0;
}
