#include "my_pwm.h"

/*void PWM_Pulse_Gen(int Period,int duty_div)
{
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, Period);

    //duty_div 为分频因子，也即占空比因子
    ASSERT(Period<SysCtlClockGet());
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_0,
                     Period / duty_div);
    
}*/
void Init_PWM_console()      //pwm控制初始化
{
    uint32_t ui32GenBase1, ui32Reg1;   
  
    SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);           //50MHz system clock
   //
    // Set the PWM clock to the system clock.
    //
    //SysCtlPWMClockSet(SYSCTL_PWMDIV_1);      // PWM clock is processor clock /1
    SysCtlPWMClockSet(SYSCTL_PWMDIV_16);      // PWM clock is processor clock /8

     //
    // The PWM peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);

     //
    // For this example PWM0 is used with PortB Pins 6 and 7.  The actual port
    // and pins used may be different on your part, consult the data sheet for
    // more information.  GPIO port B needs to be enabled so these pins can be
    // used.
    // TODO: change this to whichever GPIO port you are using.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    //
    //the GPIO pin muxing to select PWM functions for these pins.
    // This step selects which alternate function is available for these pins.
    // This is necessary if your part supports GPIO pin function muxing.
    // Consult the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinConfigure(GPIO_PB5_M0PWM3);
    //GPIOPinConfigure(GPIO_PB7_M0PWM1);
    GPIOPinConfigure(GPIO_PB4_M0PWM2);
    GPIOPinConfigure(GPIO_PD0_M1PWM0);
    GPIOPinConfigure(GPIO_PD1_M1PWM1);

    //
    // Configure the GPIO pad for PWM function on pins PB6 and PB7.  Consult
    // the data sheet to see which functions are allocated per pin.
    // TODO: change this to select the port/pin you are using.
    //
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_5);
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
    GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);
    //GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_7);       //6与7互补

    //
    // Configure the PWM0 to count up/down without synchronization.
    // Note: Enabling the dead-band generator automatically couples the 2
    // outputs from the PWM block so we don't use the PWM synchronization.
    //
    /*PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
                        PWM_GEN_MODE_NO_SYNC);*/
    PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN |
                        PWM_GEN_MODE_NO_SYNC);
    PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
                        PWM_GEN_MODE_NO_SYNC);
    /*PWMGenConfigure(PWM1_BASE, PWM_GEN_1, PWM_GEN_MODE_UP_DOWN |
                        PWM_GEN_MODE_NO_SYNC);*/
    //
    // Set the PWM period to 500Hz.  To calculate the appropriate parameter
    // use the following equation: N = (1 / f) * SysClk(20MHz).  Where N is the
    // function parameter, f is the desired frequency, and SysClk is the
    // system clock frequency.
    // In this case you get: (1 / 500Hz) * 20MHz = 50000 cycles.  Note that
    // the maximum period you can set is 65535=2^16 - 1.
    // TODO: modify this calculation to use the clock frequency that you are
    // using.       50M/50000=400Hz
    //
    //PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 62500);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, 62500);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, 62500);
    //PWMGenPeriodSet(PWM1_BASE, PWM_GEN_1, 62500);
    
    //
    // Enables the counter for a PWM generator block.
    //
    //PWMGenEnable(PWM0_BASE, PWM_GEN_0);
    PWMGenEnable(PWM0_BASE, PWM_GEN_1);
    PWMGenEnable(PWM1_BASE, PWM_GEN_0);
    //PWMGenEnable(PWM1_BASE, PWM_GEN_1);
    
    //
    // Set PWM0 PD0 to a duty cycle of 25%.  You set the duty cycle as a
    // function of the period.  Since the period was set above, you can use the
    // PWMGenPeriodGet() function.  For this example the PWM will be high for
    // 50% of the time or 16000 clock cycles (64000 / 4).
    //
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2,
                         PWMGenPeriodGet(PWM0_BASE, PWM_OUT_2) / 5);
    //ui32GenBase1=PWM0_BASE+PWM_OUT_0&0xffffffc0;
    //ui32Reg1 = HWREG(ui32GenBase1 + 0x00000010);
    //HWREG(ui32GenBase1 + 0x0000001C) = ui32Reg1-25000/5;
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3,
                         PWMGenPeriodGet(PWM0_BASE, PWM_OUT_3) / 5);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0,
                         PWMGenPeriodGet(PWM1_BASE, PWM_OUT_0) / 5);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1,
                         PWMGenPeriodGet(PWM1_BASE, PWM_OUT_1) / 5);
    //
    // Enable the dead-band generation on the PWM0 output signal.  PWM bit 0
    // (PD0), will have a duty cycle of 25% (set above) and PWM bit 1 will have
    // a duty cycle of 75%.  These signals will have a 10us gap between the
    // rising and falling edges.  This means that before PWM bit 1 goes high,
    // PWM bit 0 has been low for at LEAST 160 cycles (or 10us) and the same
    // before PWM bit 0 goes high.  The dead-band generator lets you specify
    // the width of the "dead-band" delay, in PWM clock cycles, before the PWM
    // signal goes high and after the PWM signal falls.  For this example we
    // will use 160 cycles (or 10us) on both the rising and falling edges of
    // PD0.  Reference the datasheet for more information on dead-band
    // generation.
    //
    //PWMDeadBandEnable(PWM0_BASE, PWM_GEN_0, 160,160 );
    //PWMDeadBandEnable(PWM1_BASE, PWM_GEN_0,160,160 );
    //
    // Enable the PWM0 Bit 0 (PB6) and Bit 1 (PB7) output signals.
    //  define PWM_OUT_4_BIT | define PWM_OUT_6_BIT | define PWM_OUT_7_BIT
    //
    PWMOutputState(PWM0_BASE,0x000000ff, true);
    PWMOutputState(PWM1_BASE,0x000000ff, true);
}
