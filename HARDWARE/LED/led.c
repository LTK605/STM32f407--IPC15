
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
// ¶¨Òå×´Ì¬×ª»»±íÎªÒ»¸ö¶þÎ¬Êý×é£¬Êý×éÔªËØÎªstate_transition_t½á¹¹Ìå
// Êý×éµÄÐÐ±íÊ¾µ±Ç°×´Ì¬£¬Êý×éµÄÁÐ±íÊ¾ÊÂ¼þ
state_transition_t state_transition_table[3][3] = {
    // ºìµÆ×´Ì¬×ª»»
    {{RED,  TURN_ON_YELLOW}, {YELLOW, TURN_ON_GREEN}, {GREEN, TURN_ON_RED}},
    // »ÆµÆ×´Ì¬×ª»»
    {{RED,  TURN_ON_YELLOW}, {YELLOW, TURN_ON_GREEN}, {GREEN, TURN_ON_RED}},
    // ÂÌµÆ×´Ì¬×ª»»
    {{RED,  TURN_ON_YELLOW}, {YELLOW, TURN_ON_GREEN}, {GREEN, TURN_ON_RED}}
};
// ¶¨ÒåÒ»¸ö±äÁ¿À´´æ´¢ÏµÍ³µÄµ±Ç°×´Ì¬
state_t current_state;
void state_init(void)
{

 current_state = RED;
}


// ¶¨ÒåÒ»¸öº¯ÊýÀ´´¦ÀíÊÂ¼þ²¢Ö´ÐÐ×´Ì¬×ª»»
void handle_event(event_t event) {
    // ´Ó×´Ì¬×ª»»±íÖÐ»ñÈ¡ÏÂÒ»¸ö×´Ì¬ºÍ¶¯×÷
    state_t next_state = state_transition_table[current_state][event].next_state;
    action_t action = state_transition_table[current_state][event].action;
    // Ö´ÐÐÊä³ö¶¯×÷
    switch (action) {
        case TURN_ON_RED:
           printf("Turn on red light\r\n"); // ´òÓ¡´ò¿ªºìµÆÐÅÏ¢
          OLED_ShowString(1, 3, "Turn on red light");
				 icm20602_test();
				LED2_ON;
					LED1_OFF;LED3_OFF;
            break;
        case TURN_ON_YELLOW:
           printf("Turn on yellow light\r\n"); // ´òÓ¡´ò¿ª»ÆµÆÐÅÏ¢
				OLED_ShowString(1, 3, "Turn on yellow light");
				bmp280_test();
				bmi088_test();
				Bee_OFF;
				LED3_ON
        LED1_OFF;LED2_OFF
            break;
        case TURN_ON_GREEN:
            printf("Turn on green light\r\n"); // ´òÓ¡´ò¿ªÂÌµÆÐÅÏ¢
				OLED_ShowString(1, 3, "Turn on green light");
				hmc_read_xyz();
				//Flsh_test();
				Bee_OFF;
				LED1_ON;LED2_OFF;LED3_OFF;
            break;
        default:
						LED1_OFF;LED2_OFF;LED3_OFF
            printf("Invalid action\r\n"); // ´òÓ¡ÎÞÐ§¶¯×÷ÐÅÏ¢
				OLED_ShowString(1, 3, "Invalid action");
            break;
    }

    // ¸üÐÂµ±Ç°×´Ì¬
    current_state = next_state;
}

// ¶¨ÒåÒ»¸öº¯ÊýÀ´Ä£ÄâÒ»Ð©ÊÂ¼þ²¢²âÊÔ×´Ì¬»ú
void test_state_machine() {
    // Ä£ÄâÒ»Ð©ÊÂ¼þ²¢´¦ÀíËüÃÇ
    handle_event(RED_TIMEOUT); // ´ò¿ª»ÆµÆ²¢½øÈë»ÆµÆ×´Ì¬
	 delay_ms(350);
    handle_event(YELLOW_TIMEOUT); // ´ò¿ªÂÌµÆ²¢½øÈëÂÌµÆ×´Ì¬
	 delay_ms(350);
    handle_event(GREEN_TIMEOUT); // ´ò¿ª»ÆµÆ²¢½øÈë»ÆµÆ×´Ì¬
	 delay_ms(350);
	//handle_event(ddd); // ´ò¿ª»ÆµÆ²¢½øÈë»ÆµÆ×´Ì¬
}





// º¯Êý£ºLED IO¿Ú³õÊ¼»¯
//



void LED_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; //¶¨Òå½á¹¹Ìå
	RCC_AHB1PeriphClockCmd ( LED1_CLK, ENABLE); 	//³õÊ¼»¯GPIOGÊ±ÖÓ	
	RCC_AHB1PeriphClockCmd ( LED2_3_CLK, ENABLE); 	//³õÊ¼»¯GPIOGÊ±ÖÓ	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   //Êä³öÄ£Ê½
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	//ÉÏÀ­
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz; //ËÙ¶ÈÑ¡Ô
	//³õÊ¼»¯ LED1 Òý½Å
	GPIO_InitStructure.GPIO_Pin = LED1_PIN;	 
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   //Êä³öÄ£Ê½
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	//ÉÏÀ­
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //ËÙ¶ÈÑ¡?
	GPIO_InitStructure.GPIO_Pin = LED2_PIN|LED3_PIN;	 
	GPIO_Init(LED2_3_PORT, &GPIO_InitStructure);	
  LED1_OFF;
	LED2_OFF;
	LED3_OFF;
}
void Flsh_test(void)
{


}
