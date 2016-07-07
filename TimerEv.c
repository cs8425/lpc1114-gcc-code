
#include "EventDriven.h"
#include "TimerEv.h"

#include "lpc111x.h"

void us_ticker_init(void){

	SYSAHBCLKCTRL |= (1<<10); // Clock TIMER_1
	uint32_t PCLK = 48000000;

	TMR32B1TCR = 0x2;  // reset

	uint32_t prescale = PCLK / 1000000; // default to 1MHz (1 us ticks)
	TMR32B1PR = prescale - 1;
	TMR32B1TCR = 1; // enable = 1, reset = 0

}

uint32_t us_ticker_read(void) {
	return TMR32B1TC;
}

void TimerEv_init(TimerEv *tmev, EventDriven *_evlp) {
	tmev->evlp = _evlp;
	TimerEv_reset(tmev);
}

void TimerEv_add(TimerEv *tmev, unsigned char ev, unsigned int time, char repeat, char enable) {
	if (tmev->count < TIMER_SIZE) {
		tmev->slot[tmev->count] = ev;

		tmev->period[tmev->count] = time;
		tmev->last[tmev->count] = us_ticker_read();

		tmev->status_map[tmev->count] = ((repeat << 1)|enable) & 0x03;

		tmev->count++;
	}
}

void TimerEv_enable(TimerEv *tmev, unsigned char ev) {
	uint16_t i;
	for(i=0; i < tmev->count; i++){
		if(tmev->slot[i] == ev){
			tmev->status_map[i] |= isEN;
			return;
		}
	}
}

void TimerEv_disable(TimerEv *tmev, unsigned char ev) {
	uint16_t i;
	for(i=0; i < tmev->count; i++){
		if(tmev->slot[i] == ev){
			tmev->status_map[i] &= ~isEN;
			return;
		}
	}
}

void TimerEv_clear(TimerEv *tmev, unsigned char ev) {
	uint16_t i;
	for(i=0; i < tmev->count; i++){
		if(tmev->slot[i] == ev){
			tmev->status_map[i] |= isRm;
			tmev->status_map[i] &= ~isEN;
			TimerEv_merge(tmev);
			return;
		}
	}
}

void TimerEv_reset(TimerEv *tmev) {
	unsigned char i;
	tmev->count = 0;
	for(i=0; i<TIMER_SIZE; i++){
		tmev->status_map[i] = 0;
		tmev->slot[i] = 0;
		tmev->last[i] = 0;
		tmev->period[i] = 0;
	}
}

int TimerEv_length(TimerEv *tmev) {
	return tmev->count;
}

/*inline int TimerEv_available(TimerEv *tmev){
	return (tmev->count) ? 0 : 1;
}*/

void TimerEv_tick(TimerEv *tmev) {
	//uint32_t now = sysTimer.read_us();
	char mg = 0;
	uint32_t now = us_ticker_read();
	uint16_t i;
	uint32_t delta;
	for(i = 0; i < tmev->count; i++){
		if(tmev->status_map[i] & isEN){
			delta = now - tmev->last[i];
			if(delta > tmev->period[i]){
				EventDriven_push(tmev->evlp, tmev->slot[i]);
				tmev->last[i] = now;
				if(!(tmev->status_map[i] & isREP)){
					tmev->status_map[i] |= isRm;
					tmev->status_map[i] ^= isEN;
					mg = 1;
				}
			}
		}
	}
	if(mg) TimerEv_merge(tmev);
}

void TimerEv_merge(TimerEv *tmev) {
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
}

