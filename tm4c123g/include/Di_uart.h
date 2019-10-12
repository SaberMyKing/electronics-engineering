/*
 * Di_uart.h
 *
 *  Created on: 2014年8月9日
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


//原始的加速度测量值
extern int16 ACC_X;
extern int16 ACC_Y;
extern int16 ACC_Z;
//原始的陀螺仪测量值
extern int16 GYRO_X;
extern int16 GYRO_Y;
extern int16 GYRO_Z;

//将“int”类型的数据分成两个单字节的数据
#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
//欧拉角
extern float Q_ANGLE_X , Q_ANGLE_Y, Q_ANGLE_Z;

void UARTInit();

void UARTSend(uint8_t *pui8Buffer, uint32_t ui32Count);

//-------------------------------------------------------------------------
//函数名称：uart_re1
//功能概要：串行接受1个字节
//参数说明：ui32Base: 串口号
//函数返回： 接受到的字节
//-------------------------------------------------------------------------
uint8 uart_re1 (uint32_t ui32Base);

//-------------------------------------------------------------------------
//函数名称：uart_send1
//功能概要：串行发送1个字节
//参数说明：ui32Base: 串口号
//         ucData:     要发送的字节
//函数返回： 无
//-------------------------------------------------------------------------
void uart_send1 (uint32_t ui32Base, uint8 ucData);


//-------------------------------------------------------------------------
//函数名称：uart_reN
//功能概要：串行 接收n个字节
//参数说明：uartNo: 串口号
//         buff: 接收缓冲区
//		   len:接收长度
//函数返回： 1:成功;0:失败
//-------------------------------------------------------------------------
uint8 uart_reN (uint32_t ui32Base ,uint16 len ,uint8* buff);

//-------------------------------------------------------------------------
//函数名称：uart_sendN
//功能概要：串行 接收n个字节
//参数说明：uartNo: 串口号
//         buff: 发送缓冲区
//		   len:发送长度
//函数返回： 无
//-------------------------------------------------------------------------
void uart_sendN (uint32_t ui32Base ,uint16 len ,uint8* buff);


//-------------------------------------------------------------------------
//函数名称：uart_send_string
//功能概要：从指定UART端口发送一个以'\0'结束的字符串
//参数说明：uartNo:UART模块号
//          buff:要发送的字符串的首地址
//函数返回： 无
//函数说明：字符串的长度范围从0到65535,内部调用了hw_uart_send1函数
//-------------------------------------------------------------------------
void uart_send_string(uint32_t ui32Base, uint8 *buff);


//-------------------------------------------------------------------------
//函数名称：uart_send_number
//功能概要：将数字转换成字符串发送出去  如123转换成"123"发送出去
//参数说明：uartNo: 第uartNo个UART模块
//         number: 要传输的数
//         point:  保留的小数位数
//函数返回： 是否发生成功(1:成功，0:失败)
//函数说明：字符串的长度范围从0到65535,内部调用了hw_uart_sendN函数
//-------------------------------------------------------------------------
uint8 uart_send_number(uint32_t ui32Base, float number,uint8 point );


//-------------------------------------------------------------------------
//函数名称：uart_send_hex_number
//功能概要：将数字转换成十六进制字符串发送出去  如16转换成"F"发送出
//参数说明：uartNo: 第uartNo个UART模块
//         number: 要传输的数组
//函数返回： 是否发生成功(1:成功，0:失败)
//函数说明：字符串的长度范围从0到65535,内部调用了hw_uart_sendN函数
//-------------------------------------------------------------------------
uint8 uart_send_hex_number(uint8 uartNo, uint32 number);


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
uint8 GetByteArrayFromNumber(uint8 array [], uint8 * length, float number,uint8 point);


//-------------------------------------------------------------------------
//函数名称：GetHexByteArrayFromNumber
//功能概要：将数字number转换成十六进制的ASCII码的形式放到数组中
//参数说明：array: 存放转换后值的数组.注意：该数组长度至少要比number的
//                最大值的位数大1
//         length: 返回转换数值的长度
//         number: 需要转换的数据
//函数返回： 是否发生成功(1:成功，0:失败)
//-------------------------------------------------------------------------
uint8 GetHexByteArrayFromNumber(uint8 array [], uint8 * length, uint32 number);



unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);

void OutPut_Data(uint8 uartNo);

void ShowOnComputer(uint8 uartNo, float data1, float data2, float data3, float data4 );

/*********************************************************
*名称：void Uart1_Send_AF
*功能：串口发送姿态数据
*入口参数：无
*出口参数：无
*说明：每一次执行这个函数就算是一帧数据，帧头为0X88，功能字
*      为0XAF
**********************************************************/
void Uart1_Send_AF(void);

uint8 UartTX_Send_char( uint8 ch);

#endif /* DI_UART_H_ */
