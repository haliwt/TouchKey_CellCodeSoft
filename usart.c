#include "usart.h"

static uint8_t interflag;
 static uint8_t  x=0;
static void wait_int(void);
/*************************************************************************
 	*
	*Function Name: USART_SendData(uint8_t data)
	*Function : GPIO口模拟串口，波特率 =9600bps ，间隔发送时间= 1s/9600=104us
	*Input Ref: data ，需要发送的数据8bit
	*Output Ref:No
	*
******************************************************************************/
void USART_SendData(uint8_t data)
{
    
    WriteByte(0xAA) ;  //报文头 AB
	WriteByte(keystr.SendSwitchData) ; //控制参数
    WriteByte(keystr.SendWindData) ; //风速量---低字节
	WriteByte(0x00) ; //风速量--高字节
	WriteByte(keystr.SendBCCdata) ; //风速量--高字节
	WriteByte(0xAB) ; //风速量--高字节
	
	
}
/*************************************************************************
 	*
	*Function Name: uint8_t BCC(uint8_t *sbytes,uint8_t width)
	*Function : BCC checksum code
	*Input Ref: 
	*Output Ref:No
	*
******************************************************************************/
uint8_t BCC(void)
{
     uint8_t sbytes[4];
	 uint8_t i;
	 sbytes[0]=0xAA;
	 uint8_t tembyte = sbytes[0];
	 
	 sbytes[1]=keystr.SendSwitchData;
	 sbytes[2]=keystr.SendWindData;
     sbytes[3]=0x00;
	 
    for (i = 1; i <4; i++) {
        tembyte ^= sbytes[i];
    }
    return tembyte;

}

/*************************************************************************
 	*
	*Function Name: 
	*Function : GPIO口模拟串口，波特率 =9600bps
	*Input Ref:
	*Output Ref:No
	*
**************************************************************************/
void SendData_0(void)
{
   PortTx =0 ;
    
}

void SendData_1(void)
{
   PortTx =1; 
    
}

//等待中断到来
static void wait_int(void)
{
	while(gBaudTime !=1);
	gBaudTime = 0;
}
/*************************************************************************
 	*
	*Function Name: void WriteByte( uint8_t Data) 
	*Function : GPIO口模拟串口，波特率 =9600bps
	*Input Ref:
	*Output Ref:No
	*
**************************************************************************/
void WriteByte( uint8_t Data) 
{
   
	int8_t i =8,pro;
	  wait_int();
	
		
		PortTx =0;
		
		wait_int();//delay_14us(1);
	
	   
	  while(i--)
		{
			 
			
			 if (Data & 0x01)
               PortTx=1;       // 1
             else
               PortTx=0;        // 0
           
				wait_int();//delay_14us(1);//
				 Data = Data >> 1;
			}	
			
		PortTx= 1;
	    wait_int();
	  // delay_14us(1);//wait_int();
		
}


