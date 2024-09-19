#ifndef __I2C2_H
#define __I2C2_H
#include "sys.h" 
 #include "stm32f4xx.h"
 void I2C2_Init(void);// I2C2初始化函数
void I2C2_Start(void);
void I2C2_Stop(void);
// I2C2发送数据函数
// 参数：
//   address: 目标设备地址
//   reg: 寄存器地址
//   data: 要发送的数据
void I2C2_SendData(uint8_t slaveAddr, uint8_t* data, uint8_t len);
// I2C2接收数据函数
// 参数：
//   address: 目标设备地址
//   reg: 寄存器地址
//   len：长度
// 返回值：
//   读取到的数据
uint8_t I2C2_ReadData(uint8_t address, uint8_t reg);
uint8_t I2C2_HMC_ReadData(uint8_t Write_address,uint8_t Recive_adress, uint8_t reg);
void I2C2_BMP_WriteData(uint8_t address, uint8_t reg, uint8_t data);
#endif
 

