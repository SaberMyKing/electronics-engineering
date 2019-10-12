/*
 * This file contains some IMU operation.
 * By IC爬虫 (1394024051@qq.com)
 * 2014-4-29 v1.0
 */
#include "IMU.h"

extern float Q_ANGLE_X,Q_ANGLE_Y,Q_ANGLE_Z;      //四元数计算出的角度

int ACC_X_BUF[FILTER_NUM],ACC_Y_BUF[FILTER_NUM],ACC_Z_BUF[FILTER_NUM];	//加速度滑动窗口滤波数组
/*
void Data_Filter(void)//均值滤波
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
		//求和
		Sum_ACC_X = ACC_X[i];
		Sum_ACC_Y = ACC_Y[i];
		Sum_ACC_Z = ACC_Z[i];
		
		//求最大值
		Max_ACC_X = (Max_ACC_X > ACC_X[i]) ? Max_ACC_X : ACC_X[i] ;
		Max_ACC_Y = (Max_ACC_Y > ACC_Y[i]) ? Max_ACC_Y : ACC_Y[i] ;
		Max_ACC_Z = (Max_ACC_Z > ACC_Z[i]) ? Max_ACC_Z : ACC_Z[i] ;
		
		//求最小值
		Min_ACC_X = (Min_ACC_X < ACC_X[i]) ? Min_ACC_X : ACC_X[i] ;
		Min_ACC_Y = (Min_ACC_Y < ACC_Y[i]) ? Min_ACC_Y : ACC_Y[i] ;
		Min_ACC_Z = (Min_ACC_Z < ACC_Z[i]) ? Min_ACC_Z : ACC_Z[i] ;
	}
	
	//去掉最小值和最大值
	ACC_X_Filted = (Sum_ACC_X - Max_ACC_X - Min_ACC_X) / 3;
	ACC_Y_Filted = (Sum_ACC_Y - Max_ACC_Y - Min_ACC_Y) / 3;
	ACC_Z_Filted = (Sum_ACC_Z - Max_ACC_Z - Min_ACC_Z) / 3;
}*/

/**********************************************************/
//函数名称：Prepare_Data
//入口参数：无
//出口参数：无
//函数功能：读取MPU6050数据进行平滑滤波，为后续计算准备数据
/**********************************************************/
void Prepare_Data(void)
{
  static unsigned char filter_cnt=0;
  long int temp1=0,temp2=0,temp3=0;
  unsigned char i;
	
  //MPU6050_Dataanl();//完成传感器数据的读取和计算，并且对数据简单处理
	
	ACC_X_BUF[filter_cnt] = ACC_X;//更新滑动窗口数组
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
	
  /*GYRO_I_X += GYRO_X * Gyro_Factor * 0.0001;//0.0001是时间间隔，两次prepare函数的执行周期
  GYRO_I_Y += GYRO_Y * Gyro_Factor * 0.0001;
  GYRO_I_Z += GYRO_Z * Gyro_Factor * 0.0001;*/
}

void Get_Attitude(void)
{
	/*IMUupdate( MPU6050_GYRO_X*Gyro_Gr,
	     MPU6050_GYRO_Y*Gyro_Gr,
	     MPU6050_GYRO_Z*Gyro_Gr,
	     ACC_AVG_X,ACC_AVG_Y,ACC_AVG_Z);	//*0.0174转成弧度*/
}
////////////////////////////////////////////////////////////////////////////////
#define Kp 10.0f        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.008f       // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.005f    // half the sample period采样周期的一半

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error
/**********************************************************/
//函数名称：IMUupdate
//入口参数：gx：浮点型的陀螺仪x轴数据
//          gy：浮点型的陀螺仪y轴数据
//          gz：浮点型的陀螺仪z轴数据
//          ax：浮点型的加速度x轴数据
//          ay：浮点型的加速度y轴数据
//          az：浮点型的加速度z轴数据
//出口参数：无
//函数功能：通过陀螺仪和加速度传感器的数据用四元数计算姿态
/**********************************************************/
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	float norm;
	float hx, hy, hz, bx, bz;
	float vx, vy, vz;// wx, wy, wz;
	float ex, ey, ez;

	//先把这些需要用到的值弄好
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
		
  norm = sqrt(ax*ax + ay*ay + az*az);//acc数据归一化
  ax = ax / norm;
  ay = ay / norm;
  az = az / norm;

  // estimated direction of gravity and flux (v and w)   估计重力方向和流量/变迁
  vx = 2*(q1q3 - q0q2);	//四元数中xyz的表示
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3 ;

  // error is sum of cross product between reference direction of fields and direction measured by sensors
  ex = (ay*vz - az*vy) ;   //向量外积在相减得到差分就是误差
  ey = (az*vx - ax*vz) ;
  ez = (ax*vy - ay*vx) ;

  exInt = exInt + ex * Ki; //对误差进行积分
  eyInt = eyInt + ey * Ki;
  ezInt = ezInt + ez * Ki;

  // adjusted gyroscope measurements
  gx = gx + Kp*ex + exInt; //将误差PI后补偿到陀螺仪，即补偿零点漂移
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt; //这里的gz由于没有观测者进行矫正会产生漂移，变现出来的就是积分自增或者自减

  // integrate quaternion rate and normalise //四元数的微分方程
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
