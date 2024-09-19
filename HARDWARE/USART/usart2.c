#include "sys.h"
#include "usart2.h"	
extern u8 recv_ok;       //������ɱ�־
extern u8 uart_buf[32];  //���ڱ��洮������
extern u8 uart_cnt;        //���ڶ�λ�������ݵ�λ��

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
	
		USART_InitStructure.USART_BaudRate = 9600;//������
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

u8 t;
void USART2_IRQHandler(void)
{
	//����־λ
  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
	  //��������
		t=USART_ReceiveData(USART2);
		//�����յ����������α��浽������
//		flag = t;
		uart_buf[uart_cnt++]=t;
		//GM65ģ�鷢��һ�����ݺ���Զ�����һ���س���������ͨ������Ƿ���ܵ��س����ж������Ƿ�������
		if(t==0x0D)
		{
		  recv_ok =1;//�������
		}

		USART_ClearITPendingBit(USART2,  USART_IT_RXNE);//��ձ�־λ
  } 
}

