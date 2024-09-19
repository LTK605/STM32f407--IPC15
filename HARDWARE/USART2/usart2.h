#ifndef __USART_H_
#define __USART_H_
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
	
#define USART2_MAX_RECV_LEN  			200  	//定义最大接收字节数 200
#define USART2_MAX_SEND_LEN        200   //定义最大发送字节数 200
extern vu16 USART2_RX_STA; 	
void USART2_ITconfig(void);

#endif


