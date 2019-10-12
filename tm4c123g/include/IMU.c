/*
 * This file contains some IMU operation.
 * By IC���� (1394024051@qq.com)
 * 2014-4-29 v1.0
 */
#include "IMU.h"

extern float Q_ANGLE_X,Q_ANGLE_Y,Q_ANGLE_Z;      //��Ԫ��������ĽǶ�

int ACC_X_BUF[FILTER_NUM],ACC_Y_BUF[FILTER_NUM],ACC_Z_BUF[FILTER_NUM];	//���ٶȻ��������˲�����
/*
void Data_Filter(void)//��ֵ�˲�
{
	
	uint8 i = 0;
	float Max_ACC_X = -65535 ;
	float Min_ACC_X =  65535 ;
	float Max_ACC_Y = -65535 ;
	float Min_ACC_Y =  65535 ;
	float Max_ACC_Z = -65535 ;
	float Min_ACC_Z =  65535 ;
	
	float Sum_ACC_X = 0;
	float Sum_ACC_Y = 0;
	float Sum_ACC_Z = 0;
	
	for( i = 0; i < 5 ;i++ )
	{
		//���
		Sum_ACC_X = ACC_X[i];
		Sum_ACC_Y = ACC_Y[i];
		Sum_ACC_Z = ACC_Z[i];
		
		//�����ֵ
		Max_ACC_X = (Max_ACC_X > ACC_X[i]) ? Max_ACC_X : ACC_X[i] ;
		Max_ACC_Y = (Max_ACC_Y > ACC_Y[i]) ? Max_ACC_Y : ACC_Y[i] ;
		Max_ACC_Z = (Max_ACC_Z > ACC_Z[i]) ? Max_ACC_Z : ACC_Z[i] ;
		
		//����Сֵ
		Min_ACC_X = (Min_ACC_X < ACC_X[i]) ? Min_ACC_X : ACC_X[i] ;
		Min_ACC_Y = (Min_ACC_Y < ACC_Y[i]) ? Min_ACC_Y : ACC_Y[i] ;
		Min_ACC_Z = (Min_ACC_Z < ACC_Z[i]) ? Min_ACC_Z : ACC_Z[i] ;
	}
	
	//ȥ����Сֵ�����ֵ
	ACC_X_Filted = (Sum_ACC_X - Max_ACC_X - Min_ACC_X) / 3;
	ACC_Y_Filted = (Sum_ACC_Y - Max_ACC_Y - Min_ACC_Y) / 3;
	ACC_Z_Filted = (Sum_ACC_Z - Max_ACC_Z - Min_ACC_Z) / 3;
}*/

/**********************************************************/
//�������ƣ�Prepare_Data
//��ڲ�������
//���ڲ�������
//�������ܣ���ȡMPU6050���ݽ���ƽ���˲���Ϊ��������׼������
/**********************************************************/
void Prepare_Data(void)
{
  static unsigned char filter_cnt=0;
  long int temp1=0,temp2=0,temp3=0;
  unsigned char i;
	
  //MPU6050_Dataanl();//��ɴ��������ݵĶ�ȡ�ͼ��㣬���Ҷ����ݼ򵥴���
	
	ACC_X_BUF[filter_cnt] = ACC_X;//���»�����������
	ACC_Y_BUF[filter_cnt] = ACC_Y;
	ACC_Z_BUF[filter_cnt] = ACC_Z;
	
	
	
  
  for(i=0;i<FILTER_NUM;i++)
  {
    temp1 += ACC_X_BUF[i];
    temp2 += ACC_Y_BUF[i];
    temp3 += ACC_Z_BUF[i];
  }
  
  ACC_AVG_X = temp1 / FILTER_NUM;
  ACC_AVG_Y = temp2 / FILTER_NUM;
  ACC_AVG_Z = temp3 / FILTER_NUM;
  
  filter_cnt++;
  
  if(filter_cnt==FILTER_NUM)  filter_cnt=0;
	
  /*GYRO_I_X += GYRO_X * Gyro_Factor * 0.0001;//0.0001��ʱ����������prepare������ִ������
  GYRO_I_Y += GYRO_Y * Gyro_Factor * 0.0001;
  GYRO_I_Z += GYRO_Z * Gyro_Factor * 0.0001;*/
}

