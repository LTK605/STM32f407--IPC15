/*
 * @Author: IPC15-李天凯
 * @Date: 2023-11-1 20:55:18
 * @LastEditTime: 2023-11-1 20:55:18
 * @FilePath: \USER\main.c
 * @custom_steing_obkoro1:  Read only, do not modify place!!!
 */
#include "sys.h" //The <lwoopc.h> is already included here
#include "delay.h"
#include "usart.h"
//#include "usart2.h"	
#include "led.h"
#include "stdio.h"
#include "math.h"
#include "OLED.h"
#include "bee.h"  
#include "w25q128.h"
#include "HMC5883L.h"
#include "math.h"
#include "icm20602.h"
#include "bmi088.h"
#include "bmp280.h"
//要写入到W25Q16的字符串数组
const u8 TEXT_Buffer[]={"Explorer STM32F4 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)	
extern u16 W25QXX_TYPE;					//定义W25QXX芯片型号	
int main(void)
{  
	  int i=0;
//	u8 datatemp[SIZE];	
	delay_init(168);
	uart_init(9600);
	LED_Init();
	OLED_Init();
	delay_ms(20);
	i2c_init();
  Bee_Init();
  hmc_init();
	delay_ms(20);
	icm20602_init();
	delay_ms(10);
	BMI088_INIT();
	delay_ms(10);
	W25QXX_Init();
	delay_ms(10);
	for(i=0;i<=400;i++)
	
	{
	
	Bee_ON;
		delay_ms(1);
		Bee_OFF;
		delay_ms(1);
	}

int c=0;
	bmp280Init();
	//hmc_read_xyz();
    while (1)
   {   
//   icm20602_test();
//		 delay_ms(1000);
//		 printf("hmc:%f\r\n",angle);
test_state_machine();//红绿灯状态机测试
	
   }
		
}

