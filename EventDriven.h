#ifndef __EventDriven_H
#define __EventDriven_H

#define QUEUE_SIZE 32

typedef struct {
	unsigned char buffer[QUEUE_SIZE];
	unsigned char head;
	unsigned char tail;

	unsigned char union_map[QUEUE_SIZE/8];

	unsigned char index;
	unsigned char offset;
} EventDriven;

void EventDriven_init(EventDriven *_ev);
void EventDriven_push(EventDriven *_ev, unsigned char c);
int EventDriven_get(EventDriven *_ev);
void EventDriven_reset(EventDriven *_ev);
int EventDriven_length(EventDriven *_ev);

inline int EventDriven_available(EventDriven *_ev){
	return (_ev->head == _ev->tail) ? 0 : 1;
}

unsigned char EventDriven_check_map(EventDriven *_ev, unsigned char c);
void EventDriven_go(EventDriven *_ev, unsigned char c);

#endif

