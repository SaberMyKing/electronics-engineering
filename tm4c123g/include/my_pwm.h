#ifndef __MY_PWM_H__
#define __MY_PWM_H__


#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/debug.h"

extern void Init_Uart_Console(void);
//extern void PWM_Pulse_Gen(int frequency,int duty_div);


#endif //end my_pwm.h