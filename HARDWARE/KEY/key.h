#ifndef __KEY_H
#define __KEY_H

#include "stm32f4xx.h"
#include "delay.h"

#define	KEY1_ON	 1		//��������
#define	KEY1_OFF	 0		//�����ſ�
#define	KEY2_ON	 1		//��������
#define	KEY2_OFF	 0		//�����ſ�
/*---------------------- �������ú� ------------------------*/

#define KEY_PIN           GPIO_Pin_3|GPIO_Pin_4        		 // KEY ����      
#define KEY_PORT          GPIOD                     // KEY GPIO�˿�     
#define KEY_CLK           RCC_AHB1Periph_GPIOD	    // KEY GPIO�˿�ʱ��

/*---------------------- �������� ----------------------------*/

void 	KEY_Init(void);	//����IO�ڳ�ʼ������
u8		KEY_Scan(void);   //����ɨ��

#endif //__KEY_H


