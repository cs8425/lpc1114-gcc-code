// accel.h
// API definition for the MPU6050 accelerometer/gyro on the I2C bus
#include <stdint.h>
typedef struct {
	int16_t x_a,y_a,z_a;
	int16_t x_g,y_g,z_g;
} motion;
int readRegister(int RegNum);
int WriteRegister(int RegNum, int Value);
int initAccel(void);
int getMotion(motion *m);
void powerUpAccel(void);
int getTemperature(void);
// MPU6050 default slave address
#define MPU_6050_ADDRESS (0x68 << 1)
// Register definitions for the MPU6050
#define	SELF_TEST_X		13
#define	SELF_TEST_Y		14
#define	SELF_TEST_Z		15
#define	SELF_TEST_A		16
#define	SMPLRT_DIV		25
#define	CONFIG			26
#define	GYRO_CONFIG		27
#define	ACCEL_CONFIG	28
#define	MOT_THR			31
#define	FIFO_EN			35
#define	I2C_MST_CTRL	36
#define	I2C_SLV0_ADDR	37
#define	I2C_SLV0_REG	38
#define	I2C_SLV0_CTRL	39
#define	I2C_SLV1_ADDR	40
#define	I2C_SLV1_REG	41
#define	I2C_SLV1_CTRL	42
#define	I2C_SLV2_ADDR	43
#define	I2C_SLV2_REG	44
#define	I2C_SLV2_CTRL	45
#define	I2C_SLV3_ADDR	46
#define	I2C_SLV3_REG	47
#define	I2C_SLV3_CTRL	48
#define	I2C_SLV4_ADDR	49
#define	I2C_SLV4_REG	50
#define	I2C_SLV4_DO		51
#define	I2C_SLV4_CTRL	52
#define	I2C_SLV4_DI		53
#define	I2C_MST_STATUS	54
#define	INT_PIN_CFG		55
#define	INT_ENABLE		56
#define	INT_STATUS		58
#define	ACCEL_XOUT_H	59
#define	ACCEL_XOUT_L	60
#define	ACCEL_YOUT_H	61
#define	ACCEL_YOUT_L	62
#define	ACCEL_ZOUT_H	63
#define	ACCEL_ZOUT_L	64
#define	TEMP_OUT_H		65
#define	TEMP_OUT_L		66
#define	GYRO_XOUT_H		67
#define	GYRO_XOUT_L		68
#define	GYRO_YOUT_H		69
#define	GYRO_YOUT_L		70
#define	GYRO_ZOUT_H		71
#define	GYRO_ZOUT_L		72
#define	EXT_SENS_DATA_00	73
#define	EXT_SENS_DATA_01	74
#define	EXT_SENS_DATA_02	75
#define	EXT_SENS_DATA_03	76
#define	EXT_SENS_DATA_04	77
#define	EXT_SENS_DATA_05	78
#define	EXT_SENS_DATA_06	79
#define	EXT_SENS_DATA_07	80
#define	EXT_SENS_DATA_08	81
#define	EXT_SENS_DATA_09	82
#define	EXT_SENS_DATA_10	83
#define	EXT_SENS_DATA_11	84
#define	EXT_SENS_DATA_12	85
#define	EXT_SENS_DATA_13	86
#define	EXT_SENS_DATA_14	87
#define	EXT_SENS_DATA_15	88
#define	EXT_SENS_DATA_16	89
#define	EXT_SENS_DATA_17	90
#define	EXT_SENS_DATA_18	91
#define	EXT_SENS_DATA_19	92
#define	EXT_SENS_DATA_20	93
#define	EXT_SENS_DATA_21	94
#define	EXT_SENS_DATA_22	95
#define	EXT_SENS_DATA_23	96
#define	I2C_SLV0_DO			99
#define	I2C_SLV1_DO			100
#define	I2C_SLV2_DO			101
#define	I2C_SLV3_DO			102
#define	I2C_MST_DELAY_CT_RL	103
#define	SIGNAL_PATH_RES_ET	104
#define	MOT_DETECT_CTRL		105
#define	USER_CTRL			106
#define	PWR_MGMT_1			107
#define	PWR_MGMT_2			108
#define	FIFO_COUNTH			114
#define	FIFO_COUNTL			115
#define	FIFO_R_W			116
#define	WHO_AM_I			117
