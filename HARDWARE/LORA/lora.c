#include "lora.h"
#include "sys.h"
#include "delay.h"
#include "usart3.h"
#include "string.h"
#include "stdio.h"
#include "usart.h"
#include "led.h"
//#include "key.h"


//??????(????????)
u8 Lora_mode=0;//0:???? 

extern u8 USART_RX_BUF[USART_REC_LEN];     //????,??USART_REC_LEN???.
EXTI_InitTypeDef EXTI_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;


//LORA??????
void LoRa_SendData(void)
{    
    	u8 temp[256] = "Hello Lora !!!";

			u3_printf("%s\r\n",temp);
			
}


//Lora??????
void LoRa_ReceData(void)
{
		u16 len=0;  

  if(USART3_RX_STA&0x8000)
	{
		len = USART3_RX_STA&0X7FFF;
		USART3_RX_BUF[len]=0;//?????
		USART3_RX_STA=0;
		printf("???????");
		printf("%s\r\n",USART3_RX_BUF);
  }

}

//???????????
void LoRa_Process(void)
{
	 u8 key=0;
	 u8 t=0;
	 static u8 n = 1;	

   while(1)
		{
			if(n==1)
			{
			 printf("??KEY0????\r\n");
			 n++;
			}
	     //key = KEY_Scan(0);
	    if(key==1)//KEY0_PRES)
			{
				if(n==2)
				{
					printf("KEY0????\r\n");
				  LoRa_SendData();//????  
					printf("??????\r\n");
				}
			}
		  LoRa_ReceData();
		  t++;
			if(t==20)
			{
					t=0;
					LED1=~LED1;
			}		
						delay_ms(10);	
			
	  }
}


void Lora_Test(void)
{   
	  u8 t=0;
		u8 key=0;

		while(1)
		{ 

			printf("??KEY_UP??????\r\n");
		  key = 1;//KEY_Scan(0);
			if(key==1)//WKUP_PRES)
			{
	      printf("??????\r\n");
			  LoRa_Process();//??????
			}
				 t++;
				if(t==30)
				{
					t=0;
					LED1=~LED1;
				}
				delay_ms(10);
		}
}

