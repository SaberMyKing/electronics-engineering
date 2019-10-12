/*
 * Di_timer.c
 *
 *  Created on: 2014Äê8ÔÂ9ÈÕ
 *      Author: Dieinwind
 */

#include "Di_timer.h"


void Timer0Init(uint32_t ms)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    //
    // Enable processor interrupts.
    //
    IntMasterEnable();
    //
    // Configure the two 32-bit periodic timers.
    //
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/1000);
    //
    // Setup the interrupts for the timer timeouts.
    //
    IntEnable(INT_TIMER0A);
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    //
    // Enable the timers.
    //
    TimerEnable(TIMER0_BASE, TIMER_A);
}

void Timer1Init(uint32_t ms)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER1_BASE, TIMER_A, SysCtlClockGet() / 1000 );
	IntEnable(INT_TIMER1A);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER1_BASE, TIMER_A);
}


