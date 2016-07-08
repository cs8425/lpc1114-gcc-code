#include "EventDriven.h"

static inline void _EventDriven_getbit(EventDriven *_ev, unsigned char c) {
	unsigned char i = 0;

	while(c >= 8) {
		i++;
		c -= 8;
	}
	_ev->index = i;
	_ev->offset = c;
}

static inline void _EventDriven_set_map(EventDriven *_ev, unsigned char c) {
	_EventDriven_getbit(_ev, c);
	_ev->union_map[_ev->index] |= (1 << _ev->offset);
}

static inline void _EventDriven_clear_map(EventDriven *_ev, unsigned char c) {
	_EventDriven_getbit(_ev, c);
	_ev->union_map[_ev->index] &= ~(1 << _ev->offset);
}

void EventDriven_init(EventDriven *_ev){
	unsigned char i;
	_ev->head = 0;
	_ev->tail = 0;
	for(i=0; i<QUEUE_SIZE/8; i++){
		_ev->union_map[i] = 0;
	}
}

void EventDriven_push(EventDriven *_ev, unsigned char c){
    int i = (unsigned char)(_ev->head + 1);
    
    if(i == QUEUE_SIZE) i = 0;

    if (i != _ev->tail) {
        _ev->buffer[_ev->head] = c;
        _ev->head = i;
        
        _EventDriven_set_map(_ev, c);
    }
}

int EventDriven_get(EventDriven *_ev) {
	// if the head isn't ahead of the tail, we don't have any characters
	if (_ev->head == _ev->tail) {
		return -1;
	} else {
		unsigned char c = _ev->buffer[_ev->tail];

		_EventDriven_clear_map(_ev, c);

		_ev->tail = (unsigned char)(_ev->tail + 1);
		if(_ev->tail == QUEUE_SIZE) _ev->tail = 0;
		return c;
	}
}

void EventDriven_reset(EventDriven *_ev) {
	unsigned char i;
	_ev->head = _ev->tail;
	for(i=0; i<QUEUE_SIZE/8; i++){
		_ev->union_map[i] = 0;
	}
}

int EventDriven_length(EventDriven *_ev) {
	if(_ev->head == _ev->tail) return 0;

	if(_ev->head > _ev->tail){
		return _ev->head - _ev->tail + 1;
	}else{
		return _ev->head - _ev->tail + QUEUE_SIZE;
	}
}

/*inline int EventDriven_available(EventDriven *_ev){
	return (_ev->head == _ev->tail) ? 0 : 1;
}*/

unsigned char EventDriven_check_map(EventDriven *_ev, unsigned char c) {
	_EventDriven_getbit(_ev, c);

	unsigned char byte = _ev->union_map[_ev->index];

	return (byte & (1 << _ev->offset));
}

void EventDriven_go(EventDriven *_ev, unsigned char c) {
	int i = _ev->tail;

	while(i != _ev->head) {
		// do something to buffer[i]
		i++;
		if(i == QUEUE_SIZE) i = 0;
	}
}
/*
static inline void _EventDriven_getbit(EventDriven *_ev, unsigned char c) {
	unsigned char i = 0;

	while(c >= 8) {
		i++;
		c -= 8;
	}
	_ev->index = i;
	_ev->offset = c;
}
static inline void _EventDriven_set_map(EventDriven *_ev, unsigned char c) {
	_EventDriven_getbit(_ev, c);
	_ev->union_map[_ev->index] |= (1 << _ev->offset);
}
static inline void _EventDriven_clear_map(EventDriven *_ev, unsigned char c) {
	_EventDriven_getbit(_ev, c);
	_ev->union_map[_ev->index] &= ~(1 << _ev->offset);
}*/

