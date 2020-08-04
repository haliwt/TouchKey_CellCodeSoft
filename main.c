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
#define	LED_KEY2	RB1  //KEY _UP   // 0x100
#define	LED_KEY3	RB2  //KEY_DOWN //0x08
#define	LED_KEY4	RB3  //KEY_RUN  //0X400;
#define	LED_KEY5	RB4   //KEY_SETUP  // 0x80
#define	LED_KEY6	RB5  //KEY_KILL // 0x10
#define	LED_KEY7	RB6  //KE_POWER  // 0X20

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
#define     PDout_6  	RD6	
#define     PDout_7  	RD7




#define 	Pin0	RA0				//����RA0�� ȡ��Pin0
#define 	Pin1	RA1				//����RA1�� ȡ��Pin1

//#define	DEBUG

volatile unsigned char MainTime;
volatile bit	B_MainLoop;

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
	TRISA = 0x0;
	TRISC = 0x0;
	PORTC = 0x00;
	
	TRISB = 0xff; //inpute port;
	
	TRISD = 0;
	PORTD = 0xff;
	
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
	
//	PIR1 = 0x22;
	
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
	TRISC = 0x0;
	//PORTC = 0;
	
    TRISD = 0;
	//PORTD = 0x00;
	
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
				
				
				case 0x1:
				     
					 PCout_0=1;
					 PDout_0=0;
					break;
				case 0x2:
				     PCout_0=1;
					 PDout_0=0;
					 break;
				case 0x4:
				     PCout_0=0;
					 PDout_0=1;
					 break;
				case 0x8:
				     PCout_0=1;
					 PDout_0=0;
					 break;
				case 0x10:
				     PCout_0=0;
					 PDout_0=1;
					 break;
				case 0x20:
					 PCout_0=0;
					 PDout_0=1;
					break;
				case 0x40:
				     PCout_0=1;
					 PDout_0=0;
				break;
				
				case 0x80:
				   PCout_0=0;
					 PDout_0=1;
				  break;
				case 0x100:
				    PCout_0=1;
					 PDout_0=0;
					break;
				case 0x200:
				     PCout_0=0;
					 PDout_0=1;
					break;
				case 0x400:
				     PCout_0=1;
					 PDout_0=0;
					break;
				case 0x800:
				     PCout_0=0;
					 PDout_0=1;
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
		PORTD = 1;
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
		if(B_MainLoop)
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
			Refurbish_Sfr();
		//	while(!(TKC0&0x40));
		}
	}
}