/*
 * Di_gpio.c
 *
 *  Created on: 2014Äê8ÔÂ9ÈÕ
 *      Author: Dieinwind
 */
#include "Di_gpio.h"


void gpio_enable_port(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
}
void gpio_init(uint32_t ui32Port, uint8_t ui8Pins, uint32_t ui32PinIO)
{
	//
	// Set the pad(s) for standard push-pull operation.
	//
	GPIOPadConfigSet(ui32Port, ui8Pins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD);

	//
	// Make the pin(s) be outputs.
	//
	GPIODirModeSet(ui32Port, ui8Pins, ui32PinIO);
}

void gpio_reverse(uint32_t ui32Port, uint8_t ui8Pins)
{
	GPIOPinWrite(ui32Port, ui8Pins, ~GPIOPinRead(ui32Port,ui8Pins));
	//GPIOPinRead(ui32Port,ui8Pins);
}


