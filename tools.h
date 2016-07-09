#ifndef _TOOL_H
#define _TOOL_H

#include <stdint.h>

inline uint16_t ABS32(int32_t num) {
    return (num < 0) ? -num : num;
}

inline uint16_t ABS16(int16_t num) {
    return (num < 0) ? -num : num;
}

// 存放邊界, 可回傳最左邊的邊界&最右邊的邊界
#define maxlength 128
#define maxval 106

typedef struct {
	uint8_t count;
	uint8_t flag;
	int8_t data[maxlength];
} edge;

void edge_init(edge* _ed);
void edge_pushT(edge* _ed, uint8_t d, uint8_t type); // (位置, type)
void edge_push(edge* _ed, uint8_t d);
int16_t edge_get_count(edge* _ed);
int16_t edge_get_L2R(edge* _ed, uint8_t index); // 回傳左到右的邊界座標
int16_t edge_get_L2R_pos(edge* _ed, uint8_t index, uint8_t m);// 回傳左到右的邊界座標, (第n個, 位置要>m)
int16_t edge_get_L2R_t(edge* _ed, uint8_t index, uint8_t m, int8_t type); // 回傳左到右的邊界座標, (第n個, 位置要>m, type)
int16_t edge_get_R2L(edge* _ed, uint8_t index); // 回傳右到左的邊界座標
int16_t edge_get_R2L_pos(edge* _ed, uint8_t index, uint8_t m); // 回傳右到左的邊界座標, (第n個, 位置要<m)
int16_t edge_get_R2L_t(edge* _ed, uint8_t index, uint8_t m, int8_t type); // 回傳右到左的邊界座標, (第n個, 位置要<m, type)
void edge_reset(edge* _ed);
/*
void edge_setNothing(edge* _ed); // 設定狀態為"資料不可用"
uint8_t edge_getNothing(edge* _ed) // 確定狀態是否為"資料不可用"
*/
inline void edge_setNothing(edge* _ed) { // 設定狀態為"資料不可用"
	_ed->flag |= 0x01;
}
inline uint8_t edge_getNothing(edge* _ed) { // 確定狀態是否為"資料不可用"
	return _ed->flag;
}



// 資料格式轉換用
union num24b {
	float f;

	int32_t i32;
	uint32_t ui32;

	int16_t i16[2];
	uint16_t ui16[2];

	char c[4];
	unsigned char uc[4];

	int8_t i8[4];
	uint8_t ui8[4];
};

union num22b {
	int16_t i16;
	uint16_t ui16;

	char c[2];
	unsigned char uc[2];

	int8_t i8[2];
	uint8_t ui8[2];
};

union num28b {
	double f;

	char c[8];
	unsigned char uc[8];

	int8_t i8[8];
	uint8_t ui8[8];
};

#endif

