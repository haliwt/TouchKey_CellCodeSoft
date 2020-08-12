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
#include "input.h"
#include "i2c.h"
#include "usart.h"


#define TASK_NUM   (5)                  //  ���ﶨ���������Ϊ4����ʾ��4�������ʹ�ô˶�ʱ����ʱ��
 uint8_t gBaudTime = 0;
 static uint8_t runTimes ;
 volatile uint16_t getMinute;
 volatile uint16_t getHour;

uint16_t TaskCount[TASK_NUM] ;           //  ����Ϊ4��������4����������Ŷ�ʱֵ
uint8_t  TaskMark[TASK_NUM];             //  ͬ����Ӧ4����־λ��Ϊ0��ʾʱ��û����Ϊ1��ʾ��ʱʱ�䵽��


struct _TASK_COMPONENTS
{
    uint8_t Run;                  // �������б�ǣ�0-�����У�1����
    uint16_t Timer;                // ��ʱ��
    uint16_t ItervalTime;              // �������м��ʱ��
    void (*TaskHook)(void);    // Ҫ���е�������
} TASK_COMPONENTS;             // ������

typedef enum _TASK_LIST
{
    TAST_DISP_NUMBER,          // ��ʾ����
    TAST_KEY_SAN,             // ����ɨ��
    TASK_RECE_IR,             // ����IR
    TASK_TELEC_WS,            // ͬ��������ͨѶ
    TASKS_MAX                 // �ܵĿɹ�����Ķ�ʱ������Ŀ
} TASK_LIST;



void TaskLEDDisplay(void);
void TaskKeySan(void);
void TaskReceiveIR(void);
void TaskTelecStatus(void);
void TaskProcess(void);

static struct _TASK_COMPONENTS TaskComps[] =
{
    {0, 300, 300, TaskLEDDisplay},        // ��ʾ���� 125us * 1000=125ms��ɨ��һ��
    {0, 1000, 1000, TaskKeySan},               // ����ɨ�� 125us * 500= 62.5msɨ��һ��
    {0, 60000, 60000, TaskReceiveIR},            // ����IR   125us * 60000= 3��48�� ִ��һ��
    {0, 40000, 40000, TaskTelecStatus}       // ͬ����ͨѶ 852ms = 125us *4000  =2��36��ִ��һ�� 
};


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
	GPIO_Init();
	//��ʱ�ȴ���Դ��ѹ�ȶ�
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
	
	PIE2 = 0;
	PIE1 = 0x02;
	PR2 = 28 ;//PR2 = 250;				//8M�½�TMR2����Ϊ125us�ж�,104us
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
	
	//TRISA = 0x0;//x65;
	TRISB = 0xFF;
	TRISC = 0x0f;
	
	
    TRISD = 0x00;
	
	
	SSPCON = 0;
	EECON1 = 0;
	
	OPTION_REG = 0;
	WDTCON = 9;
	
	
	//ˢ���ж���ؿ��ƼĴ���
	PIE2 = 0;
	PIE1 = 0x02;
	PR2 = 28 ;//PR2 = 250;
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
**
**Function Name: void main()
**Function :
**Input Ref:NO
**Return Ref:NO
**
***********************************************************/
void main()
{
	uint8_t state =1,i=0;
	Init_System();
	while(1)
	{
		state =1;
        if(runTimes==0){
			 runTimes++;
			
			 WriteByte(0xAB) ;

			
			goto Next;
		 }

	    if(B_MainLoop)
		{
Next:		B_MainLoop = 0;
			CLRWDT();
			
			CheckTouchKey();
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
		}

				#if 0
					switch(state){
						
					case 1 :
					 if(runTimes==0){
						 runTimes++;
						
						 WriteByte(0xAB) ;

						
						goto Next;
					 }
					 state =2;
					break;
					
			Next:	case 2: 
			         TaskLEDDisplay();
			            BKLT_L=1;
					//	delay_10us(1000);
					
				        goto NEXT_2;
						state=3;
					break;
			NEXT_2:		  case 3:
					  
				  	    BKLT_R=1;
						BKLT_POINT=1;
					    delay_10us(1000);
						delay_10us(1000);
						
						state=1;
				    break;
					}
				#endif 
		
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
    for (i=0; i<TASKS_MAX; i++)           // ���������ѯʱ�䴦��
    {
        if(TaskComps[i].Run)           // ʱ�䲻Ϊ0
        {
             TaskComps[i].TaskHook();         // ��������
             TaskComps[i].Run = 0;          // ��־��0
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
void TaskLEDDisplay(void)
{
   //LEDDisplay_Data();
   //BKLT_R=1;
     BKLT_POINT=1;
	 BKLT_TIM;//=1;
	 //Init_System();
     WriteByte(0x8b) ;
	 WriteByte(0x01) ;	
		
   
	

	 

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
	BKLT_L=1;
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

	if(TMR2IF)				//��ֻʹ����һ���ж�Դ,������ȥ�ж�
	{
		TMR2IF = 0;
		//delay_us(1);
		asm("nop");
		asm("nop");
		

		//if(++MainTime >= 31)//3.87ms
		{
			
			//delay_us(1);

			#if 0
			MainTime = 0;
			B_MainLoop = 1;
		    gBaudTime =1 ;
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
				
				for (i=0; i<TASKS_MAX; i++)          // ���������ѯʱ�䴦��
				{
						if (TaskComps[i].Timer)          // ʱ�䲻Ϊ0
						{
							TaskComps[i].Timer--;         // ��ȥһ������
							if (TaskComps[i].Timer == 0)       // ʱ�������
							{
								TaskComps[i].Timer = TaskComps[i].ItervalTime;       // �ָ���ʱ��ֵ��������һ��
								TaskComps[i].Run = 1;           // �����������
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
				#endif 	
	
	}	
	}	
	else
	{
		PIR1 = 0;
		PIR2 = 0;

	}
}
	

