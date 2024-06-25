// (c) 2024 Dylan Leifer-Ives
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __WARTLESS_V_0_1
#define __WARTLESS_V_0_1
#include "wartless.h"
#endif

#include "bigint.h"

#define assert(CONDITIONAL,STRING,...) if(!(CONDITIONAL)){printf("Error: %s:%d\n",__FILE__,__LINE__);printf(STRING,##__VA_ARGS__);exit(1);}

void print_bytes_as_hex(u8 *buffer, u32 len){
	assert((buffer != NULL), "Buffer is NULL\n");
	for(u32 i = 0; i < len; i++){
		printf("%02X ", buffer[i]);
	}
	printf("\n");
}

typedef enum{
	F_SIZE_B = 1,
	F_SIZE_H = 2,
	F_SIZE_S = 4,
	F_SIZE_D = 8,
	F_SIZE_Q = 16,
}REG_GP_FLOAT_SIZE_t;

typedef struct{
	u8 data[16]; // space for 128 bits
	REG_GP_FLOAT_SIZE_t size;
}REG_GP_FLOAT_t;

typedef enum{
	I_SIZE_B = 1,
	I_SIZE_H = 2,
	I_SIZE_W = 4,
	I_SIZE_X = 8,
}REG_GP_INT_SIZE_t;

typedef struct{
	u8 data[8];
	REG_GP_INT_SIZE_t size;
}REG_GP_INT_t;


// goes 1 2 4 8 6 2..... for each bit 
// 0 0 0 0 1 3 6 2 5 1 2 4 9 


s32 main(s32 argc,s8 **argv){

	return 0;
}
