#ifndef __I2C3_H
#define __I2C3_H
#include "sys.h" 
 
#include "stm32f4xx.h"
 
// I2C3初始化函数
void I2C3_Init(void);
 
// I2C3发送数据函数
// 参数：
//   address: 目标设备地址
//   reg: 寄存器地址
//   data: 要发送的数据
void I2C3_WriteData(uint8_t address, uint8_t reg, uint8_t data);
 
// I2C3接收数据函数
// 参数：
//   address: 目标设备地址
//   reg: 寄存器地址
// 返回值：
//   读取到的数据
uint8_t I2C3_ReadData(uint8_t address, uint8_t reg);
 
 
 
#endif
 

