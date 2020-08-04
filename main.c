/*-------------------------------------------

�������ƣ�CMS89FT628���ʾ������
���ڰ汾��2018/6/15 <V1.1>

��ע��

*�������� ��΢�뵼�����޹�˾ &Ӧ��֧�ֲ�& ��д����
*��˾��ַ www.mcu.com.cn
*����֧��QQ 3001082102  �޹�
            3001085706	�¹�
-------------------------------------------*/
#include <cms.h>
#include "Touch_Kscan_Library.h"

#define	LED_KEY1	RB0  //KEY_TIMER 0x800
#define	LED_KEY2	RB1  //KEY _UP   // 0x80
#define	LED_KEY3	RB2  //KEY_DOWN //0x08
#define	LED_KEY4	RB3  //KEY_RUN  //0X400;
#define	LED_KEY5	RB4   //KEY_SETUP  // 0x40
#define	LED_KEY6	RB5  //KEY_KILL // 0x04
#define	LED_KEY7	RB6  //KEY_POWER  // 0x200

#define		PCout_0 	RC0				//����RC0�� ȡ��Pout
#define		PCout_1 	RC1				//����RC0�� ȡ��Pout
#define		PCout_2 	RC2				//����RC0�� ȡ��Pout
#define		PCout_3 	RC3				//����RC0�� ȡ��Pout


#define     PDout_0  	RD0	
#define     PDout_1  	RD1	
#define     PDout_2  	RD2	
#define     PDout_3  	RD3	
#define     PDout_4  	RD4	
#define     PDout_5  	RD5	
#define     CONTROL  	RD6	
#define     SEG6  	   RD7

#define     SEG7     (RC0 |= (1<<0))
#define     SEG8     (RC1 &= ~(1<<1))
#define     SEG9     (RC2 &= ~(1<<2))
#define     SEG10    (RC3 &= ~(1<<3))



#define 	Pin0	RA0				//����RA0�� ȡ��Pin0
#define 	Pin1	RA1				//����RA1�� ȡ��Pin1

//#define	DEBUG

volatile unsigned char MainTime;
volatile bit	B_MainLoop;

unsigned char keyflag_1 = 1;
unsigned char keyflag_2 = 2;
unsigned char keyflag_3= 3;
unsigned char keyflag_4 = 4;
unsigned char keyflag_5 = 5;
unsigned char keyflag_6 = 6;
unsigned char keyflag_7 = 7;

//ϵͳ��ʼ��
void Init_System()
{
	asm("nop");
	asm("clrwdt");
	INTCON = 0;				//��ֹ�ж�
	OSCCON = 0X71;			//������Ϊ8M
	OPTION_REG = 0;
	
	//��ʱ�ȴ���Դ��ѹ�ȶ�
	//DelayXms(200);
	WPUA = 0B00000001;				//�����������
	WPUB = 0B00000000;
	WPUC = 0B00000000;
	WPUD = 0B00000000;
	
	TRISA = 0x0;
    PORTA = 0x00;

	TRISC = 0x00;
	PORTC = 0x00;
	
	TRISB = 0xff; //inpute port;
	
	TRISD = 0x00;
	PORTD = 0x3f;
	
	LED_KEY1 = 1;
	LED_KEY2 = 1;
	LED_KEY3 = 1;
	LED_KEY4 = 1;
	LED_KEY5 = 1;
	LED_KEY6 = 1;
	LED_KEY7 = 1;
	
	PIE2 = 0;
	PIE1 = 0x02;
	PR2 = 250;				//8M�½�TMR2����Ϊ125us�ж�
	T2CON = 4;				//ʹ�ܶ�ʱ��2
	
	
	
	INTCON = 0XC0;			//ʹ���ж�
}


/**********************************************************
�������ƣ�Refurbish_Sfr
�������ܣ�ˢ��һЩ���⹦�ܼĴ���
��ڲ�������
���ڲ������� 
��    ע��ÿ��һ��ʱ��ˢ��һ��SFR����ǿ����������
**********************************************************/
void Refurbish_Sfr()
{
	//��Ϊ���ֿ�
//	ANSEL = 0;
//	ANSELH = 0;
	
	TRISA = 0x0;//x65;
	TRISB = 0xFF;
	TRISC = 0xf0;
	
	
    TRISD = 0x00;
	
	
	SSPCON = 0;
	EECON1 = 0;
	
	OPTION_REG = 0;
	WDTCON = 9;
	
	
	//ˢ���ж���ؿ��ƼĴ���
	PIE2 = 0;
	PIE1 = 0x02;
	PR2 = 250;
//	PIR1 = 0x22;
	INTCON = 0XC0;
	if(4 != T2CON)
		T2CON = 4;
}


