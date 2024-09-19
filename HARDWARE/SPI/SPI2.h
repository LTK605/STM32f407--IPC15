#ifndef __SPI2_H
#define __SPI2_H
#include "sys.h" 

 void SPI2_Init(void);
//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@refSPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz：
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData);
#endif
 

