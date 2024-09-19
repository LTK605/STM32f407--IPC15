#ifndef __BEE_H
#define __BEE_H

#include "stm32f4xx.h"
#include "delay.h"


#define Bee_OFF 	  GPIO_ResetBits(Bee_PORT,Bee_PIN);	// 输出低电平	
#define Bee_ON	  GPIO_SetBits(Bee_PORT,Bee_PIN);		// 输出高电平
/*---------------------- 配置宏 ------------------------*/

#define Bee_PIN           GPIO_Pin_14        		 // BEE 引脚      
#define Bee_PORT          GPIOB                     // BEE GPIO端口     
#define Bee_CLK           RCC_AHB1Periph_GPIOB	    // BEE GPIO端口时钟

/*---------------------- 函数声明 ----------------------------*/

void 	Bee_Init(void);	//按键IO口初始化函数

#endif //__BEE_H


