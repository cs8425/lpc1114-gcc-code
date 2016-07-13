#ifndef _RINGBUFF_H
#define _RINGBUFF_H

#include <stdint.h>

#define RbufType int16_t
#define RbufIdxType uint16_t
#define RBUFSIZE 9
typedef struct {
	volatile RbufIdxType head;
	volatile RbufIdxType tail;
	volatile unsigned count;
	RbufType data[RBUFSIZE];
} Ringbuff;


#define Ringbuff_new(X) Ringbuff X = { .head = 0, .tail = 0, .count = 0, .data = {0} }

void Ringbuff_init(Ringbuff *buff);
void Ringbuff_push(Ringbuff *buff, RbufType num);
void Ringbuff_unshift(Ringbuff *buff, RbufType num);

RbufType Ringbuff_pop(Ringbuff *buff);
RbufType Ringbuff_shift(Ringbuff *buff);

void Ringbuff_resetAs(Ringbuff *buff, RbufType num);
inline int Ringbuff_length(Ringbuff *buff) {
	return buff->count;
}

void Ringbuff_pushA(Ringbuff *buff, RbufType num);
RbufType Ringbuff_filt(Ringbuff *buff);


#endif

