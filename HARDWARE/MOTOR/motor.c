/*
 * @Author: Dragon
 * @Date: 2022-08-27 20:56:30
 * @LastEditTime: 2022-08-30 11:57:59
 * @FilePath: \USERf:\WorkSpace\Engineering training\Intelligent handling vehicle\code\Stepper motor S control\HARDWARE\MOTOR\motor.c
 * @custom_steing_obkoro1:  Read only, do not modify place!!!
 */
#include "motor.h"
#include "stdio.h"
#include "math.h"
#include "usart.h"
#include "stm32f4xx_it.h"
extern WheelRunningData wheel_RF;
extern WheelRunningData wheel_RB;
extern WheelRunningData wheel_LF;
extern WheelRunningData wheel_LB;
extern VehicleRunningData vehicle;
extern float c[103];
float32_t v_X=0;	
float32_t v_Y=0;	
uint32_t time_weith[5];
/*Interrupt service function declaration*/
void IRQ_Wheel_RF(void);
void IRQ_Wheel_RB(void);
void IRQ_Wheel_LF(void);
void IRQ_Wheel_LB(void);

void StepperMotor_Init(void)
{
    /**
     * The four-way stepping motor uses the output comparison mode of TIM2 to complete the control
     *
     * TIM2_ch1 PA0 LF   TIM2_ch2 PA1 RF
     *
     * TIM2_ch3 PA2 LB   TIM2_ch4 PA3 RB
     *
     */
    // Initialize related structure definition
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // Relevant clock enable
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    // Alternate function statement
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM2);

    // Pin configuration statement
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Timer basic configuration statement
    TIM_TimeBaseStructure.TIM_Period = ARR;
    TIM_TimeBaseStructure.TIM_Prescaler = PSC;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    //比较 模式  输出比较翻转触发模式(当计数值与比较/捕获寄存器值相同时，翻转输出引脚的电平)
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		//TIM_OCInitStructure.TIM_Pulse =500-1;

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable );

    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable );

    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable );

    TIM_OC4Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Disable );

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE);
		TIM_ITConfig(TIM2,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4,ENABLE);
}
//void control(WheelRunningData*wheel)
//{
//if(wheel->run_state!= STOP)//在STOP情况下继续
//		return;
//	if(wheel->sum_step < 0)
//	{
//	wheel->dir=CCW;//ccw反转
//		//DIR IO
//		wheel->sum_step=-wheel_LF.sum_step;
//	}
//	else
//	{
//		wheel->dir=CW;//cw正转
//		//DIR IO
//		wheel->sum_step=-wheel_LF.sum_step;
//	}
//	if(wheel->sum_step == 1)
//	{
//	wheel->accel_step=1;
//	wheel->run_state= DECEL;//减速
//	wheel->step_delay=100;
//	}
//	else if()
//}
void Relative_Move( float32_t distance_X,  float32_t distance_Y, float32_t distance_W,uint32_t v_W,uint32_t v_accel, uint32_t v_decel, float32_t v_speed)
{    
//	  float32_t pSinVal;
//    float32_t pCosVal;	
//	  arm_sin_cos_f32(distance_W, &pSinVal, &pCosVal);
//	  v_X=   v_speed * pCosVal;
//	  v_Y=   v_speed * pSinVal;
    float32_t displacement=__sqrtf(distance_X*distance_X+distance_Y*distance_Y);
	  float32_t t= v_speed/displacement;
	  v_X=   v_speed * distance_X/displacement;
    v_Y=   v_speed * distance_Y/displacement;
	  v_W=(uint32_t)(distance_W/t);
	  wheel_LF.max_speed= (int32_t)((v_X - v_Y - v_W * (WIDTH + LENGTH)));
    wheel_RF.max_speed = (int32_t)((v_X + v_Y + v_W * (WIDTH + LENGTH)));
    wheel_LB.max_speed = (int32_t)((v_X + v_Y - v_W * (WIDTH + LENGTH)));
    wheel_RF.max_speed= (int32_t)((v_X - v_Y + v_W* (WIDTH + LENGTH)));
    /*From the vehicle motion information to the number of turns each wheel needs to turn*/
    wheel_LF.sum_step = (int32_t)((distance_X - distance_Y - distance_W * (WIDTH + LENGTH)) / L);
    wheel_RF.sum_step = (int32_t)((distance_X + distance_Y + distance_W * (WIDTH + LENGTH)) / L);
    wheel_LB.sum_step = (int32_t)((distance_X + distance_Y - distance_W * (WIDTH + LENGTH)) / L);
    wheel_RF.sum_step = (int32_t)((distance_X - distance_Y + distance_W * (WIDTH + LENGTH)) / L);
	
    StepperMotor_LF_control(wheel_LF.sum_step, v_accel, v_decel, wheel_LF.max_speed);
    StepperMotor_LB_control(wheel_LB.sum_step, v_accel, v_decel, wheel_RF.max_speed);
    StepperMotor_RF_control(wheel_RF.sum_step, v_accel, v_decel, wheel_LB.max_speed);
    StepperMotor_RB_control(wheel_RB.sum_step, v_accel, v_decel, wheel_RF.max_speed);
}

