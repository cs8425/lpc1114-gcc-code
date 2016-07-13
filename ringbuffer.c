#include "ringbuffer.h"

RbufType prefilt_C[8] = {5, 3, 2, 2, 1, 1, 1, 1};

void Ringbuff_init(Ringbuff *buff) {
	Ringbuff_resetAs(buff, 0);
}

void Ringbuff_resetAs(Ringbuff *buff, RbufType num){
	uint16_t i;
	buff->head = 0;
	buff->tail = 0;
	buff->count = 0;
	for(i=0; i<RBUFSIZE; i++){
		buff->data[i] = num;
	}
}

void Ringbuff_push(Ringbuff *buff, RbufType num) {
	if(buff->count >= RBUFSIZE - 1){
		Ringbuff_shift(buff);
	}
	buff->count++;
	buff->data[buff->head++] = num;
	if(buff->head >= RBUFSIZE){
		buff->head = 0;
	}
}
void Ringbuff_unshift(Ringbuff *buff, RbufType num) {
	if(buff->count >= RBUFSIZE - 1){
		Ringbuff_pop(buff);
	}
	buff->count++;
	int32_t idx = buff->tail--;
	if(idx < 0){
		buff->tail = RBUFSIZE - 1;
	}
	buff->data[buff->tail] = num;

}
RbufType Ringbuff_pop(Ringbuff *buff) {
	RbufType ret = -1;
	if(buff->count > 0){
		buff->count--;
		int32_t idx = buff->head--;
		if(idx < 0){
			buff->head = RBUFSIZE - 1;
		}
		ret = buff->data[buff->head];
	}
	return ret;
}

RbufType Ringbuff_shift(Ringbuff *buff){
	RbufType ret = -1;
	if(buff->count > 0){
		buff->count--;
		ret = buff->data[buff->tail++];
		if(buff->tail >= RBUFSIZE){
			buff->tail = 0;
		}
	}
	return ret;
}


void Ringbuff_pushA(Ringbuff *buff, RbufType num) {
	RbufIdxType i = RBUFSIZE;
	while(i--){
		buff->data[i] = buff->data[i-1];
	}
	buff->data[0] = num;
}
RbufType Ringbuff_filt(Ringbuff *buff) {
	int32_t tmp = (buff->data[1] - buff->data[0])*5 + (buff->data[2] - buff->data[1])*3
		+ (buff->data[3] - buff->data[2])*2 + (buff->data[4] - buff->data[3])*2
		+ (buff->data[5] - buff->data[4]) + (buff->data[6] - buff->data[5])
		+ (buff->data[7] - buff->data[6]) + (buff->data[8] - buff->data[7]);
	return (tmp >> 4) + buff->data[0];
	/*RbufType ret = buff->data[buff->head-1];
	uint16_t i;
	uint16_t j;
	uint16_t tmp = 0;
	int16_t last = buff->head - RBUFSIZE + 1;
	if(last < 0){
		last += RBUFSIZE;
	}

	for(i=buff->tail, j=0; i<buff->head; i++) {
		
		tmp += prefilt_C[j] * buff->data[i];
	}

	return ret + (tmp >> 4);*/
}




