#ifndef _PID_H
#define _PID_H

#include <stdint.h>

typedef struct {
	int16_t target;
	int16_t lastError;
	float sumError;
	float maxI;
	float minI;
	float k_p;
	float k_i;
	float k_d;
} PIDf32;


#define PIDf32_new(X, _P, _I, _D) PIDf32 X = { .k_p = _P, .k_i = _I, .k_d = _D, .target = 0, .lastError = 0, .sumError = 0,  .maxI = 10000.0f, .minI = -10000.0f }

void PIDf32_init(PIDf32 *_pid);
void PIDf32_set(PIDf32 *_pid, float Kp, float Ki, float Kd);
float PIDf32_PDctrl2(PIDf32 *_pid, int16_t pos, int16_t dpos);
float PIDf32_ctrlT(PIDf32 *_pid, int16_t pos, uint32_t dt);
float PIDf32_PDctrl(PIDf32 *_pid, int16_t pos);
float PIDf32_PDctrlDF(PIDf32 *_pid, int16_t pos);


typedef struct {
	int16_t lastError;
	int16_t target;
	int32_t   K[3];
} PIDi32;


#endif
