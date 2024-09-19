#ifndef __USART3_H
#define __USART3_H	 
#include "sys.h"  


#define USART3_MAX_RECV_LEN		1024			    //?????????
#define USART3_MAX_SEND_LEN		600					//?????????
#define USART3_RX_EN 			1					//0,???;1,??.

extern u8  USART3_RX_BUF[USART3_MAX_RECV_LEN]; 		//????,??USART3_MAX_RECV_LEN??
extern u8  USART3_TX_BUF[USART3_MAX_SEND_LEN]; 		//????,??USART3_MAX_SEND_LEN??
extern vu16 USART3_RX_STA;   						//??????

void usart3_init(u32 bound);		//??2??? 
void usart3_set(u8 bps,u8 parity);    
void usart3_rx(u8 enable);
void u3_printf(char* fmt,...);
#endif











