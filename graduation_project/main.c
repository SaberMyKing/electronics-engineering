/****************************************
 * 
 * 浙江理工大学
 * 信息学院
 * 12电子信息工程3班
 * 梁鑫磊
 * 2016-3-12
 *  
 ****************************************/

#include "uart_PC.h"
#include "clock.h"
#include "Timers.h"
#include "common.h"
#include "include/buttons.h"
#include "include/rgb.h"
#include "mpu6050.h"
#include "mpr121.h"
#include "IIC.h"
#include "OLED.h"
#include "touch_led_bee.h"
#include "WIFI.h"
#include "pedometer.h"
#include "my_eeprom.h"

int main(void)
{
	ms_delay(500);
	clock_50MHZ();
	
	SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
	
	IntMasterDisable();
	
	uart0_init();
	uart4_init();
	uart_RX_FIFO_clear();
	
	TouchPadLedInit();
	BeeInit();
	WorkingLedInit();
	PowerLedInit();
	TouchPadLedOnAll();
	
	SysTickInit();
	RTC_Start(7, 2016, 4, 6, 13, 5, 55);
	int i = 0;
	for(i=0;i<3;i++)
	{
		AlarmTimeSet(i, 8, 30, OFF);
	}
	//AlarmTimeSet(1, 1, 6, ON_ONCE);
	
	OLED_init();
	OLED_DesktopUpdate();
	
	IIC1_init();
	IIC3_init();
	mpr121_init();
	mpu6050_init();
	
	Wide_timer0A_init();
	
	IntMasterEnable();
	
	WIFI_init();
	
	uart_send_str("System Initialize Finished...\n");
	//OLED_DesktopUpdate();

	BeeAlarm();

	ms_delay(200);
	TouchPadLedOffAll();
	
	//PadometerHourDataDefault();
	PadometerHourDataReadAll();
	
	g_OLEDLastFlag = UPDATE_DESKTOP;
	g_OLEDUpdateFlag = 0;
	
	IntEnable(INT_UART4);
    UARTIntEnable(UART4_BASE, UART_INT_RX | UART_INT_RT);
	
	while(1)
	{
		while(!g_OLEDUpdateFlag);
		uart_send_uint(g_OLEDLastFlag);
		if(g_OLEDLastFlag!=g_OLEDUpdateFlag)
		{
			OLED_clear();
		}
		switch(g_OLEDUpdateFlag)
		{
		case UPDATE_DATE:
			OLED_DateUpdate(g_CurrentTime.Year, g_CurrentTime.Month, g_CurrentTime.Date, g_CurrentTime.WeekDay);
			g_OLEDLastFlag = UPDATE_DATE;
			break;
		case UPDATE_TIME:
			OLED_TimeUpdate(g_CurrentTime.Hour, g_CurrentTime.Minute);
			g_OLEDLastFlag = UPDATE_TIME;
			break;
		case UPDATE_DESKTOP:
			OLED_DesktopUpdate();
			g_OLEDLastFlag = UPDATE_DESKTOP;
			break;
		case UPDATE_MAINMENU:
			OLED_MainMenuUpdate();
			g_OLEDLastFlag = UPDATE_MAINMENU;
			break;
		case UPDATE_SUBMENU:
			g_OLEDLastFlag = UPDATE_SUBMENU;
			break;
		case UPDATE_PEDOMETER_DATE:
			OLED_PedometerDateUpdate(g_LastPedometerDataAddr);
			g_OLEDLastFlag = UPDATE_PEDOMETER_DATE;
			break;
		case UPDATE_TORCH:
			OLED_TorchUpdate();
			g_OLEDLastFlag = UPDATE_TORCH;
			break;
		case UPDATE_CURTIME:
			OLED_TimeSetUpdate();
			g_OLEDLastFlag = UPDATE_CURTIME;
			break;
		default:
			break;
		}
		
		
		g_OLEDUpdateFlag = 0;
		
		
	}
	
	return 0;
}
