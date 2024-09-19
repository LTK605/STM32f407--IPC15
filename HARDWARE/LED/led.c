
#include "led.h"  
#include "delay.h"
#include "usart.h"
#include "OLED.h"
#include "bee.h"  
#include "w25q128.h"
#include "HMC5883L.h"
#include "bmp280.h"
#include "icm20602.h"
#include "bmi088.h"
// ����״̬ת����Ϊһ����ά���飬����Ԫ��Ϊstate_transition_t�ṹ��
// ������б�ʾ��ǰ״̬��������б�ʾ�¼�
state_transition_t state_transition_table[3][3] = {
    // ���״̬ת��
    {{RED,  TURN_ON_YELLOW}, {YELLOW, TURN_ON_GREEN}, {GREEN, TURN_ON_RED}},
    // �Ƶ�״̬ת��
    {{RED,  TURN_ON_YELLOW}, {YELLOW, TURN_ON_GREEN}, {GREEN, TURN_ON_RED}},
    // �̵�״̬ת��
    {{RED,  TURN_ON_YELLOW}, {YELLOW, TURN_ON_GREEN}, {GREEN, TURN_ON_RED}}
};
// ����һ���������洢ϵͳ�ĵ�ǰ״̬
state_t current_state;
void state_init(void)
{

 current_state = RED;
}


// ����һ�������������¼���ִ��״̬ת��
void handle_event(event_t event) {
    // ��״̬ת�����л�ȡ��һ��״̬�Ͷ���
    state_t next_state = state_transition_table[current_state][event].next_state;
    action_t action = state_transition_table[current_state][event].action;
    // ִ���������
    switch (action) {
        case TURN_ON_RED:
           printf("Turn on red light\r\n"); // ��ӡ�򿪺����Ϣ
          OLED_ShowString(1, 3, "Turn on red light");
				 icm20602_test();
				LED2_ON;
					LED1_OFF;LED3_OFF;
            break;
        case TURN_ON_YELLOW:
           printf("Turn on yellow light\r\n"); // ��ӡ�򿪻Ƶ���Ϣ
				OLED_ShowString(1, 3, "Turn on yellow light");
				bmp280_test();
				bmi088_test();
				Bee_OFF;
				LED3_ON
        LED1_OFF;LED2_OFF
            break;
        case TURN_ON_GREEN:
            printf("Turn on green light\r\n"); // ��ӡ���̵���Ϣ
				OLED_ShowString(1, 3, "Turn on green light");
				hmc_read_xyz();
				//Flsh_test();
				Bee_OFF;
				LED1_ON;LED2_OFF;LED3_OFF;
            break;
        default:
						LED1_OFF;LED2_OFF;LED3_OFF
            printf("Invalid action\r\n"); // ��ӡ��Ч������Ϣ
				OLED_ShowString(1, 3, "Invalid action");
            break;
    }

    // ���µ�ǰ״̬
    current_state = next_state;
}

// ����һ��������ģ��һЩ�¼�������״̬��
void test_state_machine() {
    // ģ��һЩ�¼�����������
    handle_event(RED_TIMEOUT); // �򿪻ƵƲ�����Ƶ�״̬
	 delay_ms(350);
    handle_event(YELLOW_TIMEOUT); // ���̵Ʋ������̵�״̬
	 delay_ms(350);
    handle_event(GREEN_TIMEOUT); // �򿪻ƵƲ�����Ƶ�״̬
	 delay_ms(350);
	//handle_event(ddd); // �򿪻ƵƲ�����Ƶ�״̬
}





// ������LED IO�ڳ�ʼ��
//



void LED_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ��
	RCC_AHB1PeriphClockCmd ( LED1_CLK, ENABLE); 	//��ʼ��GPIOGʱ��	
	RCC_AHB1PeriphClockCmd ( LED2_3_CLK, ENABLE); 	//��ʼ��GPIOGʱ��	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   //���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	//����
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz; //�ٶ�ѡ�
	//��ʼ�� LED1 ����
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;	 
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   //���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //�ٶ�ѡ?
	GPIO_InitStructure.GPIO_Pin = LED2_PIN|LED3_PIN;	 
	GPIO_Init(LED2_3_PORT, &GPIO_InitStructure);	
  LED1_OFF;
	LED2_OFF;
	LED3_OFF;
}
void Flsh_test(void)
{


}
