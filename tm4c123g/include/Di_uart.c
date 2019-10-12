/*
 * Di_uart.c
 *
 *  Created on: 2014年8月9日
 *      Author: Dieinwind
 */
#include "Di_uart.h"


void UARTInit()
{
    //
    // Enable the peripherals used by this example.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    //
    // Enable processor interrupts.
    //
    IntMasterEnable();

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinConfigure(GPIO_PC4_U4RX);
    GPIOPinConfigure(GPIO_PC5_U4TX);

    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    //
    UARTConfigSetExpClk(UART4_BASE, SysCtlClockGet(), 115200,
                            (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                             UART_CONFIG_PAR_NONE));

    //
    // Enable the UART interrupt.
    //
    IntEnable(INT_UART4);
    UARTIntEnable(UART4_BASE, UART_INT_RX | UART_INT_RT);
}



//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void UARTSend(uint8_t *pui8Buffer, uint32_t ui32Count)
{
    //
    // Loop while there are more characters to send.
    //
    while(ui32Count--)
    {
        //
        // Write the next character to the UART.
        //
    	UARTCharPut(UART4_BASE, *pui8Buffer++);
    }
}

//-------------------------------------------------------------------------
//函数名称：uart_re1
//功能概要：串行接受1个字节
//参数说明：uartNo: 串口号
//         ch:    接收到的字节
//函数返回： 成功:1;失败:0
//-------------------------------------------------------------------------
uint8 uart_re1 (uint32_t ui32Base)
{
    //
    // Wait until a char is available.
    //
    while(HWREG(ui32Base + UART_O_FR) & UART_FR_RXFE)
    {
    }

    //
    // Now get the char.
    //

    return(HWREG(ui32Base + UART_O_DR));
}

//-------------------------------------------------------------------------
//函数名称：uart_send1
//功能概要：串行发送1个字节
//参数说明：uartNo: 串口号
//         ch:     要发送的字节
//函数返回： 无
//-------------------------------------------------------------------------
void uart_send1 (uint32_t ui32Base, uint8 ucData)
{
/*    //
    // Wait until space is available.
    //
    while(HWREG(ui32Base + UART_O_FR) & UART_FR_TXFF)
    {
    }

    //
    // Send the char.
    //
    HWREG(ui32Base + UART_O_DR) = ucData;*/
	UARTCharPut(ui32Base, ucData);
}


//-------------------------------------------------------------------------
//函数名称：uart_reN
//功能概要：串行 接收n个字节
//参数说明：uartNo: 串口号
//         buff: 接收缓冲区
//		   len:接收长度
//函数返回： 1:成功;0:失败
//-------------------------------------------------------------------------
uint8 uart_reN (uint32_t ui32Base ,uint16 len ,uint8* buff)
{
    uint8 m;
    uint8 fp;
    m = 0;
                                                       //接收n个数据
    while (m<len)
    {
    	buff[m] = uart_re1(ui32Base);
	    if (fp == 1) return 1;       //只要有1个字节数据没接收到就返回报错
	    m++;
    }
    return 0;
}

//-------------------------------------------------------------------------
//函数名称：uart_sendN
//功能概要：串行 接收n个字节
//参数说明：uartNo: 串口号
//         buff: 发送缓冲区
//		   len:发送长度
//函数返回： 无
//-------------------------------------------------------------------------
void uart_sendN (uint32_t ui32Base ,uint16 len ,uint8* buff)
{
    int i;
	for(i=0;i<len;i++)
    {
		UARTCharPut(ui32Base, buff[i]);
    }
}


//-------------------------------------------------------------------------
//函数名称：uart_send_string
//功能概要：从指定UART端口发送一个以'\0'结束的字符串
//参数说明：uartNo:UART模块号
//          buff:要发送的字符串的首地址
//函数返回： 无
//函数说明：字符串的长度范围从0到65535,内部调用了hw_uart_send1函数
//-------------------------------------------------------------------------
void uart_send_string(uint32_t ui32Base, uint8 *buff)
{
	uint16 i = 0;

    for(i = 0;buff[i] != '\0';i++)
    {
    	//从指定UART端口发送字符串数据
    	uart_send1(ui32Base,buff[i]);
    }
}