/***********************************************************
��������
***********************************************************/
void KeyServer()
{
	
	 
	static unsigned int KeyOldFlag = 0;
	unsigned int i = (unsigned int)((KeyFlag[1]<<8) | KeyFlag[0]);
	if(i)
	{
		if(i != KeyOldFlag)
		{
			KeyOldFlag = i;
			switch(i)
			{
				
				
				
				
			
				case 0x4: //KEY_KILL
				
				  keyflag_2 =1;//KEY_KILL
				  
				   
					 break;
				case 0x8://KEY_DOWN //0x08
				
				     keyflag_1 =1;
					 break;
			
			
				case 0x40://KEY_SETUP
				
				      keyflag_4 =1;// KEY_SETUP
				
					 
				break;
				
				
			  case 0x80: //KEY _UP   // 0x100
				     keyflag_5 =1;
					break;
					
				case 0x200: //KEY_POWER
				      keyflag_3 =1;////KEY_POWER  // 0X23
					break;
					
				case 0x400://KEY_RUN  //0X400;
			          keyflag_6 =1;
					break;
					
				case 0x800://KEY_TIMER 0x800
				   keyflag_7 =1;
				break;
				
			
				
				
				
			}
		}
	}
	else
	{
		KeyOldFlag = 0;
	}
}

/***********************************************************
�жϷ�����
***********************************************************/
void interrupt Isr_Timer()
{
	if(TMR2IF)				//��ֻʹ����һ���ж�Դ,������ȥ�ж�
	{
		TMR2IF = 0;
		if(++MainTime >= 32)
		{
			MainTime = 0;
			B_MainLoop = 1;
		}
	}
	else
	{
		PIR1 = 0;
		PIR2 = 0;

	}
}

/***********************************************************
��ѭ��
***********************************************************/
void main()
{
#ifdef	DEBUG
	volatile unsigned char maxtime = 0,maxtime1 = 0;
#endif
	Init_System();
	while(1)
	{
		//if(B_MainLoop)
		{
			B_MainLoop = 0;
			CLRWDT();
			
			CheckTouchKey();

#ifdef	DEBUG			
			maxtime1 = MainTime;
			if(maxtime1 > maxtime)
			{
				maxtime = maxtime1;
			}
#endif			
			
			KeyServer();
			if(keyflag_1 ==1){//KEY_DOWN //0x08
				keyflag_1=0;
				RA1 =1;
				RD0= 0;
				
			};
			if(keyflag_2 ==1){  //KEY_KILL ??
				keyflag_2=0;
				RA1 =0;
				RD0= 1;
				
				RA3= 1;
				RD1=0;
				
			};
			if(keyflag_3 ==1){  //KE_POWER  // 0X20 ??
				keyflag_3=0;
			
			    RA7=0;
				RD3=1;
				
				RA4= 1;
				RD4=0;
				
				
				
			};
			if(keyflag_4 ==1){// KEY_SETUP ??
				keyflag_4=0;
				
			
			    RA7=0;
				RD3=1;
				
				RA4= 1;
				RD4=0;
				
			};
			if(keyflag_5 ==1){//KEY _UP   // 0x100
				keyflag_5=0;
			    RA7=0;
				RD3=1;
				
				RA4= 1;
				RD4=0;
				
			};
			if(keyflag_6 ==1){//KEY_RUN  //0X400;
				keyflag_6=0;
			   
				
				RA4= 0;
				RD4=1;
				
				RA7=1;
				RD5= 0;
			};
			if(keyflag_7 ==1){//KEY_TIMER 0x800
				keyflag_7=0;
			    PORTD = 0x3f;
				
				
			};
			Refurbish_Sfr();
		//	while(!(TKC0&0x40));
		}
	}
}