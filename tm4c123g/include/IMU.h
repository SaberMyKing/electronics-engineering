#ifndef _IMU_H_
#define _IMU_H_

#include "mpu6050.h"
#include "math.h"

#define RtA 	   57.324841  //���ȵ��Ƕ�
#define AtR    	   0.0174533  //�ȵ��Ƕ�
#define Acc_Factor 	   0.0011963  //���ٶȱ��G
#define Gyro_Factor 	   0.0152672  //���ٶȱ�ɶ�
#define Gyro_Gr	   0.00028    //0.0002663	//���ٶȱ�ɻ����ٶ�
#define FILTER_NUM 20

extern int   ACC_AVG_X,ACC_AVG_Y,ACC_AVG_Z;      //ƽ��ֵ�˲����ACC
extern float GYRO_I_X,GYRO_I_Y,GYRO_I_Z;         //�����ǻ���
extern float EXP_ANGLE_X,EXP_ANGLE_Y,EXP_ANGLE_Z;//�����Ƕ�
extern float DIF_ANGLE_X,DIF_ANGLE_Y,DIF_ANGLE_Z;//�����ǶȺ�ʵ�ʽǶȵĲ�

typedef struct T_INT16_XYZ
{
	int16 X;
	int16 Y;
	int16 Z;
}T_INT16_XYZ;


T_INT16_XYZ		GYRO_OFFSET,ACC_OFFSET;			//��Ư

/*extern int16 MPU6050_ACC_X ;
extern int16 MPU6050_ACC_Y ;
extern int16 MPU6050_ACC_Z ;
extern int16 MPU6050_GYRO_X;
extern int16 MPU6050_GYRO_Y;
extern int16 MPU6050_GYRO_Z;*/

//ԭʼ�ļ��ٶȲ���ֵ
extern int16 ACC_X;
extern int16 ACC_Y;
extern int16 ACC_Z;
//ԭʼ�����ǲ���ֵ
extern int16 GYRO_X ;
extern int16 GYRO_Y ;
extern int16 GYRO_Z ;

//�˲�����ļ��ٶ�ֵ
extern int16 ACC_X_Filted;
extern int16 ACC_Y_Filted;
extern int16 ACC_Z_Filted;



extern float Q_ANGLE_X,Q_ANGLE_Y,Q_ANGLE_Z;

void Prepare_Data(void);
void Get_Attitude(void);
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);
void null_shift();//��Ư����

//void Data_Filter(void);

#endif
