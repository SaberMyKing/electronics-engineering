#include <stdint.h>
#include <stdbool.h>
#include "MPU6050.h"

uint8						mpu6050_buffer[14];					//iic读取后存放数据
uint8						GYRO_OFFSET_OK = 1;
uint8						ACC_OFFSET_OK = 1;
typedef struct T_INT16_XYZ
{
	int16 X;
	int16 Y;
	int16 Z;
}T_INT16_XYZ;



T_INT16_XYZ		MPU6050_ACC_LAST,MPU6050_GYRO_LAST;		//最新一次读取值

//原始的加速度测量值






unsigned char shuju[5] = {0x00,0x07,0x06,0x18,0x09};

 /*
void Delay_ms_mpu(u16 nms)
{
	uint8 delay_cnt = TIM3_IRQCNT;
	while((delay_cnt+(nms*2)) > TIM3_IRQCNT);
}   */
/**************************实现函数********************************************
//将iic读取到得数据分拆,放入相应寄存器
*******************************************************************************/
void MPU6050_Dataanl(void)
{

/*	MPU6050_ACC_LAST.X=acce_x - ACC_OFFSET.X;
	MPU6050_ACC_LAST.Y=acce_y - ACC_OFFSET.Y;
	MPU6050_ACC_LAST.Z=acce_z - ACC_OFFSET.Z;
	//跳过温度ADC
	MPU6050_GYRO_LAST.X=gyro_x - GYRO_OFFSET.X;
	MPU6050_GYRO_LAST.Y=gyro_y - GYRO_OFFSET.Y;
	MPU6050_GYRO_LAST.Z=gyro_z - GYRO_OFFSET.Z;

	if(!GYRO_OFFSET_OK)
	{
		static int32	tempgx=0,tempgy=0,tempgz=0;
		static uint8 cnt_g=0;
// 		LED1_ON;
		if(cnt_g==0)
		{
			GYRO_OFFSET.X=0;
			GYRO_OFFSET.Y=0;
			GYRO_OFFSET.Z=0;
			tempgx = 0;
			tempgy = 0;
			tempgz = 0;
			cnt_g = 1;
			return;
		}
		tempgx+= gyro_x;
		tempgy+= gyro_y;
		tempgz+= gyro_z;
		if(cnt_g==200)
		{
			GYRO_OFFSET.X=tempgx/cnt_g;
			GYRO_OFFSET.Y=tempgy/cnt_g;
			GYRO_OFFSET.Z=tempgz/cnt_g;
			cnt_g = 0;
			GYRO_OFFSET_OK = 1;
//			EE_SAVE_GYRO_OFFSET();//保存数据
			return;
		}
		cnt_g++;
	}
	if(!ACC_OFFSET_OK)
	{
		static int32	tempax=0,tempay=0,tempaz=0;
		static uint8 cnt_a=0;
// 		LED1_ON;
		if(cnt_a==0)
		{
			ACC_OFFSET.X = 0;
			ACC_OFFSET.Y = 0;
			ACC_OFFSET.Z = 0;
			tempax = 0;
			tempay = 0;
			tempaz = 0;
			cnt_a = 1;
			return;
		}
		tempax+= acce_x;
		tempay+= acce_y;
		//tempaz+= acce_z;
		if(cnt_a==200)
		{
			ACC_OFFSET.X=tempax/cnt_a;
			ACC_OFFSET.Y=tempay/cnt_a;
			ACC_OFFSET.Z=tempaz/cnt_a;
			cnt_a = 0;
			ACC_OFFSET_OK = 1;
//			EE_SAVE_ACC_OFFSET();//保存数据
			return;
		}
		cnt_a++;
	}*/
}
/**************************实现函数********************************************
//将iic读取到得数据分拆,放入相应寄存器,更新MPU6050_Last
*******************************************************************************/
void MPU6050_Init()
{
I2CWrite(shuju+0,PWR_MGMT_1,1);
I2CWrite(shuju+1,SMPLRT_DIV,1);
I2CWrite(shuju+2,CONFIG,1);
I2CWrite(shuju+3,GYRO_CONFIG,1);
I2CWrite(shuju+4,ACCEL_CONFIG,1);
}
void Soft_I2C_Init()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	GPIODirModeSet(SCL_BASE,0X0C,0X01);
	GPIOPadConfigSet(SCL_BASE,0X0C,GPIO_STRENGTH_4MA,GPIO_PIN_TYPE_OD);
	GPIOPinWrite(SDA_BASE,0X0C,0X0C);
	MPU6050_Init();
}