/*左前*/
void StepperMotor_LF_control(int32_t sum_step, uint32_t accel, uint32_t decel, int32_t speed)
{
	if(wheel_LF.run_state!= STOP)//在STOP情况下继续
		return;
	if(sum_step < 0)
	{
	wheel_LF.dir=CCW;//ccw反转
		//DIR IO
		sum_step=-sum_step;
		speed=-speed;
	}
	else
	{
		wheel_LF.dir=CW;//cw正转
		//DIR IO
	}
  if(sum_step!=0)
	{
		wheel_LF.min_delay=(uint32_t) (L*T1_FREQ/speed);//95577
		wheel_LF.step_delay=(uint32_t)((T1_FREQ  * __sqrtf(2*L / accel))); /* delate */
    /*暂时认为加速度和减速度相等*/
    decel = accel;
    wheel_LF.accel_step = (uint32_t)((speed * speed) / (2 * accel * L));
	
		if(wheel_LF.accel_step==0)
		{
		wheel_LF.accel_step=1;
		
		}
			
    wheel_LF.accel_max_step = (uint32_t)(sum_step / 2);
		
    if (wheel_LF.accel_step >= wheel_LF.accel_max_step)
    {
        wheel_LF.decel_step = (uint32_t)(sum_step - wheel_LF.accel_max_step);
    }
	
    else
    {
        wheel_LF.decel_step =  wheel_LF.accel_step;
    }
		if(wheel_LF.decel_step==0)
		{
		wheel_LF.decel_step=1;
		}
		    wheel_LF.decel_start = wheel_LF.sum_step- wheel_LF.decel_step;
//printf("%d", wheel_LF.accel_step);//,wheel_LF.accel_max_step,wheel_LF.decel_step);
//	Usart_SendHalfWord(USART1, 0x0d0a);		
		  if( wheel_LF.step_delay <=  wheel_LF.min_delay)                 /* Èç¹ûÒ»¿ªÊ¼c0µÄËÙ¶È±ÈÔÈËÙ¶ÎËÙ¶È»¹´ó£¬¾Í²»ÐèÒª½øÐÐ¼ÓËÙÔË¶¯£¬Ö±½Ó½øÈëÔÈËÙ */
        {
             wheel_LF.step_delay =  wheel_LF.min_delay;
             wheel_LF.run_state = RUN;
        }
        else  
        {
             wheel_LF.run_state = ACCEL;
        }
        
}
//
}
/*左后*/
void StepperMotor_LB_control(int32_t sum_step, uint32_t accel, uint32_t decel, int32_t speed)
{
    if(wheel_LB.run_state!= STOP)//在STOP情况下继续
		return;
	  if(sum_step < 0)
	 {
	   wheel_LB.dir=CCW;//ccw反转
		   //DIR IO
		 sum_step=sum_step;
	}
	  else
   	{
		   wheel_LB.dir=CW;//cw正转
		//DIR IO
		   sum_step=-sum_step;
  	}
  if(sum_step!=0)
	{
		wheel_LB.min_delay=(uint32_t) (L*T1_FREQ/speed);
		wheel_LB.step_delay=(uint32_t)((T1_FREQ * 0.69 * 100 * __sqrtf(2*L / accel))/100); /* c0 */
    /*暂时认为加速度和减速度相等*/
    decel = accel;
    wheel_LB.accel_step = (uint32_t)((speed * speed) / (2 * accel * L));
		if(wheel_LB.accel_step==0)
		{
		wheel_LB.accel_step=1;
		
		}
			
    wheel_LB.accel_max_step = (uint32_t)(sum_step / 2);
		
    if (wheel_LB.accel_step >= wheel_LF.accel_max_step)
    {
        wheel_LB.decel_step = (uint32_t)(sum_step - wheel_LB.accel_max_step);
    }
	
       else
    {
        wheel_LB.decel_step =  wheel_LB.accel_step;
    }
		if(wheel_LB.decel_step==0)
		{
		wheel_LB.decel_step=1;
		}
		 wheel_LB.decel_start = wheel_LB.sum_step- wheel_LB.decel_step; 
		  if( wheel_LB.step_delay <=  wheel_LB.min_delay)                 /* Èç¹ûÒ»¿ªÊ¼c0µÄËÙ¶È±ÈÔÈËÙ¶ÎËÙ¶È»¹´ó£¬¾Í²»ÐèÒª½øÐÐ¼ÓËÙÔË¶¯£¬Ö±½Ó½øÈëÔÈËÙ */
        {
             wheel_LB.step_delay =  wheel_LB.min_delay;
             wheel_LB.run_state = RUN;
        }
        else  
        {
             wheel_LB.run_state = ACCEL;
        }
         //wheel_LB.accel_count = 0;                                  /* ¸´Î»¼Ó¼õËÙ¼ÆÊýÖµ */
}
	
}

