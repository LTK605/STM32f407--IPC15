#ifndef __ENC_H
#define __ENC_H

#include "stm32f4xx.h"
#include "delay.h"

#define Line 256.0f//编码器线数
#define length 1.0f//全向轮到质心的距离
#define R 2.9f//2.9cm,轮半径
#define al 1.73205080756887f
#define Wength (2*PI*R)
void Speed_ENC(void);
void TIM6_Int_Init(void);
int TIM1_Read_Encoder(void);
int TIM5_Read_Encoder(void);
int TIM8_Read_Encoder(void);
void TIM6_DAC_IRQHandler(void);
void Get_direction(void);
#endif //__KEY_H
