#include "huidu.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "delay.h"
uint8_t Fcount=0;   //前方灰度传感器扫过的黑线数目
uint8_t Bcount=0;   //后方灰度扫过的黑线数目
uint8_t Lcount=0;   //左方灰度扫过的黑线数目
uint8_t Rcount=0;   //右方灰度扫过的黑线数目
short int Forward[3]={0};
short int Back[3]={0};
short int Left[3]={0};
short int Right[3]={0};
extern char cl,ch;
extern short int flag_z;//
extern char flag_check; //校正检查判断标志位
void TIM6_Int_Init(void)//¶¨Ê±Æ÷²ÉÑù
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);  //Ê±ÖÓ¿ªÆô
	
  TIM_TimeBaseInitStructure.TIM_Period = 5000-1; 	
	TIM_TimeBaseInitStructure.TIM_Prescaler=84-1;  
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //ÏòÉÏ¼ÆÊý
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); 
	TIM_Cmd(TIM6,ENABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
short int time1,time2=0;
void TIM6_DAC_IRQHandler(void)//5ms
{
	if(TIM_GetITStatus(TIM6,TIM_IT_Update) != RESET)
	{TIM_ClearITPendingBit(TIM6,TIM_IT_Update);//Çå¿ÕÖÐ¶Ï±êÖ¾Î»
			date_huidu();
//	printf("%d",I);
	}
	
}
/*-----------------------------------------------------------
函数功能：灰度引脚初始化
入口参数：无
返 回 值：无
说    明：从左至右
前灰度：F0 F1 F2 F3 F4 F5 F6
后灰度：F8 F9 F10 F11 F12 F13 F14 (F15)
左灰度：
右灰度：
-----------------------------------------------------------*/
void gray_GPIO_init(void)
{
	/*右灰度引脚*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;   //输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_Init(GPIOF,&GPIO_InitStructure);

	/*前灰度引脚*/
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;   //输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;   //输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	/*左灰度引脚*/
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;   //输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

	/*后灰度引脚*/
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;   //输出模式
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

}


void date_huidu(void)
{
if(F1||F2||F3)
	{
	Forward[0]=1;
	}
	else{Forward[0]=0;}
	if(F4)
	{
	Forward[1]=1;
	}
	else{Forward[1]=0;}
	if(F5||F6||F7)
	{
	Forward[2]=1;
	}
	else{Forward[2]=0;}
	if(R1||R2||R3)
	{
	Right[0]=1;
	}
	else{Right[0]=0;}
	if(R4)
	{
	Right[1]=1;
	}
	else{Right[1]=0;}
	if(R5||R6||R7)
	{
	Right[2]=1;
	}
	else{Right[2]=0;}
	if(B1||B2||B3)
	{
	Back[0]=1;
	}
	else{Back[0]=0;}
	if(B4)
	{
	Back[1]=1;
	}
	else{Back[1]=0;}
	if(B5||B6||B7)
	{
	Back[2]=1;
	}
	else{Back[2]=0;}
	if(L1||L2||L3)
	{
	Left[0]=1;
	}
	else{Left[0]=0;}
	if(L4)
	{
	Left[1]=1;
	}
	else{Left[1]=0;}
	if(L5||L6||L7)
	{
	Left[2]=1;
	}
	else{Left[2]=0;}
//printf("%d\r\n",Right[0]);
}
short int action=0;//开始处理车身
short int actionZ=0;//开始处理车身
short int actionX=0;//开始处理车身
short int actionY=0;//开始处理车身
///*-----------------------------------------------------------
//函数功能：前灰度寻迹
//入口参数：直行速度
//返 回 值：返回flag值（是否扫过黑线）
//说    明：偏左偏右速度根据车体调整
//-----------------------------------------------------------*/
void Track_Gyro()
{
	if(flag_check=='A')
	{
		action =1;
		printf("#K\r\n");
		flag_check = 0x00;
	}
while(action)
{
	
//	if(USART_RX_BUF[0]=='#'&&USART_RX_BUF[1]=='s'&&USART_RX_BUF[2]=='t'&&USART_RX_BUF[3]=='o')
//	{
//		action =0;
//    printf("#ok\r\n");
//		USART_RX_STA=0;
//	}
	while(1)
	{
	if(flag_check=='Z')
	{    
		USART_RX_STA=0;	
		if(flag_z==1)
		{
		printf("#C%c%c\r\n",ch,cl);
		}
		if(flag_z==0)
		{
		printf("#W%c%c\r\n",ch,cl);
		}
		flag_check = 0x00;
		break;
	}
	}
//	7
	if(F4&&B4&&L4&&R4)//前有一个检测到黑线
	{
			printf("#S\r\n");//全部回正
		  delay_ms(50);
			printf("#S\r\n");//全部回正
			delay_ms(50);
			printf("#S\r\n");//全部回正
		  delay_ms(50);
			printf("#S\r\n");//全部回正
			delay_ms(50);
			printf("#S\r\n");//全部回正
		actionZ=0;
		action=0;
		actionX=0;//开始处理车身
    actionY=0;//开始处理车身
		break;
	}
	delay_ms(200);
	printf("#K\r\n");
	
	while(flag_check!='X')
	{}
	if(flag_check=='X')
	{    
		flag_check = 0x00;
		actionX=1;
   if((Right[0]==Right[1]==Right[2]==0)&&(Left[0]==Left[1]==Left[2]==0))
	 {
	 printf("#K\r\n");
		 actionX=0;
	 }
		if(F4&&B4&&L4&&R4)//前有一个检测到黑线
		{
			printf("#S\r\n");//全部回正
		  delay_ms(50);
			printf("#S\r\n");//全部回正
			delay_ms(50);
			printf("#S\r\n");//全部回正
		  delay_ms(50);
			printf("#S\r\n");//全部回正
			delay_ms(50);
			printf("#S\r\n");//全部回正
			action=0;
			actionX=0;//开始处理车身
			actionY=0;//开始处理车身
			break;
	}
	if(Right[1]==1&&Left[1]==1)
	{
		printf("#K\r\n");
		actionZ=0;
		action=0;
	  actionX=0;
	}
	if(Right[0]||Left[2])
	{ 
		printf("#B\r\n");//向前
		delay_ms(600);
		printf("#K\r\n");}
//		while(actionX)
//		{
//			if(x==20)
//			{
//			printf("#K\r\n");
//		    actionX=0;
//			}
			
//		if(Right[1]==1&&Left[1]==1)
//		 {
//		 printf("#K\r\n");
//		 actionX=0;
//		flag_check = 0x00;
//	   }
//		 else
//     {
//      printf("#B\r\n");//向前
//			 x++;
//     }
//		}
//	 }
	else if(Right[2]||Left[0])
	{ 
		printf("#F\r\n");
		delay_ms(600);
		printf("#K\r\n");
		
	}
	flag_check = 0x00;
		while((flag_check!='Y'))
	{}
	
	if(flag_check=='Y')
	{    
		flag_check = 0x00;
		actionY=1;
 if((Forward[0]==Forward[1]==Forward[2]==0)&&(Back[0]==Back[1]==Back[2]==0))
 {
		 actionY=0;
			printf("#S\r\n");//全部回正
		  delay_ms(50);
			printf("#S\r\n");//全部回正
			delay_ms(50);
			printf("#S\r\n");//全部回正
		  delay_ms(50);
			printf("#S\r\n");//全部回正
			delay_ms(50);
			printf("#S\r\n");//全部回正
			 action=0;
	 }		
	if(F4&&B4&&L4&&R4)//前有一个检测到黑线
	{
			printf("#S\r\n");//全部回正
		  delay_ms(50);
			printf("#S\r\n");//全部回正
			delay_ms(50);
			printf("#S\r\n");//全部回正
		  delay_ms(50);
			printf("#S\r\n");//全部回正
			delay_ms(50);
			printf("#S\r\n");//全部回正
		actionZ=0;
		action=0;
		actionX=0;//开始处理车身
    actionY=0;//开始处理车身
		break;
	}
	if(Forward[1]==1&&Back[1]==1)
	{
		//printf("#K\r\n");
		actionZ=0;
		action=0;
	  actionX=0;
		actionY=0;
			printf("#S\r\n");//全部回正
		  delay_ms(50);
			printf("#S\r\n");//全部回正
			delay_ms(50);
			printf("#S\r\n");//全部回正
		  delay_ms(50);
			printf("#S\r\n");//全部回正
			delay_ms(50);
			printf("#S\r\n");//全部回正
		action=0;
		break;
	}
	if(Back[0]||Forward[2])
	{ 
		printf("#L\r\n");//向左
		delay_ms(500);
			printf("#S\r\n");
			printf("#S\r\n");//全部回正
		  delay_ms(50);
			printf("#S\r\n");//全部回正
		action=0;

	 }
	else if(Back[2]||Forward[0])
	{ 

		printf("#R\r\n");//逆时针
					delay_ms(500);
			printf("#S\r\n");
			printf("#S\r\n");//全部回正
		  delay_ms(50);
			printf("#S\r\n");//全部回正
		action=0;

	}
	if(F4&&B4&&L4&&R4)//前有一个检测到黑线
	{
			printf("#S\r\n");//全部回正
		  delay_ms(50);
			printf("#S\r\n");//全部回正
			delay_ms(50);
			printf("#S\r\n");//全部回正
		  delay_ms(50);
			printf("#S\r\n");//全部回正
			delay_ms(50);
			printf("#S\r\n");//全部回正
		actionZ=0;
		action=0;
		actionX=0;//开始处理车身
    actionY=0;//开始处理车身
		break;
	}
	flag_check = 0x00;
	printf("#S\r\n");//全部回正
	delay_ms(50);
	printf("#S\r\n");//全部回正
	break;
}
}
}
}
