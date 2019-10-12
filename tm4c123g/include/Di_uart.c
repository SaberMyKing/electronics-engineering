/*
 * Di_uart.c
 *
 *  Created on: 2014��8��9��
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
//�������ƣ�uart_re1
//���ܸ�Ҫ�����н���1���ֽ�
//����˵����uartNo: ���ں�
//         ch:    ���յ����ֽ�
//�������أ� �ɹ�:1;ʧ��:0
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
//�������ƣ�uart_send1
//���ܸ�Ҫ�����з���1���ֽ�
//����˵����uartNo: ���ں�
//         ch:     Ҫ���͵��ֽ�
//�������أ� ��
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
//�������ƣ�uart_reN
//���ܸ�Ҫ������ ����n���ֽ�
//����˵����uartNo: ���ں�
//         buff: ���ջ�����
//		   len:���ճ���
//�������أ� 1:�ɹ�;0:ʧ��
//-------------------------------------------------------------------------
uint8 uart_reN (uint32_t ui32Base ,uint16 len ,uint8* buff)
{
    uint8 m;
    uint8 fp;
    m = 0;
                                                       //����n������
    while (m<len)
    {
    	buff[m] = uart_re1(ui32Base);
	    if (fp == 1) return 1;       //ֻҪ��1���ֽ�����û���յ��ͷ��ر���
	    m++;
    }
    return 0;
}

//-------------------------------------------------------------------------
//�������ƣ�uart_sendN
//���ܸ�Ҫ������ ����n���ֽ�
//����˵����uartNo: ���ں�
//         buff: ���ͻ�����
//		   len:���ͳ���
//�������أ� ��
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
//�������ƣ�uart_send_string
//���ܸ�Ҫ����ָ��UART�˿ڷ���һ����'\0'�������ַ���
//����˵����uartNo:UARTģ���
//          buff:Ҫ���͵��ַ������׵�ַ
//�������أ� ��
//����˵�����ַ����ĳ��ȷ�Χ��0��65535,�ڲ�������hw_uart_send1����
//-------------------------------------------------------------------------
void uart_send_string(uint32_t ui32Base, uint8 *buff)
{
	uint16 i = 0;

    for(i = 0;buff[i] != '\0';i++)
    {
    	//��ָ��UART�˿ڷ����ַ�������
    	uart_send1(ui32Base,buff[i]);
    }
}


//-------------------------------------------------------------------------
//�������ƣ�uart_send_number
//���ܸ�Ҫ��������ת�����ַ������ͳ�ȥ  ��123ת����"123"���ͳ�ȥ
//����˵����uartNo: ��uartNo��UARTģ��
//         number: Ҫ�������
//         point:  ������С��λ��
//�������أ� �Ƿ����ɹ�(1:�ɹ���0:ʧ��)
//����˵�����ַ����ĳ��ȷ�Χ��0��65535,�ڲ�������hw_uart_sendN����
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
//�������ƣ�GetByteArrayFromNumber
//���ܸ�Ҫ��������numberת����ASCII�����ʽ�ŵ�������
//����˵����array: ���ת����ֵ������.ע�⣺�����鳤������Ҫ��number��
//                ���ֵ��λ����1
//         length: ����ת����ֵ�ĳ���
//         number: ��Ҫת��������
//         point:  ������С��λ��
//�������أ� �Ƿ����ɹ�(1:�ɹ���0:ʧ��)
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

    if(number == 0) //���������0
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
    //��������ASCII�����ݴ����array��
    for(j = 0; j < i; j++)
    {
        array[j+n] = pBuff[i - j] ;
    }
    return 1;
}


//-------------------------------------------------------------------------
//�������ƣ�GetHexByteArrayFromNumber
//���ܸ�Ҫ��������numberת����ʮ�����Ƶ�ASCII�����ʽ�ŵ�������
//����˵����array: ���ת����ֵ������.ע�⣺�����鳤������Ҫ��number��
//                ���ֵ��λ����1
//         length: ����ת����ֵ�ĳ���
//         number: ��Ҫת��������
//�������أ� �Ƿ����ɹ�(1:�ɹ���0:ʧ��)
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
*���ƣ�void Uart1_Send_AF
*���ܣ����ڷ�����̬����
*��ڲ�������
*���ڲ�������
*˵����ÿһ��ִ���������������һ֡���ݣ�֡ͷΪ0X88��������
*      Ϊ0XAF
**********************************************************/
void Uart1_Send_AF(void)
{
	unsigned char sum = 0;//�ۼӴ��ڷ��͵����ݵ�ֵ����У����
	unsigned int _temp;


    sum += UartTX_Send_char(0x88);//(0x88);  //֡ͷ
    sum += UartTX_Send_char(0xAF);//(0xAF);  //������  AF�ɿ���ʾ  A1������ʾ
    sum += UartTX_Send_char(0x1c);
    sum += UartTX_Send_char( BYTE1(ACC_X) ); //���ͼ��ٶ����ݵĸ�8λ
    sum += UartTX_Send_char( BYTE0(ACC_X) ); //���ͼ��ٶ����ݵĵ�8λ
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

    UartTX_Send_char(sum); //���ڷ����ۼ�ֵ����У��

}

uint8 UartTX_Send_char( uint8 ch)
{

	uart_send1(UART4, ch);
	return ch;
}