//void delay1_3us()
//{
//	int i=6;
//	while(i)
//		i--;
//}
void delay0_6us()
{	int i=3;
	while(i)
		i--;
}
void delay100us()
{
	int i=8;
	while(i)
	i--;
}
void i2cmasterdataput(unsigned char data)
{
  int i;
	for(i=8;i>0;i--){
	GPIOPinWrite(SCL_BASE,SCL_PIN,0);
	delay100us();
	GPIOPinWrite(SDA_BASE,SDA_PIN,(data>>(i-1))<<3);
        delay100us();
	GPIOPinWrite(SCL_BASE,SCL_PIN,SCL_PIN);
	delay100us();
	}
}
//i2cwrite() return 1 when it writes data success
int I2CWrite(unsigned char *putdata,unsigned long adr,long num)
{
	GPIOPinWrite(SDA_BASE,SDA_PIN,0);
	delay100us();
	i2cmasterdataput(SlaveAddress);
	GPIOPinWrite(SCL_BASE,SCL_PIN,0);
        delay100us();
	GPIOPinWrite(SDA_BASE,SDA_PIN,SDA_PIN);
	delay100us();
	GPIOPinWrite(SCL_BASE,SCL_PIN,SCL_PIN);
	GPIODirModeSet(SDA_BASE,SDA_PIN,0);
	delay100us();
	if(GPIOPinRead(SDA_BASE,SDA_PIN)==1)  return(0);
	GPIODirModeSet(SDA_BASE,SDA_PIN,1);
	GPIOPinWrite(SDA_BASE,SDA_PIN,0);
	delay100us();
	i2cmasterdataput(adr);
	GPIOPinWrite(SCL_BASE,SCL_PIN,0);
        delay100us();
	GPIOPinWrite(SDA_BASE,SDA_PIN,SDA_PIN);
	delay100us();
	GPIOPinWrite(SCL_BASE,SCL_PIN,SCL_PIN);
	GPIODirModeSet(SDA_BASE,SDA_PIN,0);
	delay100us();
	if(GPIOPinRead(SDA_BASE,SDA_PIN)==1)  return(0);
	GPIODirModeSet(SDA_BASE,SDA_PIN,1);
	while(num!=0)
	{
		i2cmasterdataput(*putdata);
		putdata++;
		num--;
		GPIOPinWrite(SCL_BASE,SCL_PIN,0);
		delay100us();
		GPIOPinWrite(SDA_BASE,SDA_PIN,SDA_PIN);
                delay100us();
		GPIOPinWrite(SCL_BASE,SCL_PIN,SCL_PIN);
		GPIODirModeSet(SDA_BASE,SDA_PIN,0);
		delay100us();
		if(GPIOPinRead(SDA_BASE,SDA_PIN)==1)  return(0);
		GPIODirModeSet(SDA_BASE,SDA_PIN,1);
  }
		GPIOPinWrite(SCL_BASE,SCL_PIN,0);
		delay100us();
	  GPIOPinWrite(SDA_BASE,SDA_PIN,0);
	  delay100us();
		GPIOPinWrite(SCL_BASE,SCL_PIN,SCL_PIN);
	  delay100us();
	  GPIOPinWrite(SDA_BASE,SDA_PIN,SDA_PIN);
	  delay100us();
	return (1);
}
void i2cmasterdataget(unsigned char *getdata)
{
	int i;
	*getdata=0;
	GPIOPinWrite(SCL_BASE,SCL_PIN,0);
	delay100us();
	for(i=8;i>0;i--){
	GPIODirModeSet(SDA_BASE,SDA_PIN,1);
	GPIOPinWrite(SDA_BASE,SDA_PIN,SDA_PIN);
	delay100us();
	GPIODirModeSet(SDA_BASE,SDA_PIN,0);
	GPIOPinWrite(SCL_BASE,SCL_PIN,SCL_PIN);
	delay100us();
	*getdata=(*getdata)*2+(GPIOPinRead(SDA_BASE,SDA_PIN)>>3);
	GPIOPinWrite(SCL_BASE,SCL_PIN,0);
	delay100us();
	}
	GPIODirModeSet(SDA_BASE,SDA_PIN,1);
	GPIOPinWrite(SDA_BASE,SDA_PIN,0);
	delay100us();
	GPIOPinWrite(SCL_BASE,SCL_PIN,SCL_PIN);
	delay100us();
}
//  I2CRead() return 1 when it gets data success
int I2CRead(unsigned char *getdata,unsigned long adr,long num)
{
	GPIOPinWrite(SDA_BASE,SDA_PIN,0);
	delay100us();
	i2cmasterdataput(SlaveAddress);
	GPIOPinWrite(SCL_BASE,SCL_PIN,0);
        delay100us();
	GPIOPinWrite(SDA_BASE,SDA_PIN,SDA_PIN);
	delay100us();
	GPIOPinWrite(SCL_BASE,SCL_PIN,SCL_PIN);
	GPIODirModeSet(SDA_BASE,SDA_PIN,0);
	delay100us();
	if(GPIOPinRead(SDA_BASE,SDA_PIN)==1)  return(0);
	GPIODirModeSet(SDA_BASE,SDA_PIN,1);
	i2cmasterdataput(adr);
	GPIOPinWrite(SCL_BASE,SCL_PIN,0);
        delay100us();
	GPIOPinWrite(SDA_BASE,SDA_PIN,SDA_PIN);
	delay100us();
	GPIOPinWrite(SCL_BASE,SCL_PIN,SCL_PIN);
	GPIODirModeSet(SDA_BASE,SDA_PIN,0);
	delay100us();
	if(GPIOPinRead(SDA_BASE,SDA_PIN)==1)  return(0);
	GPIODirModeSet(SDA_BASE,SDA_PIN,1);
	GPIOPinWrite(SCL_BASE,SCL_PIN,0);
	delay100us();
	GPIOPinWrite(SDA_BASE,SDA_PIN,SDA_PIN);
	delay100us();
	GPIOPinWrite(SCL_BASE,SCL_PIN,SCL_PIN);
	delay100us();
	GPIOPinWrite(SDA_BASE,SDA_PIN,0);
	delay100us();
	i2cmasterdataput(SlaveAddress+1);
	GPIOPinWrite(SCL_BASE,SCL_PIN,0);
        delay100us();
	GPIOPinWrite(SDA_BASE,SDA_PIN,SDA_PIN);
	delay100us();
	GPIOPinWrite(SCL_BASE,SCL_PIN,SCL_PIN);
	GPIODirModeSet(SDA_BASE,SDA_PIN,0);
	delay100us();
	if(GPIOPinRead(SDA_BASE,SDA_PIN)==1)  return(0);
	while(num!=0)
	{
		i2cmasterdataget(getdata);
		getdata++;
		num--;
  }
	GPIODirModeSet(SDA_BASE,SDA_PIN,1);
	GPIOPinWrite(SDA_BASE,SDA_PIN,SDA_PIN);
	delay100us();
	return (1);
}