//-------------------------------------------------------------------------
//函数名称：uart_send_number
//功能概要：将数字转换成字符串发送出去  如123转换成"123"发送出去
//参数说明：uartNo: 第uartNo个UART模块
//         number: 要传输的数
//         point:  保留的小数位数
//函数返回： 是否发生成功(1:成功，0:失败)
//函数说明：字符串的长度范围从0到65535,内部调用了hw_uart_sendN函数
//-------------------------------------------------------------------------
uint8 uart_send_number(uint32_t ui32Base, float number,uint8 point )
{
	uint8 array[10];
	uint8 len = 0;
    if(GetByteArrayFromNumber(array, &len, number,point) == 1)
    {
    	uart_sendN(ui32Base, len, (uint8*)array);
    }
    else
    {
        return 0;
    }
    return 1;
}



//-------------------------------------------------------------------------
//函数名称：GetByteArrayFromNumber
//功能概要：将数组number转换成ASCII码的形式放到数组中
//参数说明：array: 存放转换后值的数组.注意：该数组长度至少要比number的
//                最大值的位数大1
//         length: 返回转换数值的长度
//         number: 需要转换的数据
//         point:  保留的小数位数
//函数返回： 是否发生成功(1:成功，0:失败)
//-------------------------------------------------------------------------
uint8 GetByteArrayFromNumber(uint8 array [], uint8 * length, float number,uint8 point)
{
	uint32 Num;
    unsigned int i = 0;
    unsigned int j = 0;
    uint8 pBuff [10],n=0;
    if(!point)
      number=(int)number;
    else if(point>5)
    	point=5;
    if(number < 0)
    {
    	array[n++]='-';
    	number=-number;
    }

    if(number == 0) //如果数字是0
    {
        i = 1;
        *length = i;
        array[0] = '0';
        return 1;
    }
  else if(number<1)
    {
	  array[n++]=0x30;
	  array[n++]='.';
      number*=10;
      while(number<1)
        {
    	  array[n++]=0x30;
    	  number*=10;
    	  i++;
         }
      while(number>(int)number)
        {
           number*=10;
           if(++i>=point-1)
        	 number=(int)number;
         }
      i=0;
      }
    else
       while(number>(int)number)
       {
       	number*=10;
       	if(++j>=point)
       		number=(int)number;
       }
     Num=number;
       while(Num)
       {
       	if((i++==j)&&j)
       		pBuff[i]='.';
       	else
       	{
       		pBuff[i]= Num%10+0x30;
       	 Num/=10;
       	}
       }
    *length = i+n;
    //将解析的ASCII码数据存放在array中
    for(j = 0; j < i; j++)
    {
        array[j+n] = pBuff[i - j] ;
    }
    return 1;
}


//-------------------------------------------------------------------------
//函数名称：GetHexByteArrayFromNumber
//功能概要：将数字number转换成十六进制的ASCII码的形式放到数组中
//参数说明：array: 存放转换后值的数组.注意：该数组长度至少要比number的
//                最大值的位数大1
//         length: 返回转换数值的长度
//         number: 需要转换的数据
//函数返回： 是否发生成功(1:成功，0:失败)
//-------------------------------------------------------------------------
//uint8 GetHexByteArrayFromNumber(uint8 array [], uint8 * length, uint32 number);


unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
	unsigned short CRC_Temp;
	unsigned char i,j;
	CRC_Temp = 0xffff;
	for (i=0;i<CRC_CNT; i++)
	{
		CRC_Temp ^= Buf[i];
			for (j=0;j<8;j++)
			{
				if (CRC_Temp & 0x01)
					CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
				else
					CRC_Temp = CRC_Temp >> 1;
			}
	}
	return(CRC_Temp);
}

float OutputData[4];

void OutPut_Data(uint8 uartNo)
{
	int temp[4] = {0};
	unsigned int temp1[4] = {0};
	unsigned char databuf[10] = {0};
	unsigned char i;
	unsigned short CRC16 = 0;
	for(i=0;i<4;i++)
	{
		temp[i]  = (int)OutputData[i];
		temp1[i] = (unsigned int)temp[i];
	}
	for(i=0;i<4;i++)
	{
		databuf[i*2]   = (unsigned char)(temp1[i]%256);
		databuf[i*2+1] = (unsigned char)(temp1[i]/256);
	}
	CRC16 = CRC_CHECK(databuf,8);
     databuf[8] = CRC16%256;
     databuf[9] = CRC16/256;

	for(i=0;i<10;i++)
	//hw_uart_send1(uartNo,databuf[i]);
	uart_send1(UART4, databuf[i] );
}

