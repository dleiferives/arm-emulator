// (c) 2024 Dylan Leifer-Ives
// header guard //
#ifndef _BIGINT_H
#define _BIGINT_H

// includes //
#ifndef __WARTLESS_V_0_1
#define __WARTLESS_V_0_1
#include "wartless.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// defines // 
#define BIGINT_DEFAULT_RADIX 10
#define BIGINT_DEFAULT_CAPACITY 10

#ifndef exit_error
#define exit_error(STRING,...) do{fprintf(stderr,STRING,##__VA_ARGS__);exit(1);}while(0)
#endif

typedef struct{
	u64 *data;
	u64 capacity;
	u64 size;
	u64 radix;
}BigFixedInt_t;

// this is a fixed point representation of a big integer
// there is an imaginary dot in the middle of the array
// size is the max of the number of digits to the right of the dot or to the left of the dot
// if size*2 >= capacity, the array is resized by a factor of 2

BigFixedInt_t *BigFixedInt_new(u64 radix){
	BigFixedInt_t *bfi = (BigFixedInt_t *)malloc(sizeof(BigFixedInt_t));
	if(bfi == NULL) exit_error("BigFixedInt_new: failed to allocate memory for structure\n");
	bfi->data = (u64 *)malloc(sizeof(u64)*BIGINT_DEFAULT_CAPACITY);
	if(bfi->data == NULL) exit_error("BigFixedInt_new: failed to allocate memory for data\n");
	bfi->capacity = BIGINT_DEFAULT_CAPACITY;
	bfi->size = 0;
	bfi->radix = radix;
	return bfi;
}

void BigFixedInt_free(BigFixedInt_t *bfi){
	if(bfi == NULL) return;
	if(bfi->data != NULL) free(bfi->data);
	bfi->data = NULL;
	free(bfi);
}

// if size*2 >= capacity, the array is resized by a factor of 2
// when resizing, the array is copied to a new array, such that the old array's
// center is at the center of the new array
void BigFixedInt_resize(BigFixedInt_t *bfi){
	if(bfi == NULL) return;
	if(bfi->size*2 < bfi->capacity) return;
	u64 *new_data = (u64 *)malloc(sizeof(u64)*bfi->capacity*2);
	if(new_data == NULL) exit_error("BigFixedInt_resize: failed to allocate memory for new_data\n");
	// memset new data to 0
	memset(new_data,0,sizeof(u64)*bfi->capacity*2);
	// find the center of the old data
	// this is the offset from the start to copy the old data to the new data
	u64 offset = bfi->capacity/2;
	// copy the old data to the new data
	memcpy(new_data+offset,bfi->data,sizeof(u64)*bfi->capacity);
	free(bfi->data);
	bfi->data = new_data;
	bfi->capacity *= 2;
}

void BigFixedInt_radix_enforce(BigFixedInt_t *bfi){
	if(bfi == NULL) return;
	u64 carry = 0;
	for(u64 i = 0; i < bfi->capacity; i++){
		bfi->data[i] += carry;
		carry = bfi->data[i]/bfi->radix;
		bfi->data[i] %= bfi->radix;
	}
	if(carry > 0){
		BigFixedInt_resize(bfi);
		bfi->data[(bfi->capacity/2) + bfi->size++] = carry;
	} 
}

void BigFixedInt_unsigned_add_u64_into(BigFixedInt_t *bfi, u64 value){
	if(bfi == NULL) return;
	BigFixedInt_resize(bfi);
	bfi->data[(bfi->capacity/2)] += value;
	BigFixedInt_radix_enforce(bfi);
}

s8 BigFixedInt_compare_upper(BigFixedInt_t *a, BigFixedInt_t *b){
	if(a == NULL || b == NULL) return;
	u64 index_a_first_non_zero = a->capacity -1;
	u64 index_b_first_non_zero = b->capacity -1;
	u8 a_found = 0;
	u8 b_found = 0;
	for(u64 i = 0; i < a->capacity/2; i++){
		if(a->data[index_a_first_non_zero] == 0){
			a_found = 1;
			break;
		}
		index_a_first_non_zero--;
	}
	for(u64 i = 0; i < b->capacity/2; i++){
		if(b->data[index_b_first_non_zero] == 0){
			b_found = 1;
			break;
		}
		index_b_first_non_zero--;
	}
	if(a_found == 0 && b_found == 0) return 0;
	if(a_found == 0) return -1;
	if(b_found == 0) return 1;

	// compare the indexes
	if(index_a_first_non_zero > index_b_first_non_zero) return 1;
	if(index_a_first_non_zero < index_b_first_non_zero) return -1;
	// compare the values
	for(u64 i = 0; i < a->capacity/2; i++){
		if(a->data[index_a_first_non_zero] > b->data[index_b_first_non_zero]) return 1;
		if(a->data[index_a_first_non_zero] < b->data[index_b_first_non_zero]) return -1;
		index_a_first_non_zero--;
		index_b_first_non_zero--;
	}
	
	// they must be identical
	return 0;
}

