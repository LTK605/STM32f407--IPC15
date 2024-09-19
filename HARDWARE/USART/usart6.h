#ifndef __USART6_H
#define __USART6_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
#define EN_USART6_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
extern u16 USART6_RX_STA;         		//����״̬���	

void USART6_Init(unsigned long baudrate);
void USART6_IRQHandler(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendArray(USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);

#endif
