#include "sys.h"
#include "usart2.h"	
extern u8 recv_ok;       //接收完成标志
extern u8 uart_buf[32];  //用于保存串口数据
extern u8 uart_cnt;        //用于定位串口数据的位置

//串口2初始化
void USART2_ITconfig(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

		GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2-TX
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2-RX
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
		GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		USART_InitStructure.USART_BaudRate = 9600;//波特率
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//数据位
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//停止位
		USART_InitStructure.USART_Parity = USART_Parity_No ;//奇偶校验
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//流控
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//模式
		USART_Init(USART2, &USART_InitStructure);
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //定义中断分组	（一般都是2  打断）
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;    //中断函数优先设置
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //相应优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;     //抢占优先级
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;      // 使能
		NVIC_Init(&NVIC_InitStructure); 
		
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  
		USART_Cmd(USART2, ENABLE);		
}

u8 t;
void USART2_IRQHandler(void)
{
	//检测标志位
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
	  //接收数据
		t=USART_ReceiveData(USART2);
		//将接收到的数据依次保存到数组里
//		flag = t;
		uart_buf[uart_cnt++]=t;
		//GM65模块发完一组数据后会自动发送一个回车符，所以通过检测是否接受到回车来判断数据是否接收完成
		if(t==0x0D)
		{
		  recv_ok =1;//接收完成
		}

		USART_ClearITPendingBit(USART2,  USART_IT_RXNE);//清空标志位
  } 
}

