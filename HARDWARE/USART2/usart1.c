//#include "sys.h"
//#include "usart.h"	
//#include "usart3.h"	
//#include "stm32f4xx.h"

//static unsigned char TxBuffer[256];
//static unsigned char TxCounter=0;
//static unsigned char count=0; 
//extern void CopeSerial1Data(unsigned char ucData);

//void Initial_UART1(unsigned long bound)
//{
//		//GPIO端口设置
//		GPIO_InitTypeDef GPIO_InitStructure;
//		USART_InitTypeDef USART_InitStructure;
//		NVIC_InitTypeDef NVIC_InitStructure;
//		
//		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
//	 
//		//串口1对应引脚复用映射
//		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
//		GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
//		
//		//USART1端口配置
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
//		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
//		GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10

//		 //USART1 初始化设置
//		USART_InitStructure.USART_BaudRate = bound;//波特率设置
//		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
//		USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
//		USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
//		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
//		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
//		USART_Init(USART1, &USART_InitStructure); //初始化串口1
//		
//		USART_Cmd(USART1, ENABLE);  //使能串口1 
//		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//		NVIC_Init(&NVIC_InitStructure);
//}

//void UART1_Put_Char(unsigned char DataToSend)
//{
//		TxBuffer[count++] = DataToSend;  
//		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  
//}

//void UART1_Put_String(unsigned char *Str)
//{
//	while(*Str)
//	{
//		if(*Str=='\r')UART1_Put_Char(0x0d);
//			else if(*Str=='\n')UART1_Put_Char(0x0a);
//				else UART1_Put_Char(*Str);
//		Str++;
//	}
//}


////重定义fputc函数 
//int fputc(int ch, FILE *f)
//{      
//		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
//			USART1->DR = (u8) ch;      
//		return ch;
//}



//void USART1_IRQHandler(void)
//{
//  
//		if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
//		{   
//			USART_SendData(USART1, TxBuffer[TxCounter++]); 
//			if(TxCounter == count) 
//			{
//				USART_ITConfig(USART1, USART_IT_TXE, DISABLE);// 全部发送完成
//			}
//			USART_ClearITPendingBit(USART1, USART_IT_TXE); 
//		}
//		else if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
//		{
//			CopeSerial1Data((unsigned char)USART1->DR);//处理数据
//			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//		}
//		USART_ClearITPendingBit(USART1,USART_IT_ORE);

//}

////void CopeSerial1Data(unsigned char ucData)
////{	
////	USART3_Put_Char(ucData);//转发串口1收到的数据给串口2（JY模块）
////}