void ShowOnComputer(uint8 uartNo, float data1, float data2, float data3, float data4 )
{
	//data1 = GValueOfTuoLuo2;    //JiaoDuFromJiaSu
	//data2 =PWMStandControlValue;
	//data1 = GTempValueOfTuoLuo;
	//data2 = TurnSpeedFromTuoLuo2;
	//data2 = GValueOfJiaSu;
	//data3 = JiaoDuFromTuoLuo;
	//data4 = JiaoDuFromJiaSu;
	//data1 =   RightPulCNT;
	//data2  =   LeftPulCNT;
	//data3 =   GCarSpeed*100;
	//data4  =JiaoDuFromTuoLuo;
	//data1  =   RightPulCNT;
	//data2  =   LeftPulCNT;
	//data3  =   PWMSpeedControlValue;
	//data4  =   GCarSpeed*100;
	//data1 =displayl;
	//data2 = displayr;
	//data3 = PWMDirectControlValue;
	//data4 =   GCarSpeed*100;
	//data3 = Direction_delta;
	//data3 = GDirectControlOutNew;
	//data4 = LightAvg;
	//data4 =  Lightsum;
	//data1 = leftside;
	//data2 =  rightside;
	//data3 =  ccdfinal;
	//data3 =  mid;
	//data4 =  PWMDirectControlValue;
	//data4 = GValueOfTuoLuo2;
	//data4 =  GCarSpeed*100;
	//data1 = GTempValueOfTuoLuo;
	//data2 = GValueOfJiaSu;
	//data3 = JiaoDuFromTuoLuo;
	//data4 = JiaoDuFromJiaSu;

	OutputData[0] = data1;
	OutputData[1] = data2;
	OutputData[2] = data3;
	OutputData[3] = data4;
	OutPut_Data(uartNo);
}

/*********************************************************
*名称：void Uart1_Send_AF
*功能：串口发送姿态数据
*入口参数：无
*出口参数：无
*说明：每一次执行这个函数就算是一帧数据，帧头为0X88，功能字
*      为0XAF
**********************************************************/
void Uart1_Send_AF(void)
{
	unsigned char sum = 0;//累加串口发送的数据的值，做校验用
	unsigned int _temp;


    sum += UartTX_Send_char(0x88);//(0x88);  //帧头
    sum += UartTX_Send_char(0xAF);//(0xAF);  //功能字  AF飞控显示  A1波形显示
    sum += UartTX_Send_char(0x1c);
    sum += UartTX_Send_char( BYTE1(ACC_X) ); //发送加速度数据的高8位
    sum += UartTX_Send_char( BYTE0(ACC_X) ); //发送加速度数据的低8位
    sum += UartTX_Send_char( BYTE1(ACC_Y) );
    sum += UartTX_Send_char( BYTE0(ACC_Y) );
    sum += UartTX_Send_char( BYTE1(ACC_Z) );
    sum += UartTX_Send_char( BYTE0(ACC_Z) );
    sum += UartTX_Send_char( BYTE1(GYRO_X) );
    sum += UartTX_Send_char( BYTE0(GYRO_X) );
    sum += UartTX_Send_char( BYTE1(GYRO_Y) );
    sum += UartTX_Send_char( BYTE0(GYRO_Y) );
    sum += UartTX_Send_char( BYTE1(GYRO_Z) );
    sum += UartTX_Send_char( BYTE0(GYRO_Z) );
    sum += UartTX_Send_char(0);
    sum += UartTX_Send_char(0);
    sum += UartTX_Send_char(0);
    sum += UartTX_Send_char(0);
    sum += UartTX_Send_char(0);
    sum += UartTX_Send_char(0);

    _temp = (long int)(Q_ANGLE_X*100);
    sum += UartTX_Send_char( BYTE1(_temp) );
    sum += UartTX_Send_char( BYTE0(_temp) );
    _temp = (long int)(Q_ANGLE_Y*100);
    sum += UartTX_Send_char( BYTE1(_temp) );
    sum += UartTX_Send_char( BYTE0(_temp) );

     _temp = (long int)(Q_ANGLE_Z);
     sum += UartTX_Send_char( BYTE1(_temp) );
     sum += UartTX_Send_char( BYTE0(_temp) );
     //sum += UartTX_Send_char(0);
    //sum += UartTX_Send_char(0);
    sum += UartTX_Send_char(0);
    sum += UartTX_Send_char(0);
    sum += UartTX_Send_char(0);
    sum += UartTX_Send_char(0);

    UartTX_Send_char(sum); //串口发送累加值用于校验

}

uint8 UartTX_Send_char( uint8 ch)
{

	uart_send1(UART4, ch);
	return ch;
}
