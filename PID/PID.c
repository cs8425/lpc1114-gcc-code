#include "PID.h"

void PIDf32_init(PIDf32 *_pid) {
	_pid->lastError = 0;
	_pid->target = 0;
	_pid->sumError = 0.0f;
	_pid->maxI = 10000.0f;
	_pid->minI = -10000.0f;
	PIDf32_set(_pid, 1.0f, 0.0f, 0.0f);
}

void PIDf32_set(PIDf32 *_pid, float Kp, float Ki, float Kd){
	_pid->k_p = Kp;
	_pid->k_d = Kd;
	_pid->k_i = Ki;
}

float PIDf32_PDctrl2(PIDf32 *_pid, int16_t pos, int16_t dpos) {
        int32_t error = _pid->target - pos;
        int32_t dError = dpos;
        _pid->lastError = error;

        float out = (_pid->k_p * error) + (_pid->k_d * dError);
        return out;
}

float PIDf32_ctrlT(PIDf32 *_pid, int16_t pos, uint32_t dt) {
	return PIDf32_PDctrl(_pid, pos);
}

float PIDf32_PDctrl(PIDf32 *_pid, int16_t pos) {
        int32_t error = pos - _pid->target;
        //_pid->sumError = _pid->sumError + error;
        //if(_pid->sumError < _pid->minI) _pid->sumError = _pid->minI;
        //if(_pid->sumError > _pid->maxI) _pid->sumError = _pid->maxI;

        int32_t dError = error - _pid->lastError;
        _pid->lastError = error;

        //float out = (_pid->k_p * error) + (_pid->k_i * _pid->sumError) + (_pid->k_d * dError);
        float out = (_pid->k_p * error) + (_pid->k_d * dError);

        return out;
}
float PIDf32_PDctrlDF(PIDf32 *_pid, int16_t pos) {
        int32_t error = pos - _pid->target;
        //
        //if(_pid->sumError < _pid->minI) _pid->sumError = _pid->minI;
        //if(_pid->sumError > _pid->maxI) _pid->sumError = _pid->maxI;

        int32_t dError = error - _pid->lastError;
        _pid->lastError = error;
        int32_t aError = dError - _pid->sumError;
	_pid->sumError = dError;

        //float out = (_pid->k_p * error) + (_pid->k_i * _pid->sumError) + (_pid->k_d * dError);
        float out = (_pid->k_p * error) + (_pid->k_d * dError) + (_pid->k_i * aError);

        return out;
}
float PIDf32_PDctrlSD(PIDf32 *_pid, int16_t pos) {
	int8_t dir = (pos > 0) ? 1 : -1;

	pos = (dir == _pid->sumError) ? pos : (pos * 2);
	_pid->sumError = dir;

	pos = (pos < 0) ? -pos : pos;

        int32_t error = pos - _pid->target;

        int32_t dError = error - _pid->lastError;
        _pid->lastError = error;

        //float out = (_pid->k_p * error) + (_pid->k_i * _pid->sumError) + (_pid->k_d * dError);
        float out = (_pid->k_p * error) + (_pid->k_d * dError);

        return out;
}

