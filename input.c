#include "input.h"
#include "tm1620.h"

void GPIO_Init(void)
{

	WPUA = 0B00000000;				//配置上拉情况 1-使能上拉   0-禁止下拉
	WPUB = 0B00000000;

	WPUD = 0B00000000;
	
	TRISA = 0B00000000;				//配置IO状态 
	TRISB = 0B11111111;     		//KEY inpute GPIO
	
	TRISD = 0B10000000;             //RD7 IR input 
	
	
}
/****************************************************
    *
	*Function Name :IR_Output(void)
	*Function : IR output 38KHZ frequency ,period = 26us
	*
	*
	*
****************************************************/
void IR_ReadData(void)
{
    static uint8_t irflg =0;
    uint8_t irtimes = 0;
    irflg = irflg ^ 0x01;
    
   
             if(IR_RE_DATA == 0){
             	 delay_10us(100);
                if(BKLT_R == 1 && BKLT_L == 1 && irtimes !=1){
                
		       	     
						  keystr.BackLed_On=1;
			              irtimes =0;
						  BKLT_R = 0;
					      BKLT_L =0;
					      irtimes =1;
			   	  }
			   	   else if(irtimes !=1){
			   	      		irtimes = 1;
			   	      		BKLT_R = 1;
				        	BKLT_L =1;
				         	keystr.BackLed_On=0;
				         	irtimes = 1;

			   	      }
				}	 
			            
}