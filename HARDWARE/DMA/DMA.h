#ifndef __DMA_H
#define __DMA_H

#include "stm32f4xx.h"
#include "delay.h"
#include "string.h"
struct SAcc
{
	short a[3];
	short T;
};
struct SGyro
{
	short w[3];
	short T;
};
struct SAngle
{
	short Angle[3];
	short T;
};
 

void CopeSerial2Data(unsigned char ucData);
void UART2_Put_Char(unsigned char DataToSend);
void uart2_init(u32 bound);
void sendcmd(char cmd[]);
void UART2_Put_String(unsigned char *Str);
#endif 
