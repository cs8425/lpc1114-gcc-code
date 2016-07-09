#ifndef _FILTER_H
#define _FILTER_H

#include "tools.h"

// 攝影機要使用的px範圍  (去頭去尾)
#define TRIM_start 6
#define TRIM_end 122

// 最後輸出多少個px
// #define outlength 116-2*(winlength-1)
#define outlength 106

// filter的長度
// #define winlength 4
#define winlength 6

// filter 係數
// 越長   >> 對縫隙不明顯   (太長會連邊界線都濾掉)
// int8_t window[winlength]={-1,-1,1,1};
int8_t window[winlength]={-1, -1, -1, 1, 1, 1};

// 最後偵測邊緣用的閥值
#define threshold 1024 * 0.5


#define CAM0 px0

#define CAM_value_MAX 1024
#define CAM_value_MIN 0

uint16_t px0[128];

int16_t filt_out[outlength]={0};

int16_t MA_out[outlength];
uint16_t MA_out_ABS[outlength];

int8_t bin_out[outlength];

int16_t Debug_out[outlength];

uint16_t maximum;
uint16_t minimum;

uint32_t cam_total;


// 存放邊界
edge edgeL2H;
edge edgeH2L;

// filter實作, 降低亮度影響
// low  >> high : +
// high >>  low : -
void filt(void){

	uint8_t i,j;
	int32_t temp;

	cam_total = 0;

	for(i=TRIM_start+winlength;i<outlength+winlength+TRIM_start;i++) {
		cam_total += CAM0[i];

		filt_out[i-TRIM_start-winlength] = 0;
		temp =  0;
		for(j=0;j<winlength;j++){
			temp += CAM0[i-j]*window[j];
		}
		//filt_out[i-TRIM_start-winlength] = ABS16(temp);
		filt_out[i-TRIM_start-winlength] = temp;
	}
}

// 低通, 讓數值比較平緩
void MA(void){
	uint8_t i;
	maximum = CAM_value_MIN;
	minimum = CAM_value_MAX;
	//for(i=1;i<outlength-1;i++) {
	for(i=2;i<outlength-2;i++) {
		MA_out[i] = filt_out[i-2] + (filt_out[i-1] << 1) + (filt_out[i] << 1) + (filt_out[i+1] << 1) + filt_out[i+2];
		MA_out[i] >>= 3;
		//MA_out[i] = filt_out[i];

		//MA_out[i] /= 4;
		MA_out_ABS[i] = ABS16(MA_out[i]);

		if(maximum < MA_out_ABS[i]){
			maximum = MA_out_ABS[i];
		}
		if(minimum > MA_out_ABS[i]){
			minimum = MA_out_ABS[i];
		}
	}

//	MA_out[0] = minimum;
//	MA_out[1] = minimum;
//	MA_out[outlength-2] = minimum;
//	MA_out[outlength-1] = minimum;

	MA_out_ABS[0] = minimum;
	MA_out_ABS[1] = minimum;
	MA_out_ABS[outlength-2] = minimum;
	MA_out_ABS[outlength-1] = minimum;
}

// 標準化, 最大值一律拉到CAM_value_MAX, 避免閥值需要一直改動
void scale(void){
	// uint16_t scalefector = (4095.0 / (maximumL-minimumL)) + 0.5;
	uint16_t scalefector = (((CAM_value_MAX*2 - 1) / (maximum-minimum)) + 1) >> 1;
//	uint16_t scalefector = ((((CAM_value_MAX/2) - 1) / (maximum-minimum)) + 1) >> 1;
	uint8_t i;

	edge_reset(&edgeL2H);
	edge_reset(&edgeH2L);

	// 簡單判斷這筆資料能不能用(原數值都太低 >> 可能太暗抓不到任何東西)
	// if(scalefector > 4){
	if(scalefector > 5){
		edge_setNothing(&edgeL2H);
		edge_setNothing(&edgeH2L);
	}else{
		for(i=0;i<outlength;i++){
			// filt_out[i]=(filt_out[i] - minimum) * scalefector;
			MA_out_ABS[i] = (MA_out_ABS[i] - minimum) * scalefector;
			if(MA_out[i] > 0){
				MA_out[i] = (MA_out[i] - minimum) * scalefector;
			}else{
				MA_out[i] = (MA_out[i] + minimum) * scalefector;
			}
		}
	}
}

// 2值化, 並紀錄邊界的座標(在第幾個px)
void cut(void){
	uint8_t i;
	int8_t status;

	//MA_out_ABS[0] = status = (MA_out_ABS[0] > threshold) ? 1 : 0;

	if(MA_out_ABS[0] > threshold){
		//MA_out_ABS[0] = 1;
		if(MA_out[0] > 0){
			status = bin_out[0] = 1;
			Debug_out[0] = 1;
		}else{
			status = bin_out[0] = -1;
			Debug_out[0] = -1;
		}
	}else{
		//MA_out_ABS[0] = 0;
		status = bin_out[0] = 0;
		Debug_out[0] = 0;
	}

	for(i=1;i<outlength;i++){
		if(MA_out_ABS[i] > threshold){
			// MA_out_ABS[i] = 1;
			if(MA_out[i] > 0){
				Debug_out[i] = 1;
				bin_out[i] = 1;
			}else{
				Debug_out[i] = -1;
				bin_out[i] = -1;
			}
		}else{
			// MA_out_ABS[i] = 0;
			Debug_out[i] = 0;
			bin_out[i] = 0;
		}
		if(bin_out[i] != status){
			if(bin_out[i] > status){
				edge_pushT(&edgeL2H, i, bin_out[i]);
			}else{
				edge_pushT(&edgeH2L, i, bin_out[i]);
			}
		}else{
			
		}
		status = bin_out[i];
	}
}

#endif

