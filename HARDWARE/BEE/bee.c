/***
	***************************************************************************
	*	@file  	bee.c
	*	@version V1.0.0
	*	@brief   按键接口相关函数
   ***************************************************************************
   *  @description
	*
	*  初始化引脚，配置为上拉输入、速度等级2M。
	* 	
	***************************************************************************
***/

#include "bee.h"  

// 函数：按键IO口初始化
//
void Bee_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体
	RCC_AHB1PeriphClockCmd ( Bee_CLK, ENABLE); 	//初始化GPIO时钟	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   //输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	//上拉
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz; //速度选?
	GPIO_InitStructure.GPIO_Pin = Bee_PIN;	 
	GPIO_Init(Bee_PORT, &GPIO_InitStructure);	

}



