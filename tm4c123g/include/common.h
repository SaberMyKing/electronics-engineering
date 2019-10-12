/*
 * common.h
 *
 *  Created on: 2014年8月9日
 *      Author: Dieinwind
 */

#ifndef COMMON_H_
#define COMMON_H_
//类型别名宏定义
typedef unsigned char         uint8;  // 无符号8位数，字节
typedef unsigned short int    uint16; // 无符号16位数，字
typedef unsigned long int     uint32; // 无符号32位数，长字

typedef volatile uint8        vuint8;  // 不优化无符号8位数，字节
typedef volatile uint16       vuint16; // 不优化无符号16位数，字
typedef volatile uint32       vuint32; // 不优化无符号32位数，长字

typedef char                  int8;   // 有符号8位数
typedef short int             int16;  // 有符号16位数
typedef int                   int32;  // 有符号32位数

typedef volatile int8         vint8;  // 不优化有符号8位数
typedef volatile int16        vint16; // 不优化有符号16位数
typedef volatile int32        vint32; // 不优化有符号32位数


#endif /* COMMON_H_ */
