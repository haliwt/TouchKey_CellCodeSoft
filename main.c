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
#include "tm1620.h"
#include "usart.h"



#define TASK_NUM   (5)                  //  这里定义的任务数为4，表示有4个任务会使用此定时器定时。

 static uint8_t runTimes ;
 static uint8_t gEvent = 0;      //g = globe
 static uint16_t getMinute=0;
 static uint16_t getHour=0;
 volatile uint8_t KeyID ;

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



void TaskLEDDisplay(void);
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

static int8_t TimerBaseTim=0;
volatile unsigned char MainTime;
volatile bit	B_MainLoop;
	static uint8_t powerSt =0,timeupSt=0,runSt=0,timerSt=0,timedownSt=0;
	static uint8_t killSt = 0,setupSt = 0,tempuint=1,upflag=0,downflag =0;
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
	OSCCON = 0X71;			//配置振荡为8M  指令周期4个，执行一条指令，单指令 = 0.5us 双指令：1us
	OPTION_REG = 0;
	GPIO_Init();
	//延时等待电源电压稳定
	//DelayXms(200);
	
	
	TRISA = 0x00;
	
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
	//INTCON = 0XC0;			//使能中断
	INTCON = 0xA0;	
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
	
	
	TRISA = 0x00;
	TRISB = 0xFF;

	PortTx =1;
	
	PIE2 = 0;
	PIE1 = 0x02; //
	PR2 = 208 ;//PR2 = 250;				//8M下将TMR2设置为125us中断,104us,波特率9600bps
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
	
	 TRISA = 0x00;//x65;
	 TRISB = 0xFF;
	 TRISD = 0x80;
	 
	
	SSPCON = 0;
	EECON1 = 0;
	
	OPTION_REG = 0;
	WDTCON = 9;
	
	//---------------------------------------
	OPTION_REG = 0xD0;		//Timer0ʹ���ڲ�ʱ��Focs/4��Ԥ��Ƶ��Ϊ1:2
	TMR0 = 131;				//������ʼֵ

	//INTCON = 0xA0;			//��������δ�����ε��жϡ���ֹ�����жϣ�ʹ��Timer0
//---------------------------------------
	
	PIE1 = 0x0;

	PIE2 = 0;
	INTCON = 0xA0;	//INTCON = 0XC0;			//使能中断
	
}


