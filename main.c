/*-------------------------------------------

程序名称：CMS89FT628框架示例程序
日期版本：2018/6/15 <V1.1>

备注：

*本程序由 中微半导体有限公司 &应用支持部& 编写整理
*公司网址 www.mcu.com.cn
*技术支持QQ 3001082102  罗工
            3001085706	陈工
-------------------------------------------*/
#include <cms.h>
#include "Touch_Kscan_Library.h"
#include "input.h"


//#define	DEBUG

volatile unsigned char MainTime;
volatile bit	B_MainLoop;



//系统初始化
void Init_System()
{
	asm("nop");
	asm("clrwdt");
	INTCON = 0;				//禁止中断
	OSCCON = 0X71;			//配置振荡为8M
	OPTION_REG = 0;
	GPIO_Init();
	//延时等待电源电压稳定
	//DelayXms(200);
	
	TKM1C1 = 0x10; //配置IO 口为普通GPIO口。
	TKM2C1 = 0x10;
	
	
	TRISA = 0x0;
    PORTA = 0x00;


	
	
	TRISB = 0xff; //inpute port;
	
	TRISD = 0x00;
	PORTD = 0x00;
	
	LED_KEY1 = 1;
	LED_KEY2 = 1;
	LED_KEY3 = 1;
	LED_KEY4 = 1;
	LED_KEY5 = 1;
	LED_KEY6 = 1;
	LED_KEY7 = 1;
	
	PIE2 = 0;
	PIE1 = 0x02;
	PR2 = 250;				//8M下将TMR2设置为125us中断
	T2CON = 4;				//使能定时器2
	
	
	
	INTCON = 0XC0;			//使能中断
}


/**********************************************************
函数名称：Refurbish_Sfr
函数功能：刷新一些特殊功能寄存器
入口参数：无
出口参数：无 
备    注：每隔一定时间刷新一次SFR可增强抗干扰能力
**********************************************************/
void Refurbish_Sfr()
{
	//均为数字口
//	ANSEL = 0;
//	ANSELH = 0;
	
	TRISA = 0x0;//x65;
	TRISB = 0xFF;
	TRISC = 0x0f;
	
	
    TRISD = 0x00;
	
	
	SSPCON = 0;
	EECON1 = 0;
	
	OPTION_REG = 0;
	WDTCON = 9;
	
	
	//刷新中断相关控制寄存器
	PIE2 = 0;
	PIE1 = 0x02;
	PR2 = 250;
//	PIR1 = 0x22;
	INTCON = 0XC0;
	if(4 != T2CON)
		T2CON = 4;
}


/***********************************************************
键处理函数
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
				
				  keyflag_KILL =1;//KEY_KILL
				  
				  break;
				case 0x8://KEY_DOWN //0x08
				
				     keyflag_DOWN =1; //
					 break;
			
			
				case 0x40://KEY_SETUP
				
				      keyflag_SETUP =1;// KEY_SETUP
				
					 
				break;
				
				
			  case 0x80: //KEY _UP   // 0x100
				     keyflag_UP =1;
					break;
					
				case 0x200: //KEY_POWER
				      keyflag_POWER =1;//KEY_POWER  
					break;
					
				case 0x400://KEY_RUN  //0X400;
			          keyflag_RUN =1;
					break;
					
				case 0x800://KEY_TIMER 0x800
				   keyflag_TIMER=1;
				  
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
中断服务函数
***********************************************************/
void interrupt Isr_Timer()
{
	if(TMR2IF)				//若只使能了一个中断源,可以略去判断
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
主循环
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
			if(keyflag_DOWN ==1){//KEY_DOWN //0x08
				keyflag_DOWN=0;
			    
				SEG9 = 1;	
			
				
			}
			if(keyflag_KILL ==1){  //KEY_KILL ??
				keyflag_KILL=0;
				BKLT_L =1;
				
			}
			if(keyflag_POWER ==1){  //KE_POWER  
				keyflag_POWER=0;
			
			    BKLT_R =1;
				
				
				
			}
			if(keyflag_SETUP ==1){// KEY_SETUP ??
				keyflag_SETUP=0;
				BKLT_R=0;
			
				
			}
			if(keyflag_UP ==1){//KEY _UP   // 0x100
				keyflag_UP=0;
			    BKLT_L=0;
				
			}
			if(keyflag_RUN ==1){//KEY_RUN  //0X400;
				keyflag_RUN=0;
			    BKLT_POINT=1;
			
			}
			if(keyflag_TIMER ==1){//KEY_TIMER 0x800
				keyflag_TIMER=0;
				 BKLT_POINT=0;
			
				BKLT_R =0;
				BKLT_L =0;
				SEG9 = 0;	
			}
			Refurbish_Sfr();
		//	while(!(TKC0&0x40));
		}
	}
}