void Get_Attitude(void)
{
	/*IMUupdate( MPU6050_GYRO_X*Gyro_Gr,
	     MPU6050_GYRO_Y*Gyro_Gr,
	     MPU6050_GYRO_Z*Gyro_Gr,
	     ACC_AVG_X,ACC_AVG_Y,ACC_AVG_Z);	//*0.0174ת�ɻ���*/
}
////////////////////////////////////////////////////////////////////////////////
#define Kp 10.0f        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.008f       // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.005f    // half the sample period�������ڵ�һ��

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error
/**********************************************************/
//�������ƣ�IMUupdate
//��ڲ�����gx�������͵�������x������
//          gy�������͵�������y������
//          gz�������͵�������z������
//          ax�������͵ļ��ٶ�x������
//          ay�������͵ļ��ٶ�y������
//          az�������͵ļ��ٶ�z������
//���ڲ�������
//�������ܣ�ͨ�������Ǻͼ��ٶȴ���������������Ԫ��������̬
/**********************************************************/
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	float norm;
	float hx, hy, hz, bx, bz;
	float vx, vy, vz;// wx, wy, wz;
	float ex, ey, ez;

	//�Ȱ���Щ��Ҫ�õ���ֵŪ��
	float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
	float q0q3 = q0*q3;
	float q1q1 = q1*q1;
	float q1q2 = q1*q2;
	float q1q3 = q1*q3;
	float q2q2 = q2*q2;
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;
	
  if(ax*ay*az==0) return;
		
  norm = sqrt(ax*ax + ay*ay + az*az);//acc���ݹ�һ��
  ax = ax / norm;
  ay = ay / norm;
  az = az / norm;

  // estimated direction of gravity and flux (v and w)   �����������������/��Ǩ
  vx = 2*(q1q3 - q0q2);	//��Ԫ����xyz�ı�ʾ
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;

  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy) ;   //�������������õ���־������
  ey = (az*vx - ax*vz) ;
  ez = (ax*vy - ay*vx) ;

  exInt = exInt + ex * Ki; //�������л���
  eyInt = eyInt + ey * Ki;
  ezInt = ezInt + ez * Ki;

  // adjusted gyroscope measurements
  gx = gx + Kp*ex + exInt; //�����PI�󲹳��������ǣ����������Ư��
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt; //�����gz����û�й۲��߽��н��������Ư�ƣ����ֳ����ľ��ǻ������������Լ�

  // integrate quaternion rate and normalise //��Ԫ����΢�ַ���
  q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  q1 = q1 + ( q0*gx + q2*gz - q3*gy)*halfT;
  q2 = q2 + ( q0*gy - q1*gz + q3*gx)*halfT;
  q3 = q3 + ( q0*gz + q1*gy - q2*gx)*halfT;

  // normalise quaternion
  norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 = q0 / norm;
  q1 = q1 / norm;
  q2 = q2 / norm;
  q3 = q3 / norm;

  //Q_ANGLE_Z = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3 + 1)* 57.3; // yaw
  Q_ANGLE_Y = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.32784; // pitch
  Q_ANGLE_X = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.32484; // roll
}

void null_shift()
{
	ACC_X -= ACC_OFFSET.X;
	ACC_Y -= ACC_OFFSET.Y;
	ACC_Z -= ACC_OFFSET.Z;

	GYRO_X -= GYRO_OFFSET.X;
	GYRO_Y -= GYRO_OFFSET.Y;
	GYRO_Z -= GYRO_OFFSET.Z;


}
