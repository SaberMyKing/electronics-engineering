/*
 * Di_gpio.h
 *
 *  Created on: 2014Äê8ÔÂ9ÈÕ
 *      Author: Dieinwind
 */

#ifndef DI_GPIO_H_
#define DI_GPIO_H_
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/gpio.h"
#include "sysctl.h"
#include "common.h"
#include <stdbool.h>

#define GPIOA GPIO_PORTA_BASE  // GPIO Port A
#define GPIOB GPIO_PORTB_BASE  // GPIO Port B
#define GPIOC GPIO_PORTC_BASE  // GPIO Port C
#define GPIOD GPIO_PORTD_BASE  // GPIO Port D
#define GPIOE GPIO_PORTE_BASE  // GPIO Port E
#define GPIOF GPIO_PORTF_BASE  // GPIO Port F
#define GPIOG GPIO_PORTG_BASE  // GPIO Port G
#define GPIOH GPIO_PORTH_BASE  // GPIO Port H
#define GPIOJ GPIO_PORTJ_BASE  // GPIO Port J

#define pin0 GPIO_PIN_0  // GPIO pin 0
#define pin1 GPIO_PIN_1  // GPIO pin 1
#define pin2 GPIO_PIN_2  // GPIO pin 2
#define pin3 GPIO_PIN_3  // GPIO pin 3
#define pin4 GPIO_PIN_4  // GPIO pin 4
#define pin5 GPIO_PIN_5  // GPIO pin 5
#define pin6 GPIO_PIN_6  // GPIO pin 6
#define pin7 GPIO_PIN_7  // GPIO pin 7

#define GPIOIN  GPIO_DIR_MODE_IN  // Pin is a GPIO input
#define GPIOOUT GPIO_DIR_MODE_OUT  // Pin is a GPIO output
#define GPIOHW  GPIO_DIR_MODE_HW  // Pin is a peripheral function

void gpio_enable_port(void);
void gpio_init(uint32_t ui32Port, uint8_t ui8Pins, uint32_t ui32PinIO);
uint8 gpio_get(uint32_t ui32Port, uint8_t ui8Pins);
void gpio_reverse(uint32_t ui32Port, uint8_t ui8Pins);



#endif /* DI_GPIO_H_ */
