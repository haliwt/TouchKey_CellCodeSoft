
#ifndef __INPUT_H__
#define __INPUT_H__
#include <cms.h>

					

#define		Pout	RC0				//定义RC0口 取名Pout
#define 	Pin0	RA0				//定义RA0口 取名Pin0
#define 	Pin1	RA1				//定义RA1口 取名Pin1

#define	LED_KEY1	RB0  //KEY_TIMER 0x800
#define	LED_KEY2	RB1  //KEY _UP   // 0x80
#define	LED_KEY3	RB2  //KEY_DOWN //0x08
#define	LED_KEY4	RB3  //KEY_RUN  //0X400;
#define	LED_KEY5	RB4   //KEY_SETUP  // 0x40
#define	LED_KEY6	RB5  //KEY_KILL // 0x04
#define	LED_KEY7	RB6  //KEY_POWER  // 0x200

#define		PCout_0 	RC0				//定义RC0口 取名Pout
#define		PCout_1 	RC1				//定义RC0口 取名Pout
#define		PCout_2 	RC2				//定义RC0口 取名Pout
#define		PCout_3 	RC3				//定义RC0口 取名Pout


#define     PDout_0  	RD0	
#define     PDout_1  	RD1	
#define     PDout_2  	RD2	
#define     PDout_3  	RD3	
#define     PDout_4  	RD4	
#define     PDout_5  	RD5	
#define     CONTROL  	RD6	
#define     SEG6  	    RD7



#define 	Pin0	RA0				//定义RA0口 取名Pin0
#define 	Pin1	RA1				//定义RA1口 取名Pin1



//	WPUA = 0B00000000;				//配置上拉情况
//	WPUD = 0B00000000;
	WPUA = 0B00000001;				//配置上拉情况
	WPUB = 0B00000000;
	WPUC = 0B00000000;
	WPUD = 0B00000000;
	
	TRISA = 0B00000011;				//配置IO状态 输入
	TRISB = 0B00000000;     		//GPIO B 输出
	TRISC = 0B00000000;
	TRISD = 0B00000000;
	


#endif