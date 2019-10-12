#ifndef _IMU_H_
#define _IMU_H_

#include "mpu6050.h"
#include "math.h"

#define RtA 	   57.324841  //弧度到角度
#define AtR    	   0.0174533  //度到角度
#define Acc_Factor 	   0.0011963  //加速度变成G
#define Gyro_Factor 	   0.0152672  //角速度变成度
#define Gyro_Gr	   0.00028    //0.0002663	//角速度变成弧度速度
#define FILTER_NUM 20

extern int   ACC_AVG_X,ACC_AVG_Y,ACC_AVG_Z;      //平均值滤波后的ACC
extern float GYRO_I_X,GYRO_I_Y,GYRO_I_Z;         //陀螺仪积分
extern float EXP_ANGLE_X,EXP_ANGLE_Y,EXP_ANGLE_Z;//期望角度
extern float DIF_ANGLE_X,DIF_ANGLE_Y,DIF_ANGLE_Z;//期望角度和实际角度的差

typedef struct T_INT16_XYZ
{
	int16 X;
	int16 Y;
	int16 Z;
}T_INT16_XYZ;


T_INT16_XYZ		GYRO_OFFSET,ACC_OFFSET;			//零漂

/*extern int16 MPU6050_ACC_X ;
extern int16 MPU6050_ACC_Y ;
extern int16 MPU6050_ACC_Z ;
extern int16 MPU6050_GYRO_X;
extern int16 MPU6050_GYRO_Y;
extern int16 MPU6050_GYRO_Z;*/

//原始的加速度测量值
extern int16 ACC_X;
extern int16 ACC_Y;
extern int16 ACC_Z;
//原始陀螺仪测量值
extern int16 GYRO_X ;
extern int16 GYRO_Y ;
extern int16 GYRO_Z ;

//滤波过后的加速度值
extern int16 ACC_X_Filted;
extern int16 ACC_Y_Filted;
extern int16 ACC_Z_Filted;



extern float Q_ANGLE_X,Q_ANGLE_Y,Q_ANGLE_Z;

void Prepare_Data(void);
void Get_Attitude(void);
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);
void null_shift();//零漂处理

//void Data_Filter(void);

#endif
