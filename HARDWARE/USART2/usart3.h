#ifndef __USART3_H
#define __USART3_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 


//#define USART_REC_LEN  			63  	//�����������ֽ��� 200
#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART3_RX_BUF[64]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8 USART3_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void USART3_Init(u32 bound);
void uartWriteBuf(uint8_t *buf, uint8_t len);
#endif


