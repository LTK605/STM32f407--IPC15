#include "ENC.h"
#include "usart.h"
#include "led.h"
extern int circle_count[4];
extern double direction[4];

void Get_direction(void)
{
float32_t v_w,v_y,v_x=0;
float32_t pSinVal=0;
float32_t pCosVal=0;
float32_t v1,v2,v3=0;
float32_t a=0;
double x,y,alpha=0;
v1=(circle_count[0]*1000/(5*4*Line));
v2=(circle_count[1]*1000/(5*4*Line));
v3=	(circle_count[0]*1000/(5*4*Line));
v_w=((v1+v2+v3 )/(3*length));//圈每秒
a=v_w*0.05f*36.0f;//角度
arm_sin_cos_f32(a, &pSinVal, &pCosVal);
v_y=(pSinVal*(v2+v3-(2.0f*v1)*Wength+al*pCosVal*(v3-v2))/3.0f);
v_x=(pCosVal*(v2+v3-(2.0f*v1)*Wength+al*pSinVal*(v2-v3))/3.0f);
x=v_x*0.005f;
y=v_y*0.005f;
alpha=a;
direction[0]+=x;
direction[1]+=y;
direction[2]+=alpha;	
	
}

void Speed_ENC(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1 ENABLE   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//ENABLE TIME
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5 ENABLE   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ENABLE TIME
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8 ENABLE   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//ENABLE TIME
	
	/************???1?????******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11; //PTE9 PTE11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //浮空输入
	
	GPIO_Init(GPIOE,&GPIO_InitStructure); 
	/***********************************************/
	
	
	/************???5?????******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //PTA0 PTA1
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	/***********************************************/
	
	
	/***********???8?????*******************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7; //PTC6 PTC7
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	/***********************************************/
	
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); //复用
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1); //复用
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //复用
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); //复用
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8); //复用
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8); //复用
	  
	TIM_TimeBaseStructure.TIM_Prescaler=0;  //不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_Period=65535;   //arr
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period=0xffffffff;   //自动重装载
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	
	TIM_EncoderInterfaceConfig(TIM1,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);//12mode 4倍频
	TIM_EncoderInterfaceConfig(TIM5,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM8,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);

	TIM_Cmd(TIM1, ENABLE); 	
	TIM_Cmd(TIM5, ENABLE); 
	TIM_Cmd(TIM8, ENABLE); 


}
//void TIM6_DAC_IRQHandler(void)//5ms进一次中断
//{
////	if(TIM_GetITStatus(TIM6,TIM_IT_Update) != RESET)
////	{
////		circle_count[0]=TIM1_Read_Encoder();
////		circle_count[1]=TIM5_Read_Encoder();
////		circle_count[2]=TIM8_Read_Encoder();
////		LED1_OFF;
////		Get_direction();
////		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);//清空中断标志位
////	}
//}
//***************TIM1**************
int TIM1_Read_Encoder(void)
{
    int value_1;
	if((TIM1->CR1>>4 & 0x01)==0) //DIR==0
    value_1=(int)(-TIM_GetCounter(TIM1));
  else if((TIM1->CR1>>4 & 0x01)==1)//DIR==1
    value_1=(int)TIM_GetCounter(TIM1);
	  TIM_SetCounter(TIM1,0);
	  return value_1;
}


//***************TIM5???????**************
int TIM5_Read_Encoder(void)
{
    int value_1;
	if((TIM5->CR1>>4 & 0x01)==0) //DIR==0
    value_1=(int)(-TIM_GetCounter(TIM5));
  else if((TIM5->CR1>>4 & 0x01)==1)//DIR==1
    value_1=(int)TIM_GetCounter(TIM5);
	  TIM_SetCounter(TIM5,0);
	  return value_1;
}
//***************TIM8???????**************
//void TIM8_Encoder_Write(int data)
//{
//    TIM8->CNT = data;
//}

//*************TIM8**************
int TIM8_Read_Encoder(void)
{
     int value_1;
	if((TIM8->CR1>>4 & 0x01)==0) //DIR==0
    value_1=(int)(-TIM_GetCounter(TIM8));
  else if((TIM8->CR1>>4 & 0x01)==1)//DIR==1
    value_1=(int)TIM_GetCounter(TIM8);
	  return value_1;
}










