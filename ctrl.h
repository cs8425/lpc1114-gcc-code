#ifndef _CTRL_H
#define _CTRL_H

#include "PID/PID.h"
// #include "fuzzy.h"
#include "filter.h"
#include "tools.h"
#include "hal.h"


#define centerA 0
#define centerB 0


// 前輪轉向的PID參數
PIDf32_new(servo, 10.0, 0.0, 4.5);
//PIDf32_new(servo, 8.0, 0.0, 4.5);

// 後輪差速的PID
//PIDf32_new(differential, 3.0f, 0.4f, 4.0f);
//PIDf32_new(differential, 1.5f, 0.2f, 2.6f);
PIDf32_new(differential, 1.6f, 2.6f, 2.6f);


uint8_t run_mode = 0;
uint8_t show_mode = 0;

uint8_t force_stop = 0;

float debug_f = 0.0;
int16_t debug_int = 0;
int16_t debug_int2 = 0;
int16_t debug_int3 = 0;

int16_t M_FB = 0;
int16_t M_DF = 0;
int16_t M_SV = 3000;

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

	int16_t diff = 0;

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



	debug_int = L;
	debug_int2 = R;

	debug_int3 = diff;

	// 計算前輪轉向的控制量
	// debug_f = servo.ctrl2(L, R, pL.D, pR.D);
	out = PIDf32_PDctrl(&servo, diff);

	out = 3000 - out;
	if(out > 3700) out = 3700;
	if(out < 2300) out = 2300;
	M_SV = out;


	// 計算後輪差速的控制量
	dfout = PIDf32_PDctrlDF(&differential, diff);
	// dfout = differential.ctrl2(L, R, pL.D, pR.D);
//	dfout = - dfout;

	if(dfout > 4800) dfout = 4800;
	if(dfout < -4800) dfout = -4800;
	M_DF = -dfout;

	toPWM(M_FB, M_DF, M_SV);
}

//TODO: 清理&最佳化
// 輸入算完的"速度"、"差速"、"轉向"控制量
void toPWM(int16_t _FB, int16_t _DF, int16_t _SV) {

	TFC_SetServo(_SV);
	motor( -_FB, -_DF);
/*
    switch( run_mode ) {
        case Init_Mode:
//            TFC_SetMotorPWM(centerA, centerB);
//            TFC_HBRIDGE_DISABLE;
//            TFC_SetServo(_SV, _SV);
            break;
//        case RC_speed_Mode:
//            // TFC_HBRIDGE_ENABLE;
//            TFC_SetServo(0, SV);
//            if(force_stop == 0) motor(ch1, DF);
//            break;
//        case RC_Mode:
//            // TFC_HBRIDGE_ENABLE;
//            // TFC_SetServo2(0, ch3);
//            TFC_SetServo(0, ch3);
//            // TFC_SetServo(0, SV);
//            if(force_stop == 0) motor(ch1, ch2);
//            break;


        // 比賽實用的速度清單
        case OK_Mode:
            TFC_SetServo(_SV);
            //motor(pot1.read_u16() >> 6, _DF);
            break;

        case TEST2_Mode:
            TFC_SetServo(_SV);
            motor(205, _DF);
            break;

        case TEST3_Mode:
            TFC_SetServo(_SV);
            motor(240, _DF);
            break;

        default:
            ;
    }
*/
}



// 把2個數值換算為真正的PWM
// arg1 = ^V
// arg2 = <>
void motor(int16_t B, int16_t A){
	int16_t thA = A;
	int16_t thB = B;
	int16_t chA = centerA;
	int16_t chB = centerB;


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

	TFC_SetMotorPWM( chA, chB);
	//TFC_HBRIDGE_ENABLE;

}


#endif