/***********************************************************
 * *
    *Function Name : void KeyServer(void)
	*Function :键处理函数
	*Input Ref:
	*Return Ref:
	*
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
				
				  KeyID = keyflag_KILL;//KEY_KILL
				   gEvent =1;
				  break;
				case 0x8://KEY_DOWN //0x08
				
				     KeyID = keyflag_DOWN; //
					 gEvent =1;
					 break;
			
			
				case 0x40://KEY_SETUP
				
				      KeyID = keyflag_SETUP ;// KEY_SETUP
				       gEvent =1;
					 
				break;
				
				
			  case 0x80: //KEY _UP   // 0x100
				    KeyID = keyflag_UP ;
					 gEvent =1;
					break;
					
				case 0x200: //KEY_POWER
				     KeyID =  keyflag_POWER ;//KEY_POWER  
					   gEvent =1;
					break;
					
				case 0x400://KEY_RUN  //0X400;
			          KeyID = keyflag_RUN ;
					   gEvent =1;
					break;
					
				case 0x800://KEY_TIMER 0x800
				   KeyID = keyflag_TIMER;
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
	static uint16_t irtimes =0;
	Init_System();
	PowerOn_RunDisp();

	while(1)
	{
	   
	    if(runTimes==0){
			 runTimes++;
	         Init28_System();
			 USART_SendData(0);
   
			 goto Next;
		}
		if(B_MainLoop)
		{
Next:	    runTimes = 0x04;
             irtimes ++;
			 if(irtimes==3000)  runTimes=0; 
            Init_System();
            B_MainLoop = 0;
            runTimes=0;
			CLRWDT();
			
			CheckTouchKey();
			KeyServer();
			TaskKeySan();
			
			TaskLEDDisplay();
			IR_ReadData();
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
	*Function Name: void TaskKeySan(void)
	*Function : Toch key scan 
	*Input Ref:No
	*Output Ref:No
	*
***********************************************************/
void TaskKeySan(void)
{

	
	switch(KeyID){
		
		case keyflag_DOWN:
	//KEY_DOWN //0x08
			
				timedownSt = timedownSt ^ 0x01;
				BKLT_L =1;
			    BKLT_R =1;
				if(keystr.SetupOn ==1 && gEvent ==1){
					   gEvent =0;
						if(keystr.TimeBaseUint == 0)keystr.TimeBaseUint =1;
						keystr.TimeBaseUint --;
						
						if(keystr.TimeBaseUint == 0){
							   
							if(keystr.TimeDecadeHour ==0 &&  keystr.TimeHour==0 &&  keystr.TimeMinute==0 )
							{             keystr.TimeBaseUint=0;
										   keystr.TimeMinute=0;
											keystr.TimeDecadeHour=0;
											keystr.TimeHour=0;
											
							}
						    else{
								// keystr.TimeBaseUint=9;//借一当十
							
								 if( keystr.TimeMinute==0){
										if(keystr.TimeDecadeHour ==0 &&  keystr.TimeHour==0 ) {
											
												keystr.TimeBaseUint=0;
												keystr.TimeMinute=0;
												keystr.TimeDecadeHour=0;
												keystr.TimeHour=0;
								
											
										}
										else{
											keystr.TimeBaseUint=9;
											keystr.TimeMinute=5;//借一当5  60分钟
											
											if(keystr.TimeDecadeHour ==0){
												
												if(keystr.TimeHour > 0){
													keystr.TimeHour --		;				  
													keystr.TimeDecadeHour=9;//借一当十
													
												}
												else{
													keystr.TimeBaseUint=0;
													keystr.TimeMinute=0;
													keystr.TimeDecadeHour=0;
													keystr.TimeHour=0;
													
												}
											}
											else if(keystr.TimeDecadeHour > 0){
												
												keystr.TimeDecadeHour --	;					  
												keystr.TimeMinute=5;//借一当5
												keystr.TimeBaseUint=9;
												
											}
													
										}			
								    }
							        else if(keystr.TimeMinute > 0) { //借位 十位
										keystr.TimeMinute --	;					  
										keystr.TimeBaseUint=9;//借一当十
						   }						  
						} 
					}	
				}    
										
				else if(gEvent ==1 && downflag ==0 && keystr.SetupOn ==0){ //风速递减
						gEvent =0;
						keystr.windMask =1;
						if(keystr.windLevel <=0)
						    keystr.windLevel = 1;
						else {
						     keystr.windLevel --;//minWind;
						     if(keystr.windLevel == 0)keystr.windLevel = 1;
					     }
				}
			   
		
	 break ;
	 
	 case keyflag_KILL:
	
			killSt =killSt ^ 0x01;
			
			BKLT_L =1;
			BKLT_R =1;
				keystr.windMask =1;
				if(killSt ==1 && gEvent ==1){
					gEvent =0;
					if(keystr.RunOn==1)keystr.KillOn = 2;
	
			�		
				   
				}
				else if(gEvent==1){
					gEvent =0;
					killSt =0;
					 keystr.KillOn = 0;
					if(keystr.KillOn==2)keystr.KillOn = 1;
					 killSt =0;
				}
	break;
	
	case keyflag_POWER ://只是开背光，和关背光
	
			   // powerSt =powerSt ^ 0x1;
			    
				keystr.windMask = 1;
				
			    if(powerSt ==0 && gEvent ==1){
					gEvent =0;
				  	BKLT_L =1;
				    BKLT_R =1;
				    keystr.PowerOn =1;
					powerSt ++;
					upflag=0;
					
			    }
			    else if(gEvent==1 && powerSt == 1){
					powerSt =0;
					gEvent =0;
			        BKLT_R =0;
				    BKLT_L= 0;
				    powerSt =0;
				    keystr.PowerOn =0;
					keystr.RunOn = 0;
					keystr.KillOn =0 ;
				}
				
				
				
	break ;
	
	case keyflag_UP:
			

		timeupSt = timeupSt ^ 0x01;
		
		BKLT_L =1;
		BKLT_R =1;
		
		if(keystr.SetupOn ==1 && gEvent ==1 && upflag ==1){
					gEvent =0;
				
					keystr.TimeBaseUint ++ ;
					if(keystr.TimeBaseUint == 10){
						keystr.TimeBaseUint=0;
						keystr.TimeMinute++;
						if(keystr.TimeMinute==6){ 
							keystr.TimeMinute =0;
							keystr.TimeDecadeHour ++;
							{
							   if(keystr.TimeHour == 2){
									if(keystr.TimeDecadeHour == 4){
										keystr.TimeBaseUint=0;
										keystr.TimeMinute=0;
										keystr.TimeDecadeHour=0;
										keystr.TimeHour=0;
									}

								}
							   else{
									if(keystr.TimeDecadeHour ==10){ //小时
										keystr.TimeDecadeHour =0;
										keystr.TimeHour ++;
									
									}
							    }
							}
						}	
					}		
						
		}
		else if(gEvent ==1 && upflag ==0 && keystr.SetupOn ==0){ //UP 
				gEvent =0;
				keystr.windMask = 1;
				
				if(keystr.windLevel >= 5){//if(keystr.windLevel >maxWind){
					keystr.windLevel = 0;//minWind;
				}
				keystr.windLevel ++ ;
					
		}		
		break;
		
		case keyflag_RUN:
			
				runSt = runSt ^ 0x01;
				BKLT_L =1;
				BKLT_R =1;
			    keystr.windMask = 1;
				if(runSt ==1 && gEvent == 1){
					gEvent =0;
			        if( keystr.KillOn ==1)keystr.KillOn = 2;
					else keystr.KillOn = 1;
					keystr.RunOn =1;
				    
				
				}
				else if(gEvent ==1){
					gEvent =0 ;
					keystr.KillOn = 0;
					keystr.RunOn =0;
					//keystr.windLevel=0;
				}
			  
			
		break;
		
		case keyflag_SETUP:
			
				setupSt  = setupSt ^ 0x01;
				BKLT_L =1;
				BKLT_R =1;
				
				if(setupSt == 1 && gEvent==1){
					gEvent =0;
					keystr.SetupOn =1;
					keystr.TimerOn =0;
					TimerBaseTim=0;
					upflag=1;
					downflag =1;
				}
				else if(gEvent ==1){
					gEvent =0;
					
					 keystr.SetupOn = 0;
					 downflag = 0;
					 upflag =0;
					 BKLT_TIM=1; //turn off
					 keystr.TimerOn =0;
					 
				}
			
		break;
		
		case 	keyflag_TIMER:
			
			    BKLT_L =1;
				BKLT_R =1;
				timerSt = timerSt ^ 0x01;
				if(timerSt ==1 && gEvent ==1){
				   gEvent =0;
				   BKLT_TIM=0; //ON 
				   keystr.TimerOn =1;
				   getMinute =0 ;
				   TimerBaseTim = keystr.TimeBaseUint ;
				   keystr.SetupOn =0;
				   upflag=1;
				   downflag =1;
				   
				}
				else if(gEvent ==1 && timerSt == 0){
					 gEvent =0;
					BKLT_TIM=1; //turn off
					keystr.TimerOn =0;
					 upflag=0;
				     downflag =0;
					
				}
			
	    break ;
	}
			if(keystr.TimerOn ==1 && keystr.SetupOn != 1)
			{
				if(keystr.TimeBaseUint== 0 && keystr.TimeMinute==0 && keystr.TimeDecadeHour==0 && keystr.TimeHour==0){
						BKLT_TIM=1; //Tunr OFF
						keystr.TimerOn =0;
				}
			}
			Refurbish_Sfr();
			keystr.SendSwitchData = keystr.PowerOn << 7 | keystr.RunOn << 6 | keystr.KillOn << 5   ;
            keystr.SendWindData  = keystr.windLevel;
			keystr.SendBCCdata =BCC();
}
/***********************************************************
	*
	*Function Name: void TaskDisplayClock(void)
	*Function : display LED numbers
	*Input Ref:No
	*Output Ref:No
	*
***********************************************************/
void TaskLEDDisplay(void)
{
     static uint16_t timealt =0;
    	if(keystr.TimerOn ==1 && keystr.SetupOn != 1){
		
						runTimes = 0x05;
						 
						 timealt++ ;
						  if(timealt ==2000){ //10秒 发一次数据
							  timealt  = 0;
							  runTimes =0;
						  }
						if( keystr.TimeBaseUint== 0){
							   
							if(keystr.TimeDecadeHour ==0 &&  keystr.TimeHour==0 &&  keystr.TimeMinute==0 )
							{             keystr.TimeBaseUint=0;
										   keystr.TimeMinute=0;
											keystr.TimeDecadeHour=0;
											keystr.TimeHour=0;
											
							}
						    else{
								// keystr.TimeBaseUint=9;//借一当十
							
								 if( keystr.TimeMinute==0){
										if(keystr.TimeDecadeHour ==0 &&  keystr.TimeHour==0 ) {
											
											   keystr.TimeBaseUint=0;
												keystr.TimeMinute=0;
												keystr.TimeDecadeHour=0;
												keystr.TimeHour=0;
								
											
										}
										else{
										    keystr.TimeBaseUint=9;
											keystr.TimeMinute=5;//借一当5  60分钟 进位 “小时”
											
											if(keystr.TimeDecadeHour ==0){
												
												if(keystr.TimeHour > 0){
													keystr.TimeHour --		;				  
													keystr.TimeDecadeHour=9;//借一当十
													
												}
												else{
													 keystr.TimeBaseUint=0;
													keystr.TimeMinute=0;
													keystr.TimeDecadeHour=0;
													keystr.TimeHour=0;
													
												}
											}
											else if(keystr.TimeDecadeHour > 0){
												
												keystr.TimeDecadeHour --	;					  
												keystr.TimeMinute=5;//借一当5
												 keystr.TimeBaseUint=9;
												
											}
													
										}			
								    }
							        else if(keystr.TimeMinute > 0) { //借位 十位
										keystr.TimeMinute --	;					  
										keystr.TimeBaseUint=9;//借一当十
						   }						  
						} 
					}	
			
			BKLT_TIM=0; //Tunr ON
			Tm1620Dis();
			delay_10us(10);
	}
	else if(keystr.windMask ==0){
 	  Tm1620Dis();
	delay_10us(10);
	}
 	if(keystr.windMask ==1 && keystr.TimerOn !=1){
 		keystr.windMask = 0;
 		Tm1620_RunDisp();
		 delay_10us(10);
 	}
   
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

	if(TMR2IF)				//若只使能了一个中断源,可以略去判断
	{
		TMR2IF = 0;
	    gBaudTime=1;
		
		
	}	
	


    if(runTimes !=1)
	{
	if(T0IF)
	{
	//---------------------------------------
		TMR0 += 131;		//���¸���ֵ���ڸ�ֵǰTimer0���м��������ڸû����ϼӳ�ֵ
	//---------------------------------------

		T0IF = 0;			//���жϱ�־λ
		seconds++;
		if(++MainTime >= 31 || seconds >= 4000)//3.87ms
		{
			 MainTime = 0;
			B_MainLoop = 1;
			
			if(seconds >=4000){ //1s
						seconds =0;
						minutes ++;
						if(minutes ==120){ //1 minute
							minutes =0;
							if(keystr.TimerOn ==1){
							  if(keystr.TimeBaseUint == 0) keystr.TimeBaseUint=1;
							   keystr.TimeBaseUint --;
							   
							}
						 }

			}
		
			  #if 1
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
				#endif 
					
		}
	
	else
	{
		PIR1 = 0;
		PIR2 = 0;

	}
	
	}
}
	
}


