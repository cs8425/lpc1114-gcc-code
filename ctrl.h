#ifndef _CTRL_H
#define _CTRL_H

#include "PID/PID.h"
#include "filter.h"
#include "tools.h"
#include "hal.h"

#include "ringbuffer.h"


#define centerA 0
#define centerB 0

// FB = 1500
// S_PID = 5.0,0.0,8.5
// slow_PID = 12.0, 0.0, 18.0
// slowlimit = 768

// 前輪轉向的PID參數
PIDf32_new(servo, 8.0, 0.0, 8.5);
//PIDf32_new(servo, 8.0, 0.0, 4.5);

// 後輪差速的PID
//PIDf32_new(differential, 3.0f, 0.4f, 4.0f);
//PIDf32_new(differential, 1.5f, 0.2f, 2.6f);
PIDf32_new(differential, 5.2f, 4.6f, 5.6f);


// 轉彎減速
PIDf32_new(slowdown, 18.0f, 0.0f, 18.0f);

// 低通
Ringbuff_new(prefilt);

uint8_t run_mode = 0;

float debug_f = 0.0;
int16_t debug_int1 = 0;
int16_t debug_int2 = 0;
int16_t debug_int3 = 0;

int16_t M_FB = 0;
int16_t M_DF = 0;
int16_t M_SV = 3000;

int32_t emuspeed = 0;

void motor(int16_t B, int16_t A);
void toPWM(int16_t _FB, int16_t _DF, int16_t _SV);

uint16_t isStart = 0;

// Modes
#define Init_Mode        0x00
#define OK_Mode          0x01
#define TEST2_Mode       0x02
#define TEST3_Mode       0x03

#define RUN_INIT    0
#define RUN_R_out   1
#define RUN_L_out   2
#define RUN_all_out 3
uint8_t flag = 0;

void toCtrl(void) {

	int16_t out;
	int16_t dfout;
	int16_t sout;


	int16_t diff = 0;
	int16_t ddiff = 0;
	static int16_t dfout_1 = 0;

	int16_t diffpre = 0;

	// 預處理, 避免奇怪資料進入PID

	int16_t L = -1;
	int16_t R = -1;

	switch(flag){
		case RUN_all_out:
		case RUN_INIT:
			L = edge_get_R2L_t(&edgeL2H, 0, 52, -1);
			R = edge_get_L2R_t(&edgeL2H, 0, 51, 1);
			break;
		case RUN_R_out:
			//L = edgeL2H.get_R2L(0, 104, -1);
			L = edge_get_R2L_t(&edgeL2H, 0, 104, -1);
			if((L < 52) && (L != -1)){
				//R = edgeL2H.get_L2R(0, 51, 1);
				R = edge_get_L2R_t(&edgeL2H, 0, 51, 1);
			}
			break;
		case RUN_L_out:
			//R = edgeL2H.get_L2R(0, 2, 1);
			R = edge_get_L2R_t(&edgeL2H, 0, 2, 1);
			if((R > 51) && (R != -1)){
				//L = edgeL2H.get_R2L(0, 52, -1);
				L = edge_get_R2L_t(&edgeL2H, 0, 52, -1);
			}
			break;
	}


	if(R == -1){
		flag |= RUN_R_out;
	}else{
		flag &= ~RUN_R_out;
	}
	if(L == -1){
		flag |= RUN_L_out;
	}else{
		flag &= ~RUN_L_out;
	}

	switch(flag){
		case RUN_INIT:
			diff = (L + R) - 106;
			break;
		case RUN_R_out:
			R = 153;
			diff = L * 2;
			break;
		case RUN_L_out:
			L = -51;
			diff = (R - 106) * 2;
			break;
		case RUN_all_out:
			L = 53;
			R = 54;
			diff = (L + R) - 106;
			break;
	}

	Ringbuff_push(&prefilt, diff);
	diffpre = Ringbuff_filt(&prefilt);

	ddiff = diff - dfout_1;
	dfout_1 = diff;

	debug_int1 = diff;
	debug_int2 = ddiff;

	// 計算前輪轉向的控制量
	// debug_f = servo.ctrl2(L, R, pL.D, pR.D);
	out = PIDf32_PDctrl(&servo, diffpre);
//	degOfFire(&Af, (diff * 15), (ddiff / 8));
//	out = deFuzzication(&Af) / 32;

//	out = 3000 - out + 110;
	out = 3000 - out;
	if(out > 3700) out = 3700;
	if(out < 2300) out = 2300;
	M_SV = out;


	// 計算後輪差速的控制量
//	dfout = PIDf32_PDctrlDF(&differential, diff);
	degOfFire(&Af, (diff * 1), (ddiff * 1.5));
	dfout = deFuzzication(&Af) / 6;//if out small then 
	// dfout = differential.ctrl2(L, R, pL.D, pR.D);
//	dfout = - dfout;

	if(dfout >  4800) dfout = 4800;
	if(dfout < -4800) dfout = -4800;
	M_DF = -dfout;

	debug_int3 = emuspeed;

	// 計算後輪減速的控制量
	sout = PIDf32_PDctrlSD(&slowdown, diffpre);

	if(sout >  684) sout = 684;
	if(sout < -1024) sout = -1024;
	M_FB = M_FB - ((sout * emuspeed) >> 10);

	toPWM(M_FB, M_DF, M_SV);
}

// 輸入算完的"速度"、"差速"、"轉向"控制量
void toPWM(int16_t _FB, int16_t _DF, int16_t _SV) {

	TFC_SetServo(_SV);
	motor( -_FB, -_DF);

}


// 把2個數值換算為真正的PWM
// arg1 = ^V
// arg2 = <>
void motor(int16_t B, int16_t A){
	int16_t thA = A;
	int16_t thB = B;
	int16_t chA = centerA;
	int16_t chB = centerB;

	int32_t tmpA;
	int32_t tmpB;

	if(isStart == 0) return;

	chA = centerA - (thA) + (thB);
	chB = centerB + (thA) + (thB);

	if(chA > 4800){
		chB += chA - 4800;
		chA = 4800;
	}
	if(chA < -4800){
		chA = -4800;
		chB += chA + 4800;
	}
	if(chB > 4800){
		chA += chB - 4800;
		chB = 4800;
	}
	if(chB < -4800){
		chB = -4800;
		chA += chB + 4800;
	}

	tmpA = -chA;
	tmpB = -chB;

	emuspeed = ( (emuspeed * 15) + ((int32_t)((tmpA + tmpB) / 2) * 1) ) >> 4;

	TFC_SetMotorPWM( chA, chB);
	//TFC_HBRIDGE_ENABLE;

}


#endif
