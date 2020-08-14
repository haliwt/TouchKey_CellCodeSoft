/*-------------------------------------------

程序名称：CMS89FT628框架示例程序
日期版本：2018/6/15 <V1.1>

备注：波特率 2400bps

*本程序由 中微半导体有限公司 &应用支持部& 编写整理
*公司网址 www.mcu.com.cn
*技术支持QQ 3001082102  罗工
            3001085706	陈工
-------------------------------------------*/
#include <cms.h>
#include "Touch_Kscan_Library.h"
#include "input.h"
#include "i2c.h"
#include "usart.h"


#define TASK_NUM   (5)                  //  这里定义的任务数为4，表示有4个任务会使用此定时器定时。
 uint8_t gBaudTime = 0;
 static uint8_t runTimes ;
 static uint8_t gEvent = 0;      //g = globe
 volatile uint16_t getMinute;
 volatile uint16_t getHour;

uint16_t TaskCount[TASK_NUM] ;           //  这里为4个任务定义4个变量来存放定时值
uint8_t  TaskMark[TASK_NUM];             //  同样对应4个标志位，为0表示时间没到，为1表示定时时间到。


struct _TASK_COMPONENTS
{
    uint8_t Run;                  // 程序运行标记：0-不运行，1运行
    uint16_t Timer;                // 计时器
    uint16_t ItervalTime;              // 任务运行间隔时间
    void (*TaskHook)(void);    // 要运行的任务函数
} TASK_COMPONENTS;             // 任务定义

typedef enum _TASK_LIST
{
    TAST_DISP_NUMBER,          // 显示数字
    TAST_KEY_SAN,             // 按键扫描
    TASK_RECE_IR,             // 接收IR
    TASK_TELEC_WS,            // 同控制主板通讯
    TASKS_MAX                 // 总的可供分配的定时任务数目
} TASK_LIST;



void TaskLEDDisplay(uint8_t m,uint8_t de, uint8_t hundred, uint8_t thousand);
void TaskKeySan(void);
void TaskReceiveIR(void);
void TaskTelecStatus(void);
void TaskProcess(void);

static struct _TASK_COMPONENTS TaskComps[] =
{
    {0, 300, 300, TaskLEDDisplay},        // 显示数字 125us * 1000=125ms，扫描一次
    {0, 1000, 1000, TaskKeySan},               // 按键扫描 125us * 500= 62.5ms扫描一次
    {0, 60000, 60000, TaskReceiveIR},            // 接收IR   125us * 60000= 3分48秒 执行一次
    {0, 40000, 40000, TaskTelecStatus}       // 同主板通讯 852ms = 125us *4000  =2分36秒执行一次 
};


volatile unsigned char MainTime;
volatile bit	B_MainLoop;
/**********************************************************
函数名称：
函数功能：
入口参数：无
出口参数：无 
备    注：每隔一定时间刷新一次SFR可增强抗干扰能力
**********************************************************/
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
	
	
	//TRISA = 0x0;
    //PORTA = 0x00;

		PortTx =1;
	
	
	TRISB = 0xff; //inpute port;
	
	TRISD = 0x00;

	
	LED_KEY1 = 1;
	LED_KEY2 = 1;
	LED_KEY3 = 1;
	LED_KEY4 = 1;
	LED_KEY5 = 1;
	LED_KEY6 = 1;
	LED_KEY7 = 1;
	//---------------------------------------
	OPTION_REG = 0xD0;		//Timer0ʹ���ڲ�ʱ��Focs/4��Ԥ��Ƶ��Ϊ1:2
	TMR0 = 131;				//������ʼֵ
	//INTCON = 0xA0;			//��������δ�����ε��жϡ���ֹ�����жϣ�ʹ��Timer0
