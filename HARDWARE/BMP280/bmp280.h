#ifndef __BMP280_H
#define __BMP280_H


#include "sys.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "stdbool.h"
#include "math.h"
#include "usart.h"
#include "OLED.h"
#define BMP280_ADDR						(0xEC)
#define BMP280_DEFAULT_CHIP_ID			(0x58)

#define BMP280_CHIP_ID					(0xD0)         /* Chip ID Register */
#define BMP280_RST_REG					(0xE0)         /* Softreset Register */
#define BMP280_STAT_REG					(0xF3)         /* Status Register */
#define BMP280_CTRL_MEAS_REG			(0xF4)         /* Ctrl Measure Register */
#define BMP280_CONFIG_REG				(0xF5)         /* Configuration Register */
#define BMP280_PRESSURE_MSB_REG			(0xF7)         /* Pressure MSB Register */
#define BMP280_PRESSURE_LSB_REG			(0xF8)         /* Pressure LSB Register */
#define BMP280_PRESSURE_XLSB_REG		(0xF9)         /* Pressure XLSB Register */
#define BMP280_TEMPERATURE_MSB_REG		(0xFA)         /* Temperature MSB Reg */
#define BMP280_TEMPERATURE_LSB_REG		(0xFB)         /* Temperature LSB Reg */
#define BMP280_TEMPERATURE_XLSB_REG		(0xFC)         /* Temperature XLSB Reg */

#define BMP280_SLEEP_MODE				(0x00)
#define BMP280_FORCED_MODE				(0x01)
#define BMP280_NORMAL_MODE				(0x03)

#define BMP280_TEMPERATURE_CALIB_DIG_T1_LSB_REG             (0x88)
#define BMP280_PRESSURE_TEMPERATURE_CALIB_DATA_LENGTH       (24)
#define BMP280_DATA_FRAME_SIZE			(6)

#define BMP280_OVERSAMP_SKIPPED			(0x00)
#define BMP280_OVERSAMP_1X				(0x01)
#define BMP280_OVERSAMP_2X				(0x02)
#define BMP280_OVERSAMP_4X				(0x03)
#define BMP280_OVERSAMP_8X				(0x04)
#define BMP280_OVERSAMP_16X				(0x05)


bool bmp280Init(void);
void bmp280GetData(float* pressure, float* temperature, float* asl);


//IO方向设置
#define SDA_IN()  {GPIOB->MODER&=~(3<<(11*2));GPIOB->MODER|=0<<11*2;}	
#define SDA_OUT() {GPIOB->MODER&=~(3<<(11*2));GPIOB->MODER|=1<<11*2;} 
//IO操作函数	 
#define IIC_SCL    PBout(10)                                                   //SCL
#define IIC_SDA    PBout(11)                                                   //SDA	 
#define READ_SDA   PBin(11)                                                    //输入SDA 

//IIC所有操作函数
void BMP_IIC_Init(void);                                                           //初始化IIC的IO口				 
void BMP_IIC_Start(void);				                                           //发送IIC开始信号
void BMP_IIC_Stop(void);	  			                                           //发送IIC停止信号
void BMP_IIC_Send_Byte(u8 txd);			                                           //IIC发送一个字节
u8   BMP_IIC_Read_Byte(unsigned char ack);                                           //IIC读取一个字节
u8   BMP_IIC_Wait_Ack(void); 				                                           //IIC等待ACK信号
void BMP_IIC_Ack(void);					                                           //IIC发送ACK信号
void BMP_IIC_NAck(void);				                                           //IIC不发送ACK信号

void BMP_IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8   BMP_IIC_Read_One_Byte(u8 daddr,u8 addr);	
			 
u8   BMP_iicDevReadByte(u8 devaddr,u8 addr);	                                       /*读一字节*/
void BMP_iicDevWriteByte(u8 devaddr,u8 addr,u8 data);	                           /*写一字节*/
void BMP_iicDevRead(u8 devaddr,u8 addr,u8 len,u8 *rbuf);                           /*连续读取多个字节*/
void BMP_iicDevWrite(u8 devaddr,u8 addr,u8 len,u8 *wbuf);                          /*连续写入多个字节*/

void bmp280_test(void);
#endif
