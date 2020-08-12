
#ifndef __INPUT_H__
#define __INPUT_H__
#include <cms.h>


typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef int int16_t;
typedef char int8_t;

#define	LED_KEY1	RB0             //KEY_TIMER 0x800
#define	LED_KEY2	RB1             //KEY _UP   // 0x80
#define	LED_KEY3	RB2             //KEY_DOWN //0x08
#define	LED_KEY4	RB3             //KEY_RUN  //0X400;
#define	LED_KEY5	RB4             //KEY_SETUP  // 0x40
#define	LED_KEY6	RB5             //KEY_KILL // 0x04
#define	LED_KEY7	RB6             //KEY_POWER  // 0x200

#define     SEG9        RA3            //¶ÎÑ¡µÄSEG9 
#define     COM0_RUN  	RD0	           //COM0_RUN
#define     COM1_RUN  	RD1	


#define     BKLT_R  	RD6	        //BackLight control 2  ---righ led
#define     BKLT_L      RA4        //BackLigh control 1---left led 

#define     BKLT_POINT     RA7        //BackLigh   radix point
#define     BKLT_TIM  	   RD7        //Timer BackLigt control 







unsigned char keyflag_POWER = 0;
unsigned char keyflag_UP = 0;
unsigned char keyflag_DOWN= 0;
unsigned char keyflag_TIMER = 0;
unsigned char keyflag_RUN = 0;
unsigned char keyflag_KILL = 0;
unsigned char keyflag_SETUP = 0;					



void GPIO_Init(void);


	


#endif