//---------------------------------------

	PIE2 = 0;
	PIE1 = 0x0;
	INTCON = 0XC0;			//使能中断

}
/**********************************************************
函数名称：
函数功能：
入口参数：无
出口参数：无 
备    注：每隔一定时间刷新一次SFR可增强抗干扰能力
**********************************************************/
void Init28_System()
{
	asm("nop");
	asm("clrwdt");
	INTCON = 0;				//禁止中断
	OSCCON = 0X71;			//配置振荡为8M
	OPTION_REG = 0;
	GPIO_Init();
	//延时等待电源电压稳定
	//DelayXms(200);
	
	
	
    //PORTA = 0x00;

	PortTx =1;
	
	PIE2 = 0;
	PIE1 = 0x02; //
	PR2 = 28 ;//PR2 = 250;				//8M下将TMR2设置为125us中断,104us
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
	
	//TRISA = 0x0;//x65;
	TRISB = 0xFF;
	//TRISA = 0x0;
	
	
    TRISD = 0x00;
	
	
	SSPCON = 0;
	EECON1 = 0;
	
	OPTION_REG = 0;
	WDTCON = 9;
	
	//---------------------------------------
	OPTION_REG = 0xD0;		//Timer0ʹ���ڲ�ʱ��Focs/4��Ԥ��Ƶ��Ϊ1:2
	TMR0 = 131;				//������ʼֵ
	PIE1 = 0x0;
	//INTCON = 0xA0;			//��������δ�����ε��жϡ���ֹ�����жϣ�ʹ��Timer0
//---------------------------------------
	
	PIE1 = 0x0;

	PIE2 = 0;
	INTCON = 0XC0;			//使能中断
	
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
				   gEvent =1;
				  break;
				case 0x8://KEY_DOWN //0x08
				
				     keyflag_DOWN =1; //
					 gEvent =1;
					 break;
			
			
				case 0x40://KEY_SETUP
				
				      keyflag_SETUP =1;// KEY_SETUP
				       gEvent =1;
					 
				break;
				
				
			  case 0x80: //KEY _UP   // 0x100
				     keyflag_UP =1;
					 gEvent =1;
					break;
					
				case 0x200: //KEY_POWER
				      keyflag_POWER =1;//KEY_POWER  
					   gEvent =1;
					break;
					
				case 0x400://KEY_RUN  //0X400;
			          keyflag_RUN =1;
					   gEvent =1;
					break;
					
				case 0x800://KEY_TIMER 0x800
				   keyflag_TIMER=1;
				    gEvent =1;
				  
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
**
**Function Name: void main()
**Function :
**Input Ref:NO
**Return Ref:NO
**
***********************************************************/
void main()
{
	
	Init_System();
	 IIC_Init_TM1650();
	TM1650_Set(0x48,0x31);//初始化为5级灰度，开显示
	BKLT_POINT=0;

	while(1)
	{
		
        if(runTimes==0){
			 runTimes++;
			 Init28_System();
			 WriteByte(0x01) ;
			goto Next;
		 }

	    if(B_MainLoop)
		{
Next:		Init_System();
            B_MainLoop = 0;
            runTimes=0;
			CLRWDT();
			
			CheckTouchKey();
			KeyServer();
			TaskKeySan();
			
			TaskLEDDisplay(keystr.TimeSetUp,keystr.TimeMinute,keystr.TimeDecadeHour,keystr.TimeHour);
		}
		
			
		
		
		
	}
}
/***********************************************************
	*
	*Function Name: void TaskProcess(void)
	*Function : process
	*Input Ref:No
	*Output Ref:No
	*
***********************************************************/
void TaskProcess(void)
{
	uint8_t i;
    for (i=0; i<TASKS_MAX; i++)           // 逐个任务轮询时间处理
    {
        if(TaskComps[i].Run)           // 时间不为0
        {
             TaskComps[i].TaskHook();         // 运行任务
             TaskComps[i].Run = 0;          // 标志清0
        }
    }

}
/***********************************************************
	*
	*Function Name: void TaskDisplayClock(void)
	*Function : display LED numbers
	*Input Ref:No
	*Output Ref:No
	*
***********************************************************/
void TaskLEDDisplay(uint8_t m,uint8_t de, uint8_t hundred, uint8_t thousand)
{
   
    // Init_Tm1650();
	TM1650_Set(0x48,0x31);//初始化为5级灰度，开显示
	TM1650_Set(0x68,segNumber[m]);//初始化为5级灰度，开显示
   

	TM1650_Set(0x6A,segNumber[de]);//初始化为5级灰度，开显示


  TM1650_Set(0x6C,segNumber[hundred]);//初始化为5级灰度，开显示

	
   TM1650_Set(0x6E,segNumber[thousand]);//初始化为5级灰度，开显示
   
}
/***********************************************************
	*
	*Function Name: void TaskKeySan(void)
	*Function : Toch key scan 
	*Input Ref:No
	*Output Ref:No
	*
***********************************************************/
void TaskKeySan(void)
{
	static uint8_t powerSt =0,timeupSt=0,runSt=0,timerSt=0,timedownSt=0;
	static uint8_t killSt = 0,setupSt = 0;
	if(keyflag_DOWN ==1){//KEY_DOWN //0x08
				keyflag_DOWN=0;
				timedownSt = timedownSt ^ 0x01;
				if(timedownSt ==1){
			     BKLT_L =1;
				 if(gEvent == 1 ){
					 gEvent =0;
				 }
				}
				else{
					BKLT_L =0;	
					
				}
				 
				
			}
			if(keyflag_KILL ==1){  //KEY_KILL ??
				keyflag_KILL=0;
				killSt =killSt ^ 0x01;
				if(killSt ==1){
				 	BKLT_R =1;
					
				}
				else{
					BKLT_R =0;
				}
			}
			if(keyflag_POWER ==1){  //KE_POWER  
				keyflag_POWER=0;
			
			    powerSt =powerSt ^ 0x1;
			    if(powerSt ==1){
				    BKLT_R =1;
				    BKLT_L =1;
				   // BKLT_TIM=0;
			    }
			    else{
			    	BKLT_R =0;
				    BKLT_L =0;
				   // BKLT_TIM=1;
			    }
				
				
				
			}
			if(keyflag_SETUP ==1){// KEY_SETUP ??
				keyflag_SETUP=0;
				setupSt  = setupSt ^ 0x01;
				if(setupSt == 1){
				 BKLT_R=1;
				 keystr.SetupOn =1;
				}
				else{
					 BKLT_R=0;
					 keystr.SetupOn = 0;
				}
			
				
			}
			if(keyflag_UP ==1){//KEY _UP   // 0x100
				keyflag_UP=0;
				 timeupSt = timeupSt ^ 0x01;
				 if(timeupSt ==1){
			        BKLT_L =1;//BKLT_L=0;
					 gEvent =0;
				    if(keystr.SetupOn ==1){
						keystr.TimeSetUp ++ ;
						if(keystr.TimeSetUp == 10){
							keystr.TimeSetUp =0;
							keystr.TimeMinute++;
							if(keystr.TimeMinute==10){
								keystr.TimeMinute =0;
								keystr.TimeDecadeHour ++;
								if(keystr.TimeDecadeHour ==10){
									keystr.TimeDecadeHour =0;
									keystr.TimeHour ++;
									if(keystr.TimeHour == 2){
										if(keystr.TimeDecadeHour ==4){
										keystr.TimeHour =0;
										keystr.TimeDecadeHour =0;
										keystr.TimeMinute =0;
										keystr.TimeSetUp=0;
										}
									}
								}
							}	
							
							
						}
					}
					else{
					
					    keystr.windLevel ++ ;
					   if(keystr.windLevel ==5){
						   keystr.windLevel =0;
					   }
						
						
					}
				}
				 else{
				  BKLT_L =0;
				  gEvent =0;
				  if(keystr.SetupOn ==1){
						  keystr.TimeSetUp ++ ;
							if(keystr.TimeSetUp == 10){
								keystr.TimeSetUp =0;
								keystr.TimeMinute++;
								if(keystr.TimeMinute==10){
									keystr.TimeMinute =0;
									keystr.TimeDecadeHour ++;
									if(keystr.TimeDecadeHour ==10){
										keystr.TimeDecadeHour =0;
										keystr.TimeHour ++;
										if(keystr.TimeHour == 2){
											if(keystr.TimeDecadeHour ==4){
											keystr.TimeHour =0;
											keystr.TimeDecadeHour =0;
											keystr.TimeMinute =0;
											keystr.TimeSetUp=0;
											}
										}
									}
								}	
								
								
							}
						
					}
				    else{
						keystr.windLevel ++ ;
					   if(keystr.windLevel ==5){
						   keystr.windLevel =0;
					   }
						
					}
				 
				 } 
				
			}
			if(keyflag_RUN ==1){//KEY_RUN  //0X400;  OK
				keyflag_RUN=0;
				runSt = runSt ^ 0x01;
			
				if(runSt ==1){
			        BKLT_L =1;// BKLT_POINT=1;
					
				}
				else{
					BKLT_L = 0;
					
				}
			  
			
			}
			if(keyflag_TIMER ==1){//KEY_TIMER 0x800
				keyflag_TIMER=0;
				timerSt = timerSt ^ 0x01;
				if(timerSt ==1)
				   BKLT_TIM=0;
				else 
					BKLT_TIM=1; //turn off
			
			}
			Refurbish_Sfr();
}
/***********************************************************
	*
	*Function Name: void TaskReceiveIR(void)
	*Function : receive IR logic level
	*Input Ref:No
	*Output Ref:No
	*
***********************************************************/
void TaskReceiveIR(void)
{
  
  BKLT_R=0;
  BKLT_L=0;
  delay_10us(1000);

}
/***********************************************************************************************
	*
	*Function Name: void TaskTelecStatus(void)
	*Function : ommunicate serial baud rate =9600bps
	*Input Ref:No
	*Output Ref:No
	*
*************************************************************************************************/
void TaskTelecStatus(void)
{
     BKLT_R=1;
   
   // USART_SendData(data);
}
/***********************************************************
**
**Function Name: void interrupt Isr_Timer()
**Function :
**Input Ref:NO
**Return Ref:NO
**
***********************************************************/
void interrupt Isr_Timer()
{
	static uint16_t seconds=0,minutes=0, ptpwm_flag=0;
	uint8_t i;

	//if(TMR2IF)				//若只使能了一个中断源,可以略去判断
	{
		TMR2IF = 0;
	  //  runTimes =1;
		asm("nop");
		asm("nop");
		asm("nop");
		
	}	
	


    if(runTimes !=1)
	{
	if(T0IF)
	{
	//---------------------------------------
		TMR0 += 131;		//���¸���ֵ���ڸ�ֵǰTimer0���м��������ڸû����ϼӳ�ֵ
	//---------------------------------------

		T0IF = 0;			//���жϱ�־λ
		if(++MainTime >= 31)//3.87ms
		{
			 MainTime = 0;
			B_MainLoop = 1;

			seconds++;
				//Telec->get_8_microsecond++;
				ptpwm_flag=ptpwm_flag^0x1;
				if(ptpwm_flag==1)
				{
					//PortPwm =1;
				}
				else
				{
					//PortPwm =0 ;
				}

				for (i=0; i<TASKS_MAX; i++)          // ����������ѯʱ�䴦��
				{
						if (TaskComps[i].Timer)          // ʱ�䲻Ϊ0
						{
							TaskComps[i].Timer--;         // ��ȥһ������
							if (TaskComps[i].Timer == 0)       // ʱ��������
							{
								TaskComps[i].Timer = TaskComps[i].ItervalTime;       // �ָ���ʱ��ֵ��������һ��
								TaskComps[i].Run = 1;           // ������������
							}
						}
					}

					if(seconds==65536){ //��ʱ��852ms//1.7s
						seconds =0;
						minutes ++;
						if(minutes ==71){ //1����ʱ��
							minutes =0;
							getMinute++;
						}

					}
		}
	}
	else
	{
		PIR1 = 0;
		PIR2 = 0;

	}
}
}
	

