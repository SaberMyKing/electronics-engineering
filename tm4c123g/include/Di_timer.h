/*
 * Di_timer.h
 *
 *  Created on: 2014Äê8ÔÂ9ÈÕ
 *      Author: Dieinwind
 */

#ifndef DI_TIMER_H_
#define DI_TIMER_H_
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"

void Timer0Init(uint32_t ms);
void Timer1Init(uint32_t ms);

#endif /* DI_TIMER_H_ */
