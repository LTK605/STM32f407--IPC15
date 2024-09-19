/***
	***************************************************************************
	*	@file  	bee.c
	*	@version V1.0.0
	*	@brief   �����ӿ���غ���
   ***************************************************************************
   *  @description
	*
	*  ��ʼ�����ţ�����Ϊ�������롢�ٶȵȼ�2M��
	* 	
	***************************************************************************
***/

#include "bee.h"  

// ����������IO�ڳ�ʼ��
//
void Bee_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ��
	RCC_AHB1PeriphClockCmd ( Bee_CLK, ENABLE); 	//��ʼ��GPIOʱ��	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   //���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	//����
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz; //�ٶ�ѡ?
	GPIO_InitStructure.GPIO_Pin = Bee_PIN;	 
	GPIO_Init(Bee_PORT, &GPIO_InitStructure);	

}



