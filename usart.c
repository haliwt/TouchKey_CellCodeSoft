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
    
	uint8_t i,pro=0,arr[4];
    uint8_t bcc_data;
    uint8_t MarkCode[2]={0xAA,0xAB};
    //CRC_data = CRC8(arr, 3);
	bcc_data=BCC(arr,3);
	arr[3]=bcc_data;

	
	while(pro==0){
		if(gBaudTime==1){ //??104us =13us*8=104
			gBaudTime=0;
			interflag ++ ;
			if(interflag >=1 && interflag <=55){ //??6???48bit
				    if(interflag <=8){
				    	PortTx =MarkCode[0] & 0x01; //???????0xAA
						MarkCode[0]>> 1;
				    }
                    if(interflag ==9)PortTx = 1; //停止位
				    else if(interflag >9 && interflag <=17){
						PortTx =arr[0] & 0x01; //?????? head code
						arr[0] >> 1;
					}
                    if(interflag ==18)PortTx = 1; //停止位
					else if(interflag>18 && interflag <= 26){
						PortTx =arr[1] & 0x01; //??????  wind speed code hight
						arr[1] >> 1;
					}
                    if(interflag ==27)PortTx = 1; //停止位
					else if(interflag >27 && interflag <=35){
						PortTx =arr[2] & 0x01; //??????  wind speed code hight
						arr[2] >> 1;
					}
                    if(interflag ==36)PortTx = 1; //停止位
					else if(interflag >37 && interflag <=45){ //BCC ?????
						PortTx =arr[3] & 0x01; //??????   crc value
						 arr[3]>> 1;
					}
                    if(interflag ==46)PortTx = 1; //停止位
					else if(interflag >46 && interflag <54){ //?????????0xAB
						PortTx =MarkCode[1] & 0x01; //?????? head code
						MarkCode[1]>> 1;
					}
                    if(interflag ==55)PortTx = 1; //停止位
            }
			if(interflag ==56){//????
			   gBaudTime = 0;
			   interflag =0;
			   pro =1;
			}

		}
	}
}
/*************************************************************************
 	*
	*Function Name: uint8_t BCC(uint8_t *sbytes,uint8_t width)
	*Function : BCC checksum code
	*Input Ref: 
	*Output Ref:No
	*
******************************************************************************/
uint8_t BCC(uint8_t *sbytes,uint8_t width)
{
     uint8_t i;
	 uint8_t tembyte = sbytes[0];
    for (i = 1; i <width; i++) {
        tembyte ^= sbytes[i];
    }
    return tembyte;

}

/*************************************************************************
 	*
	*Function Name: USART1_ReciveData(uint8_t data)
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


void VirtualCOM_ByteSend(uint8_t Data)
{
    int j ;
	uint8_t i=8;
	gBaudTime =0;
    PortTx= 0;
	
	               // 发送起始位
    while (gBaudTime !=1);
      gBaudTime =0;
    while(i--)  	  // 发送8位数据位
     {
        if (Data & 0x01)
           PortTx=1;       // 1
        else
            PortTx=0;        // 0
       
        while (gBaudTime !=1);
		gBaudTime =0;
		Data= Data >> 1;
		
    }
	PortTx=1 ;              // 发送结束位
	 while (gBaudTime !=1);
	gBaudTime =0;
    
    
    

}