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
//原始的陀螺仪测量值
extern int16 GYRO_X ;
extern int16 GYRO_Y ;
extern int16 GYRO_Z ;

#define SCL_BASE  GPIO_PORTB_BASE
#define SCL_PIN   GPIO_PIN_2
#define SDA_BASE  GPIO_PORTB_BASE
#define SDA_PIN   GPIO_PIN_3


#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
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
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I			0x75	//IIC地址寄存器(默认数值0x68，只读)
#define	SlaveAddress	0xD0	//IIC写入时的地址字节数据，+1为读取




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
