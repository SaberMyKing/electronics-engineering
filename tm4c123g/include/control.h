/*
 * control.h
 *
 *  Created on: Jul 26, 2014
 *      Author: Dieinwind
 */
#include "common.h"
#include "my_pwm.h"
#include "Di_uart.h"
#ifndef CONTROL_H_
#define CONTROL_H_

#define A 1

//ԭʼ�������ǲ���ֵ
extern int16 GYRO_X;
extern int16 GYRO_Y;
extern int16 GYRO_Z;

//�����ر���
//P


void CONTROL(uint16 THROTTLE, float roll_angle, float pit_angle, float yaw_angle);

void PWM_Out(uint16 moto1, uint16 moto2,uint16 moto3,uint16 moto4);



#endif /* CONTROL_H_ */
