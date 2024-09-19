#include "sys.h"
#include "usart2.h"	
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	 
u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//????,??UART4_MAX_RECV_LEN???.
u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 			//????,??UART4_MAX_SEND_LEN??
vu16 USART2_RX_STA=0; 


void u2_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);	
	for(j=0;j<i;j++)							
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);  
		USART_SendData(USART2,USART2_TX_BUF[j]); 
	} 
}

//����2��ʼ��
void USART2_ITconfig(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

		GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2-TX
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3����ΪUSART2-RX
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		USART_InitStructure.USART_BaudRate = 115200;//������
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//����λ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No ;//��żУ��
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//����
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ģʽ
		USART_Init(USART2, &USART_InitStructure);
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�����жϷ���	��һ�㶼��2  ��ϣ�
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;    //�жϺ�����������
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��Ӧ���ȼ�
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;     //��ռ���ȼ�
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;      // ʹ��
		NVIC_Init(&NVIC_InitStructure); 
		
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  
		USART_Cmd(USART2, ENABLE);		
}
//u8 Get_computer_information()
//{
//	u8 i;
//	u8 yaw_neg_bit = 0;
//	u8 Dec_bit = 0;
//	float yaw_Dec = 0;
//	
//	u8 ring_bit = 0;
//	
//	u8 j=0;
//	if((USART_RX_STA2&0x8000)!=0)
//	{
//		switch(USART_RX_BUF2[0])
//		{
//			case 'A':
//				yaw = 0;
//				for(i=2;i<(USART_RX_STA2&0X3FFF);i++)
//				{
//					if(USART_RX_BUF2[i]=='-'){yaw_neg_bit=1;continue;}
//					if(USART_RX_BUF2[i]=='.'){Dec_bit=1;continue;}
//					if(Dec_bit==0){yaw*=10;yaw += (USART_RX_BUF2[i]-'0');}
//					else{yaw_Dec*=10; yaw_Dec += (USART_RX_BUF2[i]-'0');}
//				}
//				yaw+=(yaw_Dec/100);
//				if(yaw_neg_bit)yaw=-yaw;
//				USART_RX_STA2=0;
//			return 1;
//	
//			case 'R':
//				Ring[0] = 0;
//				Ring[1] = 0;
//				for(i=2;i<(USART_RX_STA2&0X3FFF);i++)
//				{
//					if(USART_RX_BUF2[i]==','){ring_bit=1;continue;}
//					if(ring_bit==0){Ring[0]*=10;Ring[0] += (USART_RX_BUF2[i]-'0');}
//					else{Ring[1]*=10; Ring[1] += (USART_RX_BUF2[i]-'0');}
//				}
//				USART_RX_STA2=0;
//			return 2;
//			case 'E':
//				for(i=2;i<(USART_RX_STA2&0X3FFF);i++)code[i-2] = USART_RX_BUF2[i];
//				USART_RX_STA2=0;
//			return 3;
//		
//			case 'C':
//				for(i=2;i<(USART_RX_STA2&0X3FFF);i++)
//				{
//					if(USART_RX_BUF2[i]!='+')
//					{
//						color[j] = USART_RX_BUF2[i];
//						j++;
//					}
//				}
//				USART_RX_STA2=0;
//			return 4;
//			
//	
//		}
//		USART_RX_STA2=0;
//	}
//	return 0;
//}
void USART2_IRQHandler(void)
{u8 Res;   
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����жϣ�������0x0d 0x0a��β
	{
		Res =USART_ReceiveData(USART2);//��ȡ��������
		if((USART2_RX_STA&0x8000)==0)//����δ���
		{
			if(USART2_RX_STA&0x4000)//���յ�0x0d
			{
				if(Res!=0x0a)USART2_RX_STA=0;//���մ���������
				else USART2_RX_STA|=0x8000;	//�������
			}
			else //û�յ�0X0D
			{	
				if(Res==0x0d)USART2_RX_STA|=0x4000;
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART2_MAX_RECV_LEN-1))USART2_RX_STA=0;//���ݴ������¿�ʼ  
				}		 
			}
		} 
	u2_printf("?????:%c/r/n",Res);		
  } 
}

