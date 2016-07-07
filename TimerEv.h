#ifndef __timerEvent_H
#define __timerEvent_H

#include <stdint.h>

#include "EventDriven.h"
//#include "us_ticker_api.h"

#define TIMER_SIZE 8

#define isEN  0x01
#define isREP 0x02
#define isRm  0x04

typedef struct {
	uint32_t last[TIMER_SIZE];
	uint32_t period[TIMER_SIZE];

	uint16_t count;

	unsigned char slot[TIMER_SIZE];
	unsigned char status_map[TIMER_SIZE];

	EventDriven *evlp;
} TimerEv;

#define US_TICKER_TIMER TMR32B1

void us_ticker_init(void);
uint32_t us_ticker_read(void);


void TimerEv_init(TimerEv *tmev, EventDriven *_evlp);
void TimerEv_add(TimerEv *tmev, unsigned char ev, unsigned int time, char repeat, char enable);
void TimerEv_enable(TimerEv *tmev, unsigned char ev);
void TimerEv_disable(TimerEv *tmev, unsigned char ev);
void TimerEv_clear(TimerEv *tmev, unsigned char ev);
void TimerEv_reset(TimerEv *tmev);
int TimerEv_length(TimerEv *tmev);
void TimerEv_tick(TimerEv *tmev);

inline int TimerEv_available(TimerEv *tmev){
	return (tmev->count) ? 0 : 1;
}
void TimerEv_merge(TimerEv *tmev);
/*static void TimerEv_merge(TimerEv *tmev) {
	uint16_t i;
	uint16_t dec = 0;
	for(i = 0; i < tmev->count; i++){
		if(tmev->status_map[i] & isRm){
			if(i+1 < tmev->count){
				tmev->status_map[i] = tmev->status_map[i+1];
				tmev->last[i] = tmev->last[i+1];
				tmev->slot[i] = tmev->slot[i+1];
				tmev->period[i] = tmev->period[i+1];
				tmev->status_map[i+1] |= isRm;
			}
			dec++;
		}
	}
	tmev->count -= dec;
}*/

#endif
