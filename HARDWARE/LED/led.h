#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"

/*---------------------- LED���ú� ------------------------*/

#define LED1_PIN             GPIO_Pin_8       		 // LED1 ����  
#define LED2_PIN             GPIO_Pin_14       		 // LED2 ����  
#define LED3_PIN             GPIO_Pin_15       		 // LED3 ���� 
#define LED1_PORT            GPIOC                 // LED1 GPIO�˿�  
#define LED2_3_PORT          GPIOD                 // LED2��3 GPIO�˿�         
#define LED1_CLK             RCC_AHB1Periph_GPIOC	 // LED1 GPIO�˿�ʱ��
#define LED2_3_CLK           RCC_AHB1Periph_GPIOD	 // LED1 GPIO�˿�ʱ��
/*---------------------- LED���ƺ� ------------------------*/
					
#define LED1_OFF 	  GPIO_ResetBits(LED1_PORT,LED1_PIN);	// ����͵�ƽ���ر�LED1	
#define LED1_ON 	  GPIO_SetBits(LED1_PORT,LED1_PIN);		// ����ߵ�ƽ������LED1	
#define LED2_OFF 	  GPIO_ResetBits(LED2_3_PORT,LED2_PIN);	// ����͵�ƽ���ر�LED1	
#define LED2_ON 	  GPIO_SetBits(LED2_3_PORT,LED2_PIN);		// ����ߵ�ƽ������LED1	
#define LED3_OFF 	  GPIO_ResetBits(LED2_3_PORT,LED3_PIN);	// ����͵�ƽ���ر�LED1	
#define LED3_ON 	  GPIO_SetBits(LED2_3_PORT,LED3_PIN);		// ����ߵ�ƽ������LED1
/*---------------------- �������� ----------------------------*/
// ����״̬ö������
typedef enum {
    RED, // ���״̬
    YELLOW, // �Ƶ�״̬
    GREEN // �̵�״̬
} state_t;

// �����¼�ö������
typedef enum {
    RED_TIMEOUT, // ��Ƴ�ʱ�¼�
    YELLOW_TIMEOUT, // �ƵƳ�ʱ�¼�
    GREEN_TIMEOUT // �̵Ƴ�ʱ�¼�
} event_t;

// �����������ö������
typedef enum {
    TURN_ON_RED, // �򿪺�ƶ���
    TURN_ON_YELLOW, // �򿪻Ƶƶ���
    TURN_ON_GREEN // ���̵ƶ���
} action_t;

// ����״̬ת���ṹ������
typedef struct {
    state_t next_state; // ת�������һ��״̬
    action_t action; // ת��������ִ�е��������
} state_transition_t;
void handle_event(event_t event);
void test_state_machine(void);
void state_init(void);

void LED_Init(void);	//LED��ʼ������

void Flsh_test(void);
#endif //__LED_H
