#ifndef __BEE_H
#define __BEE_H

#include "stm32f4xx.h"
#include "delay.h"


#define Bee_OFF 	  GPIO_ResetBits(Bee_PORT,Bee_PIN);	// ����͵�ƽ	
#define Bee_ON	  GPIO_SetBits(Bee_PORT,Bee_PIN);		// ����ߵ�ƽ
/*---------------------- ���ú� ------------------------*/

#define Bee_PIN           GPIO_Pin_14        		 // BEE ����      
#define Bee_PORT          GPIOB                     // BEE GPIO�˿�     
#define Bee_CLK           RCC_AHB1Periph_GPIOB	    // BEE GPIO�˿�ʱ��

/*---------------------- �������� ----------------------------*/

void 	Bee_Init(void);	//����IO�ڳ�ʼ������

#endif //__BEE_H


