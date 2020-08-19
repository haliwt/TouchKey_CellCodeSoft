#include "tm1620.h"


uint8_t LCDBuf[6]={0,0,0,0,0,0};
/******************************************************************
    *
    *Function Name:void delay_10us(uint16_t n)
    *Function :delay times
    *Input Ref:
    *Return Ref:
    *
*******************************************************************/
void delay_14us(uint16_t n)
{
     uint16_t i;
	for(i=0;i<n;i++)
	{
    asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
    asm("nop");
    asm("nop");
    //asm("nop");
   // asm("nop");
    asm("nop"); //better 
    asm("nop");// below new add --standard level
   // asm("nop"); //
   /***********************/
   
		
		}	
}
       
    

//*************************************
// 函数名称：Tm1620Dis
// 函数功能：驱动显示数据
// 入口参数：要操作的数据
// 出口参数：无
// 技术简要：20MS操作一次
//***************************************/
void Tm1620Dis(void)
{
	
		STB_TM1620 =0 ;  
        Tm1620SentData(ModeDisTM1620); 
		//设置显示模式，6位8段--上电默认是7段8段
		STB_TM1620 =1; 
   // Nop(2);
	   asm("nop");
       asm("nop");	
	
	 
		STB_TM1620=0;   
		//Tm1620SentData(AddrAutoAdd);
        Tm1620SentData(AddrFixed);//AddrFixed
		//写显示，自动累加地址      
		STB_TM1620=1; 
		//Nop(2);
         asm("nop");
        asm("nop");	
 
		STB_TM1620=0;   
		Tm1620SentData(Addr00H);
		//指向地址0   
	
		Tm1620SentData(segNumber[1]);
		//向地址0 先数据LCDBuf[0]
        STB_TM1620 =1; 
         asm("nop");
        asm("nop");	
         STB_TM1620 =0; 
         Tm1620SentData(OpenDisTM1620|Set14_16TM1620); 
          STB_TM1620 =1; 
    #if 0
        asm("nop");
        asm("nop");	
        STB_TM1620 =0; 
        Tm1620SentData(Addr01H);
		Tm1620SentData(segNumber[2]);
          STB_TM1620 =1; 
        asm("nop");
        asm("nop");	
        STB_TM1620 =0; 
		//向地址2 先数据LCDBuf[1]
		Tm1620SentData(Addr02H);
		Tm1620SentData(segNumber[3]);
          STB_TM1620 =1; 
        asm("nop");
        asm("nop");	
        STB_TM1620 =0; 
		
		Tm1620SentData(Addr03H);
        Tm1620SentData(segNumber[4]);
          STB_TM1620 =1; 
        asm("nop");
        asm("nop");	
        STB_TM1620 =0; 
		//向地址6 先数据LCDBuf[3]
		Tm1620SentData(Addr04H);
		Tm1620SentData(segNumber[5]);
          STB_TM1620 =1; 
        asm("nop");
        asm("nop");	
        STB_TM1620 =0; 
		//向地址8 先数据LCDBuf[4]
        Tm1620SentData(Addr05H);
		Tm1620SentData(segNumber[6]);
	
     
		
		STB_TM1620 =1; 
         asm("nop");
       asm("nop");	
		
		
		STB_TM1620 = 0; 
       Tm1620SentData(OpenDisTM1620|Set14_16TM1620); 
		//开显示 脉宽 14/16 
		#endif 
		STB_TM1620=1; 
 
 
}
//*************************************
// 函数名称：Tm1620SentData
// 函数功能：向TM1620发数据 8位数据-1个字节的数据
// 入口参数：要操作的数据
// 出口参数：无
// 技术简要：上升沿操作一位 从低位开始操作数据
//***************************************/
void Tm1620SentData(uint8_t oneByte)
{
	uint8_t i;  
   
	
  for(i=0;i<8;i++)   
  {   
    
		 SCL = 0;//CLK_TM1620_CLR; 
		 
		 if(oneByte & (1 << i))  
		 {
       SDA = 1;//DIO_TM1620_SET;
      }   
		 else 
		 {
       SDA=0;//DIO_TM1620_CLR;
       
       }   
	
		 SCL = 1;//CLK_TM1620_SET; //上升沿操作      
  }   
 
}