#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"

/*---------------------- LED配置宏 ------------------------*/

#define LED1_PIN             GPIO_Pin_8       		 // LED1 引脚  
#define LED2_PIN             GPIO_Pin_14       		 // LED2 引脚  
#define LED3_PIN             GPIO_Pin_15       		 // LED3 引脚 
#define LED1_PORT            GPIOC                 // LED1 GPIO端口  
#define LED2_3_PORT          GPIOD                 // LED2和3 GPIO端口         
#define LED1_CLK             RCC_AHB1Periph_GPIOC	 // LED1 GPIO端口时钟
#define LED2_3_CLK           RCC_AHB1Periph_GPIOD	 // LED1 GPIO端口时钟
/*---------------------- LED控制宏 ------------------------*/
					
#define LED1_OFF 	  GPIO_ResetBits(LED1_PORT,LED1_PIN);	// 输出低电平，关闭LED1	
#define LED1_ON 	  GPIO_SetBits(LED1_PORT,LED1_PIN);		// 输出高电平，点亮LED1	
#define LED2_OFF 	  GPIO_ResetBits(LED2_3_PORT,LED2_PIN);	// 输出低电平，关闭LED1	
#define LED2_ON 	  GPIO_SetBits(LED2_3_PORT,LED2_PIN);		// 输出高电平，点亮LED1	
#define LED3_OFF 	  GPIO_ResetBits(LED2_3_PORT,LED3_PIN);	// 输出低电平，关闭LED1	
#define LED3_ON 	  GPIO_SetBits(LED2_3_PORT,LED3_PIN);		// 输出高电平，点亮LED1
/*---------------------- 函数声明 ----------------------------*/
// 定义状态枚举类型
typedef enum {
    RED, // 红灯状态
    YELLOW, // 黄灯状态
    GREEN // 绿灯状态
} state_t;

// 定义事件枚举类型
typedef enum {
    RED_TIMEOUT, // 红灯超时事件
    YELLOW_TIMEOUT, // 黄灯超时事件
    GREEN_TIMEOUT // 绿灯超时事件
} event_t;

// 定义输出动作枚举类型
typedef enum {
    TURN_ON_RED, // 打开红灯动作
    TURN_ON_YELLOW, // 打开黄灯动作
    TURN_ON_GREEN // 打开绿灯动作
} action_t;

// 定义状态转换结构体类型
typedef struct {
    state_t next_state; // 转换后的下一个状态
    action_t action; // 转换过程中执行的输出动作
} state_transition_t;
void handle_event(event_t event);
void test_state_machine(void);
void state_init(void);

void LED_Init(void);	//LED初始化函数

void Flsh_test(void);
#endif //__LED_H
