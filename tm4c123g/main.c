

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/timer.h"
#include "include/Di_uart.h"
#include "include/Di_gpio.h"
#include "include/IMU.h"
#include "include/control.h"
#include "include/my_pwm.h"


//原始的加速度测量值
int16 ACC_X  = 11;
int16 ACC_Y  = 11;
int16 ACC_Z  = 11;
//原始的陀螺仪测量值
int16 GYRO_X = 11;
int16 GYRO_Y = 11;
int16 GYRO_Z = 11;

//滤波过后的加速度值
int16 ACC_X_Filted;
int16 ACC_Y_Filted;
int16 ACC_Z_Filted;

int   ACC_AVG_X = 0, ACC_AVG_Y = 0, ACC_AVG_Z = 0;      //平均值滤波后的ACC
float GYRO_I_X,GYRO_I_Y,GYRO_I_Z;         //陀螺仪积分
float EXP_ANGLE_X,EXP_ANGLE_Y,EXP_ANGLE_Z;//期望角度
float DIF_ANGLE_X,DIF_ANGLE_Y,DIF_ANGLE_Z;//期望角度和实际角度的差

uint32_t Timer0ChangeFlags;

float Q_ANGLE_X = 1, Q_ANGLE_Y = 1, Q_ANGLE_Z = 1;

uint8 Calcu_Counter = 0 ;
uint8 Send_Data_Counter = 0;
uint16 State_Counter = 0;
uint16 PWM_Counter = 0;
uint16 PWM_Percent = 0;
uint8 PIT_FUN_Counter = 0;

//计数变量
uint32 delay = 0;
uint32 delay2 = 0;
uint32 delay3 = 0;
uint32 delay4=  0;

uint16 THROTTLE = 0;
float THROTTLE_float = 0;
int Y_Angle_Error = 9;
int X_Angle_Error = -10;

int temp1 = 0;
float temp3 = 20;
float temp2 = 0.0;


//*****************************************************************************
int main(void)
{
    FPUEnable();
    FPULazyStackingEnable();
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);//50MHz
    gpio_enable_port();
    //GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    gpio_init(GPIOF, pin1, GPIOOUT);
    gpio_init(GPIOF, pin2, GPIOOUT);
    gpio_init(GPIOA, pin2, GPIOOUT);
    Soft_I2C_Init();
    UARTInit();
    UARTSend((uint8_t *)"\033[2JEnter text: ", 16);
    //配置定时中断
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/100);//定时1ms中断
	IntEnable(INT_TIMER0A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	TimerEnable(TIMER0_BASE, TIMER_A);
	Init_PWM_console();
	//变量初始化
	THROTTLE = 62500 / 20;
	THROTTLE_float = 62500 / 20;
	GYRO_OFFSET.X = 0;
	GYRO_OFFSET.Y = 0;
	GYRO_OFFSET.Z = 0;
	ACC_OFFSET.X = 0;
	ACC_OFFSET.Y = 0;
	ACC_OFFSET.Z = 0;

	delay2 = 0;
	delay = 0;
	delay4 =0;
    while(1)
    {
    	//do nothing
    }
}

void UARTIntHandler(void)
{
    uint32_t ui32Status;

    //
    // Get the interrrupt status.
    //
    ui32Status = UARTIntStatus(UART4_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    UARTIntClear(UART4_BASE, ui32Status);

    //
    // Loop while there are characters in the receive FIFO.
    //
    while(UARTCharsAvail(UART4_BASE))
    {
        //
        // Read the next character from the UART and write it back to the UART.
        //
        UARTCharPutNonBlocking(UART4_BASE,
                                   UARTCharGetNonBlocking(UART4_BASE));

        //
        // Blink the LED to show a character transfer is occuring.
        //
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

        //
        // Delay for 1 millisecond.  Each SysCtlDelay is about 3 clocks.
        //
        SysCtlDelay(SysCtlClockGet() / (1000 * 3));

        //
        // Turn off the LED
        //
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

    }
}


void Timer0IntHandler(void)
{
    char cOne, cTwo;
    TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
    //IntMasterDisable();
	//采集原始数据

	delay++;
	delay2++;
	delay3++;
	if(delay4 == 0)
	{
		if(delay3 > 200)
		{
			if(THROTTLE < (62500 / 20 + 400))
			{
				THROTTLE  ++;

			}
			else
			{
				delay4++;
			}
		}
	}


	MPU6050_GetData();//读取MPU6050数据
	//

	//数据处理,
	null_shift();//零漂
	Prepare_Data();//滤波

	Calcu_Counter++;
	Send_Data_Counter++;
	State_Counter++;

	//PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0, THROTTLE );
    //PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, THROTTLE );
    //PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, THROTTLE );
    //PWMPulseWidthSet(PWM1_BASE, PWM_OUT_2, THROTTLE );

	Calcu_Counter = 0;
	IMUupdate(GYRO_X*Gyro_Gr, GYRO_Y*Gyro_Gr, GYRO_Z*Gyro_Gr, ACC_AVG_X, ACC_AVG_Y, ACC_AVG_Z);
	CONTROL(THROTTLE, Q_ANGLE_X , Q_ANGLE_Y , 0 );

	if(Send_Data_Counter == 1) //发送数据
	{
		Send_Data_Counter = 0;
		//Uart1_Send_AF();
		ACC_AVG_X = ACC_AVG_X > 8400 ? 8402 : ACC_AVG_X;
		ACC_AVG_Y = ACC_AVG_Y > 8400 ? 8402 : ACC_AVG_Y;
		temp1 = -asin(((float)ACC_AVG_X ) / 8400 )*57.32784;
		temp2 = asin(((float)ACC_AVG_Y ) / 8400 )*57.32784;
		//ShowOnComputer(UART4, temp1, temp2,Q_ANGLE_X,Q_ANGLE_Y);
		//uart_send_string(UART4, (uint8 *)("Hello World\n"));
		//uart_send_number(UART4, ACC_X, 0);
		//UARTCharPut(UART4, '\n');
		//UARTSend((uint8_t *)"\033[2JEnter text: ", 16);
	}
	State_Counter++;
	if(State_Counter >= 100)
	{

		GPIOPinWrite(GPIOF, GPIO_PIN_1, ~GPIOPinRead(GPIOF, GPIO_PIN_1));
		//uint32_t  ui32Reg = 0, ui32GenBase ;
		//ui32GenBase = PWM_OUT_BADDR(PWM0_BASE, ui32PWMOut);
		//ui32Reg = HWREG(ui32GenBase + PWM_O_X_LOAD);
		//hw_uart_send1(UART0, 'A');
		State_Counter = 0;
	}
	//GPIOPinWrite(GPIOF, GPIO_PIN_1, ~GPIO_PIN_1);
    //IntMasterEnable();
}

