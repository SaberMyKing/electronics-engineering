#ifndef __MPU6050_H_
#define __MPU6050_H_

#include <stdint.h>
#include <stdbool.h>
#include "sysctl.h"
#include "gpio.h"
#include "hw_memmap.h"
#include "common.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"

extern int16 ACC_X ;
extern int16 ACC_Y ;
extern int16 ACC_Z ;
//ԭʼ�������ǲ���ֵ
extern int16 GYRO_X ;
extern int16 GYRO_Y ;
extern int16 GYRO_Z ;

#define SCL_BASE  GPIO_PORTB_BASE
#define SCL_PIN   GPIO_PIN_2
#define SDA_BASE  GPIO_PORTB_BASE
#define SDA_PIN   GPIO_PIN_3


#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I			0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define	SlaveAddress	0xD0	//IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ




extern uint8							GYRO_OFFSET_OK;
extern uint8							ACC_OFFSET_OK;

void MPU6050_Init(void);
void MPU6050_Read(void);
void MPU6050_Dataanl(void);
int16 MPU6050_GetData();
int I2CRead(unsigned char *getdata,unsigned long adr,long num);
void i2cmasterdataget(unsigned char *getdata);
int I2CWrite(unsigned char *putdata,unsigned long adr,long num);
void i2cmasterdataput(unsigned char data);
void delay100us();
void delay0_6us();
void Soft_I2C_Init();

#endif
