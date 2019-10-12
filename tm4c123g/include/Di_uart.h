/*
 * Di_uart.h
 *
 *  Created on: 2014��8��9��
 *      Author: Dieinwind
 */



#ifndef DI_UART_H_
#define DI_UART_H_

#include <stdint.h>
#include <stdbool.h>
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"

#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "common.h"

#define UART0 UART0_BASE// UART0
#define UART1 UART1_BASE// UART1
#define UART2 UART2_BASE// UART2
#define UART3 UART3_BASE// UART3
#define UART4 UART4_BASE// UART4
#define UART5 UART5_BASE// UART5
#define UART6 UART6_BASE// UART6
#define UART7 UART7_BASE// UART7


//ԭʼ�ļ��ٶȲ���ֵ
extern int16 ACC_X;
extern int16 ACC_Y;
extern int16 ACC_Z;
//ԭʼ�������ǲ���ֵ
extern int16 GYRO_X;
extern int16 GYRO_Y;
extern int16 GYRO_Z;

//����int�����͵����ݷֳ��������ֽڵ�����
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
//ŷ����
extern float Q_ANGLE_X , Q_ANGLE_Y, Q_ANGLE_Z;

void UARTInit();

void UARTSend(uint8_t *pui8Buffer, uint32_t ui32Count);

//-------------------------------------------------------------------------
//�������ƣ�uart_re1
//���ܸ�Ҫ�����н���1���ֽ�
//����˵����ui32Base: ���ں�
//�������أ� ���ܵ����ֽ�
//-------------------------------------------------------------------------
uint8 uart_re1 (uint32_t ui32Base);

//-------------------------------------------------------------------------
//�������ƣ�uart_send1
//���ܸ�Ҫ�����з���1���ֽ�
//����˵����ui32Base: ���ں�
//         ucData:     Ҫ���͵��ֽ�
//�������أ� ��
//-------------------------------------------------------------------------
void uart_send1 (uint32_t ui32Base, uint8 ucData);


//-------------------------------------------------------------------------
//�������ƣ�uart_reN
//���ܸ�Ҫ������ ����n���ֽ�
//����˵����uartNo: ���ں�
//         buff: ���ջ�����
//		   len:���ճ���
//�������أ� 1:�ɹ�;0:ʧ��
//-------------------------------------------------------------------------
uint8 uart_reN (uint32_t ui32Base ,uint16 len ,uint8* buff);

//-------------------------------------------------------------------------
//�������ƣ�uart_sendN
//���ܸ�Ҫ������ ����n���ֽ�
//����˵����uartNo: ���ں�
//         buff: ���ͻ�����
//		   len:���ͳ���
//�������أ� ��
//-------------------------------------------------------------------------
void uart_sendN (uint32_t ui32Base ,uint16 len ,uint8* buff);


//-------------------------------------------------------------------------
//�������ƣ�uart_send_string
//���ܸ�Ҫ����ָ��UART�˿ڷ���һ����'\0'�������ַ���
//����˵����uartNo:UARTģ���
//          buff:Ҫ���͵��ַ������׵�ַ
//�������أ� ��
//����˵�����ַ����ĳ��ȷ�Χ��0��65535,�ڲ�������hw_uart_send1����
//-------------------------------------------------------------------------
void uart_send_string(uint32_t ui32Base, uint8 *buff);


//-------------------------------------------------------------------------
//�������ƣ�uart_send_number
//���ܸ�Ҫ��������ת�����ַ������ͳ�ȥ  ��123ת����"123"���ͳ�ȥ
//����˵����uartNo: ��uartNo��UARTģ��
//         number: Ҫ�������
//         point:  ������С��λ��
//�������أ� �Ƿ����ɹ�(1:�ɹ���0:ʧ��)
//����˵�����ַ����ĳ��ȷ�Χ��0��65535,�ڲ�������hw_uart_sendN����
//-------------------------------------------------------------------------
uint8 uart_send_number(uint32_t ui32Base, float number,uint8 point );


//-------------------------------------------------------------------------
//�������ƣ�uart_send_hex_number
//���ܸ�Ҫ��������ת����ʮ�������ַ������ͳ�ȥ  ��16ת����"F"���ͳ�
//����˵����uartNo: ��uartNo��UARTģ��
//         number: Ҫ���������
//�������أ� �Ƿ����ɹ�(1:�ɹ���0:ʧ��)
//����˵�����ַ����ĳ��ȷ�Χ��0��65535,�ڲ�������hw_uart_sendN����
//-------------------------------------------------------------------------
uint8 uart_send_hex_number(uint8 uartNo, uint32 number);


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
uint8 GetByteArrayFromNumber(uint8 array [], uint8 * length, float number,uint8 point);


//-------------------------------------------------------------------------
//�������ƣ�GetHexByteArrayFromNumber
//���ܸ�Ҫ��������numberת����ʮ�����Ƶ�ASCII�����ʽ�ŵ�������
//����˵����array: ���ת����ֵ������.ע�⣺�����鳤������Ҫ��number��
//                ���ֵ��λ����1
//         length: ����ת����ֵ�ĳ���
//         number: ��Ҫת��������
//�������أ� �Ƿ����ɹ�(1:�ɹ���0:ʧ��)
//-------------------------------------------------------------------------
uint8 GetHexByteArrayFromNumber(uint8 array [], uint8 * length, uint32 number);



unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);

void OutPut_Data(uint8 uartNo);

void ShowOnComputer(uint8 uartNo, float data1, float data2, float data3, float data4 );

/*********************************************************
*���ƣ�void Uart1_Send_AF
*���ܣ����ڷ�����̬����
*��ڲ�������
*���ڲ�������
*˵����ÿһ��ִ���������������һ֡���ݣ�֡ͷΪ0X88��������
*      Ϊ0XAF
**********************************************************/
void Uart1_Send_AF(void);

uint8 UartTX_Send_char( uint8 ch);

#endif /* DI_UART_H_ */
