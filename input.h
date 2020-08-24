
#ifndef __INPUT_H__
#define __INPUT_H__
#include <cms.h>

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef int int16_t;
typedef char int8_t;

struct _KEY_STRUCT_
{
    
    uint8_t SetupOn :1;   //??????? -?????  ??? ???????????????
    uint8_t TimerOn : 1 ;    //????? --???
    uint8_t KillOn : 1;      //???? ---???
    uint8_t PowerOn : 1;     //????----???
    uint8_t RunOn: 1;       //????----???
    uint8_t windMask :1;
    uint8_t BackLed_On: 3 ;
    
    uint8_t TimeBaseUint;    //
    uint8_t TimeHour ;
    uint8_t TimeMinute;
    uint8_t TimeDecadeHour;
    uint8_t windLevel;
    uint8_t TimeSetUp;
    uint8_t TimeSetDown;
    uint8_t SendSwitchData ;
    uint8_t SendWindData ;
    uint8_t SendBCCdata;
    uint8_t TimerTim; 
    uint8_t RunStateNumber;
};

struct  _KEY_STRUCT_   keystr;
enum wind{minWind,smallWind,middleWind,moreWind,maxWind};








#define	LED_KEY1	RB0             //KEY_TIMER 0x800  
#define	LED_KEY2	RB1             //KEY _UP   // 0x80
#define	LED_KEY3	RB2             //KEY_DOWN //0x08
#define	LED_KEY4	RB3             //KEY_RUN  //0X400;
#define	LED_KEY5	RB4             //KEY_SETUP  // 0x40
#define	LED_KEY6	RB5             //KEY_KILL // 0x04
#define	LED_KEY7	RB6             //KEY_POWER  // 0x200

#define SEG9_1      RA3            //SEG9 - 1
#define SEG9_2      RA4            //SEG9 _ 2



#define  BKLT_R   	RD1	        //BackLigh control 1---right led
#define  BKLT_L     RA7       //BackLigh control 1---left led

#define IR_RE_DATA     RD7        //IR_Receive DATA Input GPIO <------"number dot"

#define BKLT_TIM  	   RD6        //Timer BackLigt control AND clock backlight 







#define    keyflag_POWER   1
#define    keyflag_RUN     2
#define    keyflag_KILL    3
#define    keyflag_SETUP   4
#define    keyflag_UP      5
#define    keyflag_DOWN    6
#define    keyflag_TIMER   7					



void GPIO_Init(void);
void IR_ReadData(void);

	


#endif