/*右前*/
void StepperMotor_RF_control(int32_t sum_step, uint32_t accel, uint32_t decel, int32_t speed)
{
   	if(wheel_RF.run_state!= STOP)//在STOP情况下继续
		return;
	if(sum_step < 0)
	{
	wheel_RF.dir=CCW;//ccw反转
		//DIR IO
		sum_step=-sum_step;
		speed=-speed;
	}
	else
	{
		wheel_RF.dir=CW;//cw正转
		//DIR IO
	}
  if(sum_step!=0)
	{
		wheel_RF.min_delay=(uint32_t) (L*T1_FREQ/speed);
		wheel_RF.step_delay=(uint32_t)((T1_FREQ * 0.69 * 100 * __sqrtf(2*L / accel))/100); /* c0 */
    /*暂时认为加速度和减速度相等*/
    decel = accel;
    wheel_RF.accel_step = (uint32_t)((speed * speed) / (2 * accel * L));
		if(wheel_RF.accel_step==0)
		{
		wheel_RF.accel_step=1;
		
		}
			
    wheel_RF.accel_max_step = (uint32_t)(sum_step / 2);
		
    if (wheel_RF.accel_step >= wheel_RF.accel_max_step)
    {
        wheel_RF.decel_step = (uint32_t)(sum_step - wheel_RF.accel_max_step);
    }
	
        else
    {
        wheel_RF.decel_step =  wheel_RF.accel_step;
    }
		if(wheel_RF.decel_step==0)
		{
		wheel_RF.decel_step=1;
		}
		    wheel_RF.decel_start = wheel_RF.sum_step- wheel_LF.decel_step; 
		  if( wheel_RF.step_delay <=  wheel_RF.min_delay)                 /* Èç¹ûÒ»¿ªÊ¼c0µÄËÙ¶È±ÈÔÈËÙ¶ÎËÙ¶È»¹´ó£¬¾Í²»ÐèÒª½øÐÐ¼ÓËÙÔË¶¯£¬Ö±½Ó½øÈëÔÈËÙ */
        {
             wheel_RF.step_delay =  wheel_RF.min_delay;
             wheel_RF.run_state = RUN;
        }
        else  
        {
             wheel_RF.run_state = ACCEL;
        }
         wheel_RF.accel_count = 0;                                  /* ¸´Î»¼Ó¼õËÙ¼ÆÊýÖµ */
}
}

