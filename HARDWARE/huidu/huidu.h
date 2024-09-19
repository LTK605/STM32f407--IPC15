#ifndef  __HUIDU_H
#define  __HUIDU_H

#include "sys.h"

/*前灰度........左至右	七路*/
#define F1 PFin(0)
#define F2 PFin(1)
#define F3 PFin(2)
#define F4 PFin(3)
#define F5 PFin(4)
#define F6 PFin(5)
#define F7 PFin(6)
/*右灰度........左至右	七路*/
#define R1 PBin(3)
#define R2 PBin(4)
#define R3 PBin(5)
#define R4 PBin(6)
#define R5 PBin(7)
#define R6 PBin(8)
#define R7 PBin(9)
/*后灰度........左至右	七路*/
#define B1 PDin(8)
#define B2 PDin(9)
#define B3 PDin(10)
#define B4 PDin(11)
#define B5 PDin(12)
#define B6 PDin(13)
#define B7 PDin(14)
/*左灰度........左至右	七路*/
#define L1 PCin(3)
#define L2 PFin(8)
#define L3 PFin(9)
#define L4 PFin(10)
#define L5 PFin(11)
#define L6 PFin(12)
#define L7 PFin(13)


void Track_Gyro(void);
void TIM6_Int_Init(void);
void TIM6_DAC_IRQHandler(void);
void date_huidu(void);
void gray_GPIO_init(void);
#endif

