#ifndef __HMC5883L_H
#define __HMC5883L_H

#include "stm32f4xx.h"
#include "bmp280.h"
#include "delay.h"
#include "usart.h"
#include "OLED.h"
//#define WRITE_ADDRESS  0X1A
//#define READ_ADDRESS 0X1b
#define WRITE_ADDRESS  0x3c
#define READ_ADDRESS 0x3d
#define CONFIGA 0x00
#define CONFIGB 0x01
#define MODE 0x02
#define DATAX_M 0x03
#define DATAX_L 0x04
#define DATAY_M 0x05
#define DATAY_L 0x06
#define DATAZ_M 0x07
#define DATAZ_L 0x08
#define STATUS 0x09



void i2c_init(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(void );
void hmc_write_reg(u8 reg,u8 data);
u8 hmc_read_reg(u8 reg);
void hmc_read_XYZ(short int *data);
void hmc_init(void);
void hmc_read_xyz(void);
#endif