/*右后*/
void StepperMotor_RB_control(int32_t sum_step, uint32_t accel, uint32_t decel, int32_t speed)
{
   if(wheel_RB.run_state!= STOP)//在STOP情况下继续
		return;
	if(sum_step < 0)
	{
	wheel_RB.dir=CCW;//ccw反转
		//DIR IO
		sum_step=-sum_step;
		speed=-speed;
	}
	else
	{
		wheel_RB.dir=CW;//cw正转
		//DIR IO
	}
  if(sum_step!=0)
	{
		wheel_RB.min_delay=(int32_t) (int32_t)(L*T1_FREQ/speed);
		wheel_RB.step_delay=(int32_t)((T1_FREQ * 0.69 * 100 * __sqrtf(2*L / accel))/100); /* c0 */
    /*暂时认为加速度和减速度相等*/
    decel = accel;
    wheel_RB.accel_step = (uint32_t)((speed * speed) / (2 * accel * L));
		if(wheel_RB.accel_step==0)
		{
		wheel_RB.accel_step=1;
		
		}
			
    wheel_RB.accel_max_step = (uint32_t)(sum_step / 2);
		
    if (wheel_RB.accel_step >= wheel_RB.accel_max_step)
    {
        wheel_RB.decel_step = (uint32_t)(sum_step - wheel_RB.accel_max_step);
    }
	
       else
    {
        wheel_RB.decel_step =  wheel_RB.accel_step;
    }
		if(wheel_RB.decel_step==0)
		{
		wheel_RB.decel_step=1;
		}
		    wheel_RB.decel_start = wheel_RB.sum_step- wheel_LF.decel_step; 
		
		  if( wheel_RB.step_delay <=  wheel_RB.min_delay)                 /* Èç¹ûÒ»¿ªÊ¼c0µÄËÙ¶È±ÈÔÈËÙ¶ÎËÙ¶È»¹´ó£¬¾Í²»ÐèÒª½øÐÐ¼ÓËÙÔË¶¯£¬Ö±½Ó½øÈëÔÈËÙ */
        {
             wheel_RB.step_delay =  wheel_RB.min_delay;
             wheel_RB.run_state = RUN;
        }
        else  
        {
             wheel_RB.run_state = ACCEL;
        }
         wheel_RB.accel_count = 0;                                  /* ¸´Î»¼Ó¼õËÙ¼ÆÊýÖµ */
}
}
int i=1000;
/******************************************************************/
void TIM2_IRQHandler(void)
{
    if ((TIM_GetITStatus(TIM2, TIM_IT_CC1))== SET)
    {
			TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);
        // do something
        IRQ_Wheel_LF();
//				printf("%d", wheel_LF.accel_count);//,wheel_LF.accel_max_step,wheel_LF.decel_step);
//	Usart_SendHalfWord(USART1, 0x0d0a);		
			time_weith[0]=TIM_GetCapture1(TIM2);
			TIM2->CNT=0;
			TIM_SetCompare1(TIM2,wheel_LF.step_delay/2);
      // printf("%d",wheel_LF.step_delay/2);
			//Usart_SendHalfWord(USART1, 0x0d0a);
		
//			TIM2->CNT = 0X00;
//			i = TIM_GetCapture1(TIM2);
//			if(i ==1000)
//			{
//				i =2000;
//			}
//			else
//			{
//				i=1000;
//			}
//        TIM2->CCR1 =i;
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC2)!=DISABLE)
    {
        // do something
        IRQ_Wheel_RF();

        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC3)!=DISABLE)
    {
        // do something
        IRQ_Wheel_RB();
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);
    }

    if (TIM_GetITStatus(TIM2, TIM_IT_CC4)!=DISABLE)
    {
        // do something
        IRQ_Wheel_RB();
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);
    }
}

void IRQ_Wheel_RF(void)
{
    __IO static uint8_t i = 2;
    if (i == 2)
    {
        Pulse_Control(&wheel_RF);
        i = 0;
    }
    i++;
}
void IRQ_Wheel_RB(void)
{
    __IO static uint8_t i = 2;
    if (i == 2)
    {
        Pulse_Control(&wheel_RB);
        i = 0;
    }
    i++;
}

void IRQ_Wheel_LF(void)
{
    __IO static uint8_t i = 2;
    if (i == 2)
    {
        Pulse_Control(&wheel_LF);
        i = 0;
    }
    i++;
}

