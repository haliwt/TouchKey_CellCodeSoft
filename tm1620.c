#include "tm1620.h"


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
       
/******************************************************
  *
  *函数名称：Tm1620Dis
  *函数功能：驱动显示数据
  *入口参数：要操作的数据
  *出口参数：无
  *
//***************************************************/
void Tm1620Dis(void)
{
	
		STB_TM1620 =0 ;  
        Tm1620SentData(ModeDisTM1620); 
		//设置显示模式，6位8段--上电默认是7段8段
		STB_TM1620 =1; 
	  
    STB_TM1620=0;   
		//Tm1620SentData(AddrAutoAdd);
        Tm1620SentData(AddrFixed);//AddrFixed
		//写显示，固定定制模式
		STB_TM1620=1; 
		
    STB_TM1620=0;   
		Tm1620SentData(Addr00H);
		//指向地址0   
	
		Tm1620SentData(segNumber[1]); //写数据
	
      //写第二位 
      
  	STB_TM1620=1; 
		
    STB_TM1620=0;   
		Tm1620SentData(Addr02H);
		//指向地址0   
	
		Tm1620SentData(segNumber[2]); //写数据
	
    //写第三位
    
    STB_TM1620=1; 
		
    STB_TM1620=0;   
		Tm1620SentData(Addr04H);
		//指向地址0   
	
		Tm1620SentData(segNumber[3]); //写数据
    
    //写第四位数据
    STB_TM1620=1; 
		
    STB_TM1620=0;   
    //指向地址6 
		Tm1620SentData(Addr06H);
	  
	  Tm1620SentData(segNumber[4]); //写数据
    
     //写第五位数据
     STB_TM1620=1; 
		
     STB_TM1620=0;   
     //指向地址8 
	  	Tm1620SentData(Addr08H);
	  
	    Tm1620SentData(segNumber[5]); //写数据
      
     //写第6位数据
     STB_TM1620=1; 
		
     STB_TM1620=0;   
     //指向地址8 
	  	Tm1620SentData(Addr0AH);
	  
	    Tm1620SentData(segNumber[6]); //写数据  
      
      
      
      STB_TM1620 =1; 
      STB_TM1620 =0; 
      Tm1620SentData(OpenDisTM1620|Set12_16TM1620); 
      STB_TM1620 =1; 
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