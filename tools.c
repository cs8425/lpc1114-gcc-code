#include "tools.h"

void edge_init(edge* _ed){
	edge_reset(_ed);
}
void edge_pushT(edge* _ed, uint8_t d, uint8_t type) { // (位置, type)
	_ed->data[_ed->count] = (type > 0)? d : -d;
	_ed->count++;
}
void edge_push(edge* _ed, uint8_t d) {
	_ed->data[_ed->count] = d;
	_ed->count++;
}
int16_t edge_get_count(edge* _ed) {
	return _ed->count;
}
int16_t edge_get_L2R(edge* _ed, uint8_t index) { // 回傳左到右的邊界座標
	if(index < _ed->count){
		return _ed->data[index];
	}else{
		return -1;
	}
}
int16_t edge_get_L2R_pos(edge* _ed, uint8_t index, uint8_t m) { // 回傳左到右的邊界座標, (第n個, 位置要>m)
	for(uint8_t i=index; i<_ed->count; i++){
		int8_t data = ABS16(edge_get_L2R(_ed, i));
		if(data > m){
			return data;
		}
	}
	return -1;
}
int16_t edge_get_L2R_t(edge* _ed, uint8_t index, uint8_t m, int8_t type) { // 回傳左到右的邊界座標, (第n個, 位置要>m, type)
	for(uint8_t i=index; i<_ed->count; i++){
		int8_t data = ABS16(edge_get_L2R(_ed, i));
		if((type > 0) && (data > m)){
			if(edge_get_L2R(_ed, i) >= 0){
				return data;
			}
		}
		if((type < 0) && (data > m)){
			if(edge_get_L2R(_ed, i) < 0){
				return data;
			}
		}
	}
	return -1;
}
int16_t edge_get_R2L(edge* _ed, uint8_t index) { // 回傳右到左的邊界座標
	if(index < _ed->count){
		//return maxval -  1 - _ed->data[_ed->count - 1 - index];
		return _ed->data[_ed->count - 1 - index];
	}else{
		return -1;
	}
}
int16_t edge_get_R2L_pos(edge* _ed, uint8_t index, uint8_t m) { // 回傳右到左的邊界座標, (第n個, 位置要<m)
	for(uint8_t i=index; i<_ed->count; i++){
		int8_t data = ABS16(edge_get_R2L(_ed, i));
		if(data < m){
			return data;
		}
	}
	return -1;
}
int16_t edge_get_R2L_t(edge* _ed, uint8_t index, uint8_t m, int8_t type) { // 回傳右到左的邊界座標, (第n個, 位置要<m, type)
	for(uint8_t i=index; i<_ed->count; i++){
		int8_t data = ABS16(edge_get_R2L(_ed, i));
		if((type > 0) && (data < m)){
			if(edge_get_R2L(_ed, i) >= 0){
				return data;
			}
		}
		if((type < 0) && (data < m)){
			if(edge_get_R2L(_ed, i) < 0){
				return data;
			}
		}
	}
	return -1;
}
/*void edge_setNothing(edge* _ed) { // 設定狀態為"資料不可用"
	_ed->flag |= 0x01;
}
uint8_t edge_getNothing(edge* _ed) { // 確定狀態是否為"資料不可用"
	return _ed->flag;
}*/
void edge_reset(edge* _ed) {
	uint8_t i;
	_ed->flag = 0x00;
	_ed->count = 0;
	for(i=0; i<maxlength; i++){
		_ed->data[i] = 0;
	}
}


