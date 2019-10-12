/*
 * control.c
 *
 *  Created on: Jul 26, 2014
 *      Author: Dieinwind
 */
#include "Control.h"



float roll_P_out, pitch_P_out, yaw_P_out = 0;
float roll_I_out = 0, pitch_I_out = 0, yaw_I_out = 0;
float roll_D_out = 0, pitch_D_out = 0, yaw_D_out = 0;
float PID_roll_OUT = 0;
float PID_pitch_OUT = 0;
float PID_yaw_OUT = 0;
//
float KP_roll = -4.02;
float KI_roll = 0;
float KD_roll = -0.04;

float KP_pitch = -4.02;
float KI_pitch = 0;
float KD_pitch = -0.04;

float KP_yaw = -4.02;
float KI_yaw = 0;
float KD_yaw = -0.04;

int Temp1 = 0;
int Temp2 = 0;





void CONTROL(uint16 THROTTLE, float roll_angle, float pit_angle, float yaw_angle)
{
	uint16 moto1=111,moto2=111,moto3=111,moto4=111;
	Temp1 = roll_angle;
	Temp2 = pit_angle;
	roll_P_out = KP_roll * roll_angle;
	roll_D_out = KD_roll * GYRO_X;
	
	pitch_P_out = KP_pitch * pit_angle;
	pitch_D_out = KD_pitch * GYRO_Y;
	
	yaw_D_out = KD_yaw * GYRO_Z;
	
	PID_roll_OUT  = roll_P_out  + roll_D_out;
	PID_pitch_OUT = pitch_P_out + pitch_D_out;
	PID_yaw_OUT   =               yaw_D_out;
	
	moto1 = THROTTLE + PID_roll_OUT - PID_pitch_OUT - PID_yaw_OUT;
	moto2 = THROTTLE + PID_roll_OUT + PID_pitch_OUT + PID_yaw_OUT;
	moto3 = THROTTLE - PID_roll_OUT + PID_pitch_OUT - PID_yaw_OUT;
	moto4 = THROTTLE - PID_roll_OUT - PID_pitch_OUT + PID_yaw_OUT;

	

	PWM_Out( moto1, moto2,moto3,moto4);

	/*if(ARMED)	Moto_PwmRflash(moto1,moto2,moto3,moto4);
	else			Moto_PwmRflash(0,0,0,0);*/
}
/*
void Pid_init(void)
{
	PID_ROL.P = 12;
	PID_ROL.I = 0;
	PID_ROL.D = 0.2;
	
	PID_PIT.P = 12;
	PID_PIT.I = 0;
	PID_PIT.D = 0.2;
	
	PID_YAW.P = 0;
	PID_YAW.I = 0;
	PID_YAW.D = 0.5;
	
	PID_ROL.pout = 0;
	PID_ROL.iout = 0;
	PID_ROL.dout = 0;
	
	PID_PIT.pout = 0;
	PID_PIT.iout = 0;
	PID_PIT.dout = 0;
	
	PID_YAW.pout = 0;
	PID_YAW.iout = 0;
	PID_YAW.dout = 0;
}


*/

void PWM_Out(uint16 moto1, uint16 moto2,uint16 moto3,uint16 moto4)
{
	//±ß½ç¼ì²é
	moto1 = moto1 >= 5625 ? 5625 : moto1;
	moto2 = moto2 >= 5625 ? 5625 : moto2;
	moto3 = moto3 >= 5625 ? 5625 : moto3;
	moto4 = moto4 >= 5625 ? 5625 : moto4;
	
	moto1 = moto1 <=  3125 ?  3125 : moto1;
	moto2 = moto2 <=  3125 ?  3125 : moto2;
	moto3 = moto3 <=  3125 ?  3125 : moto3;
	moto4 = moto4 <=  3125 ?  3125 : moto4;
	

	ShowOnComputer(UART4, moto1, moto3, Temp1 + 3500, Temp2 + 3500);
    
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,moto1 );
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3,moto2 );
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0,moto3 );
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1,moto4 );

    

}