//**************************************
//合成数据
//**************************************
int16 MPU6050_GetData()
{
	unsigned char H[10],L[10];
	int16 tmp=0;
	I2CRead(H,ACCEL_XOUT_H,1);
	I2CRead(L,ACCEL_XOUT_H+1,1);
	ACC_X=(H[0]<<8)+L[0];

	I2CRead(H+1,ACCEL_YOUT_H,1);
	I2CRead(L+1,ACCEL_YOUT_H+1,1);
	ACC_Y=(H[1]<<8)+L[1];

	I2CRead(H+2,ACCEL_ZOUT_H,1);
	I2CRead(L+2,ACCEL_ZOUT_H+1,1);
	ACC_Z=(H[2]<<8)+L[2];

	I2CRead(H+3,GYRO_XOUT_H,1);
	I2CRead(L+3,GYRO_XOUT_H+1,1);
	GYRO_X = (H[3]<<8)+L[3];

	I2CRead(H+4,GYRO_YOUT_H,1);
	I2CRead(L+4,GYRO_YOUT_H+1,1);
	GYRO_Y = (H[4]<<8)+L[4];

	I2CRead(H+5,GYRO_ZOUT_H,1);
	I2CRead(L+5,GYRO_ZOUT_H+1,1);
	GYRO_Z = (H[5]<<8)+L[5];

     return 0;
}
