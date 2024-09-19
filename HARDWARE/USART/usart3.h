#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 


//#define USART_REC_LEN  			63  	//定义最大接收字节数 200
#define EN_USART3_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8  USART3_RX_BUF[64]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8 USART3_RX_STA;         		//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void USART3_Init(u32 bound);
void uartWriteBuf(uint8_t *buf, uint8_t len);
#endif