void IRQ_Wheel_LB(void)
{
    __IO static uint8_t i = 2;
    if (i == 2)
    {
        Pulse_Control(&wheel_LB);
        i = 0;
    }
    i++;
}
/*Analyze with FSM*/
void Pulse_Control(WheelRunningData *Wheel)
{
    uint32_t  new_step_delay;
	  uint32_t  rest=0;
	  int32_t lastpulse_width;
    switch (Wheel->run_state)
    {
    case STOP:
        Wheel->step = 0x00;
	      Wheel->accel_count = 0;                                  /* ¸´Î»¼Ó¼õËÙ¼ÆÊýÖµ */
        Wheel->sum_step = 0x00;
        Wheel->accel_step = 0x00;
        Wheel->accel_max_step = 0x00;
        Wheel->decel_step = 0x00;
        /*Close timer function*/
        break;
    case ACCEL:
        /* code */
		    Wheel->accel_count++; 
	     	Wheel->step++;
       new_step_delay = Wheel->step_delay - (((2 *Wheel->step_delay) + rest)/(4 * Wheel->accel_count + 1));/* ¼ÆËãÐÂ(ÏÂ)Ò»²½Âö³åÖÜÆÚ(Ê±¼ä¼ä¸ô) */
        rest = ((2 * Wheel->step_delay)+rest)%(4 * Wheel->accel_count + 1);    
		 printf("%d",new_step_delay);
			Usart_SendHalfWord(USART1, 0x0d0a);		/* ¼ÆËãÓàÊý£¬ÏÂ´Î¼ÆËã²¹ÉÏÓàÊý£¬¼õÉÙÎó²î */
        
		    if(Wheel->step >= Wheel->decel_start)         /* ¼ì²éÊÇ·ñµ½ÁËÐèÒª¼õËÙµÄ²½Êý */
                {
                    Wheel->accel_count = Wheel->decel_step;    /* ¼ÓËÙ¼ÆÊýÖµÎª¼õËÙ½×¶Î¼ÆÊýÖµµÄ³õÊ¼Öµ */
                    Wheel->run_state = DECEL;                /* ÏÂ¸öÂö³å½øÈë¼õËÙ½×¶Î */
                }
        else if (new_step_delay <= Wheel->min_delay)
        {
					lastpulse_width= new_step_delay;
					 new_step_delay=Wheel->min_delay;
            rest = 0;
            Wheel->run_state = RUN;
        }
        break;
    case RUN:
        /* code */
		    Wheel->step++;
        new_step_delay = Wheel->min_delay;
//  	 printf("%d", new_step_delay);//,wheel_LF.accel_max_step,wheel_LF.decel_step);
//	   Usart_SendHalfWord(USART1, 0x0d0a);		
//		 printf("%d",new_step_delay);
//			Usart_SendHalfWord(USART1, 0x0d0a);
        if (Wheel->step >= Wheel->decel_start)
        {
            new_step_delay = lastpulse_width;
						Wheel->accel_count=Wheel->decel_start;
            Wheel->run_state = DECEL;
        }
        break;
    case DECEL:
        /* code */
		Wheel->step++;
		Wheel->accel_count--;
		new_step_delay = Wheel->step_delay - (((2 *  Wheel->step_delay) + rest)/(4 *  Wheel->accel_count + 1));  /* ¼ÆËãÐÂ(ÏÂ)Ò»²½Âö³åÖÜÆÚ(Ê±¼ä¼ä¸ô) */
    rest = ((2 *  Wheel->step_delay)+rest)%(4 *  Wheel->accel_count + 1);   
//printf("%d",Wheel->accel_count);
//	Usart_SendHalfWord(USART1, 0x0d0a);		/* ¼ÆËãÓàÊý£¬ÏÂ´Î¼ÆËã²¹ÉÏÓàÊý£¬¼õÉÙÎó²î */
        if (Wheel->accel_count <= 0)
        {
            Wheel->run_state = STOP;
        }
        break;
    default:
        break;
    }
   Wheel->step_delay=new_step_delay;
	 
    // A new parameter is passed into the timer register
}