s8 BigFixedInt_compare_lower(BigFixedInt_t *a, BigFixedInt_t *b){
	if(a == NULL || b == NULL) return;
	u64 index_a_first_non_zero = (a->capacity/2) -1;
	u64 index_b_first_non_zero = (b->capacity/2) -1;
	u8 a_found = 0;
	u8 b_found = 0;
	for(u64 i = 0; i < a->capacity/2; i++){
		if(a->data[index_a_first_non_zero] == 0){
			a_found = 1;
			break;
		}
		index_a_first_non_zero--;
	}
	for(u64 i = 0; i < b->capacity/2; i++){
		if(b->data[index_b_first_non_zero] == 0){
			b_found = 1;
			break;
		}
		index_b_first_non_zero--;
	}
	if(a_found == 0 && b_found == 0) return 0;
	if(a_found == 0) return -1;
	if(b_found == 0) return 1;

	// compare the indexes
	if(index_a_first_non_zero > index_b_first_non_zero) return 1;
	if(index_a_first_non_zero < index_b_first_non_zero) return -1;
	// compare the values
	for(u64 i = 0; i < a->capacity/2; i++){
		if(a->data[index_a_first_non_zero] > b->data[index_b_first_non_zero]) return 1;
		if(a->data[index_a_first_non_zero] < b->data[index_b_first_non_zero]) return -1;
		index_a_first_non_zero--;
		index_b_first_non_zero--;
	}
	
	// they must be identical
	return 0;
}

s8 BigFixedInt_compare(BigFixedInt_t *a, BigFixedInt_t *b){
	if(a == NULL || b == NULL) return;
	s8 upper = BigFixedInt_compare_upper(a,b);
	if(upper != 0) return upper;
	s8 lower = BigFixedInt_compare_lower(a,b);
	return lower;
}

BigFixedInt_t * BigFixedInt_clone(BigFixedInt_t *bfi){
	if(bfi == NULL) return NULL;
	BigFixedInt_t *clone = (BigFixedInt_t *)malloc(sizeof(BigFixedInt_t));
	if(clone == NULL) exit_error("BigFixedInt_clone: failed to allocate memory for clone\n");
	clone->size = bfi->size;
	clone->capacity = bfi->capacity;
	clone->data = (u64 *)malloc(sizeof(u64)*bfi->capacity);
	if(clone->data == NULL) exit_error("BigFixedInt_clone: failed to allocate memory for clone->data\n");
	memcpy(clone->data,bfi->data,sizeof(u64)*bfi->capacity);
	return clone;
}

void BigFixedInt_add_into(BigFixedInt_t *a, BigFixedInt_t *b){
	if(a == NULL || b == NULL) return;
	u64 carry = 0;
	for(u64 i = 0; i < a->capacity; i++){
		a->data[i] += b->data[i] + carry;
		carry = a->data[i]/a->radix;
		a->data[i] %= a->radix;
	}
	BigFixedInt_radix_enforce(a);
}

void BigFixedInt_subtract_into(BigFixedInt_t *a, BigFixedInt_t *b){
	if(a == NULL || b == NULL) return;
	todo here
}

void BigFixedInt_divide_into(BigFixedInt_t *a, BigFixedInt_t *b){
	if(a == NULL || b == NULL) return;
	BigFixedInt_t *result = BigFixedInt_new(a->radix);
	BigFixedInt_t *temp = BigFixedInt_new(a->radix);
	u64 carry = 0;
	for(u64 i = 0; i < a->size; i++){
		BigFixedInt_unsigned_add_u64_into(temp,a->data[(a->capacity/2) + i]);
		if(BigFixedInt_compare(temp,b) >= 0){
			BigFixedInt_subtract_into(temp,b);
			BigFixedInt_unsigned_add_u64_into(result,1);
		}
	}
	BigFixedInt_free(temp);
}

BigFixedInt_t *BigFixedInt_divide(BigFixedInt_t *a, BigFixedInt_t *b){
	BigFixedInt_t *result = BigFixedInt_clone(a);
	BigFixedInt_divide_into(result,b);
	return result;
}




#endif // _BIGINT_H
