
#include "i2c.h"
#include "stdio.h"

/******************************************************************
    *
    *Function Name:void delay_us(uint16_t n)
    *Function :delay times
    *Input Ref:
    *Return Ref:
    *
*******************************************************************/
void  delay_us(uint16_t n)
{
    uint16_t i,j=0;
	for(i=0;i<n;i++){
		asm("nop");
	}
    
}
/******************************************************************
    *
    *Function Name:void delay_10us(uint16_t n)
    *Function :delay times
    *Input Ref:
    *Return Ref:
    *
*******************************************************************/
void  delay_10us(uint16_t n)
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
		
		}	
}
    
 void  delay_14us(uint16_t n)
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
    asm("nop");
    asm("nop");
    asm("nop"); //better 
    asm("nop");// below new add --standard level
    asm("nop");
   // asm("nop");
		
		}	
}
       
    



/*****************************************************************************
 ** \brief	I2C_SendMasterCmd
 **			发送主控命令
 ** \param [in] none
 ** \return  none
 ** \note	 
*****************************************************************************/

/******************************************************************************
 ** 
 ** Function Name: void IIC_Init_TM1650(void)
 ** Function:          	
 ** Input Ref:
 ** Return Ref: 
 **  
 ******************************************************************************/
void IIC_Init_TM1650(void)
{
    
	TRISA  &= ~(1<<2);		//I2C SCL 
	SCL =1;		//Output
		

	TRISA |=(1<<0)	;	//I2C SDA 
				//Input

}
/******************************************************************************
 ** 
 ** Function Name: void IIC_Start_TM1650(void)
 ** Function:          	
 ** Input Ref:
 ** Return Ref: 
 **  
 ******************************************************************************/
void IIC_Start_TM1650(void)
{
	
	  // SDA 1->0 while SCL High
    //SCL高电平期间，SDA出现一个下降沿表示起始信号
    SET_SDA_OUT_TM1650();
    SDA = 1;      //数据线先保持为高，起始信号要该口的下降沿 
    asm("nop"); 
    SCL = 1;        //时钟线保持为高            
      asm("nop"); ;//有一个大概5us的延时具体以器件而定    
      asm("nop"); ;
      asm("nop"); ;
      asm("nop"); ;        
     SDA = 0;        //数据线拉低出现下降沿           
      asm("nop"); ;
      asm("nop"); ;
      asm("nop"); ;      //延时 一小会，保证可靠的下降沿
      asm("nop"); ;             
    SCL = 0;        //拉低时钟线，保证接下来数据线允许改变    
        
}

/******************************************************************************
 ** 
 ** Function Name: void I2C_Stop_TM1650(void)
 ** Function:          	
 ** Input Ref:
 ** Return Ref: 
 **  
 ******************************************************************************/
void IIC_Stop_TM1650(void)
{

	// SDA 0->1 while SCL High
    //SCL高电平期间，SDA产生一个上升沿 表示停止
    SET_SDA_OUT_TM1650();
  SCL= 0;
    asm("nop"); 
      asm("nop"); 
      asm("nop"); 
      asm("nop"); 
    SDA = 0;    //保证数据线为低电平
  delay_us(2);
    SCL= 1;   //先保证时钟线为高电平
    delay_us(1);    //延时 以得到一个可靠的电平信号            
    SDA = 1;        //数据线出现上升沿           
    delay_us(2);    //延时 保证一个可靠的高电平   
}
/******************************************************************************
 ** 
 ** Function Name: void IIC_Ack_TM1650(void)
 ** Function:          	
 ** Input Ref:
 ** Return Ref: 
 **  
 ******************************************************************************/
void IIC_Ack_TM1650(void)
{
 //数据线一直保持为低电平，时钟线出现上升沿即为应答
 
  SET_SDA_OUT_TM1650();
  delay_us(1);
    SDA = 0;
    delay_us(1);
    SCL= 0;
    delay_us(2);
  SCL = 1;
  delay_us(2);
    //应答完成后 将时钟线拉低 允许数据修改
    SCL = 0;
}
/******************************************************************************
 ** 
 ** Function Name: void IIC_NAck_TM1650(void)
 ** Function:          	
 ** Input Ref:
 ** Return Ref: 
 **  
 ******************************************************************************/
void IIC_NAck_TM1650(void)
{
    //非应答即相反 与应答区别即为数据线保持高电平即可
  SET_SDA_OUT_TM1650();
  delay_us(1);
    SDA = 1;
    delay_us(1);
  SCL= 0;
  delay_us(2);
    SCL = 1;
    delay_us(2);
    //最后要将时钟线拉低 允许数据变化
    SCL = 0;
}
/******************************************************************************
 ** 
 ** Function Name: IIC_Wait_Ack_TM1650(void)
 ** Function:   等待应答信号    	
 ** Input Ref:
 ** Return Ref: 0:应答 1：无应答
 **  
 ******************************************************************************/
uint8_t IIC_Wait_Ack_TM1650(void)
{
    //应答等待计数
    uint8_t ackTime = 0;
    //先将数据线要设置成输入模式本程序未体现，有应答则会出现下降沿
  SCL = 0;
  SET_SDA_OUT_TM1650();
    //delay_us(1);  
  SDA = 1;//
  //delay_us(2);
  SET_SDA_IN_TM1650();//切换为输入模式
  
    //时钟线拉高
    SCL = 1;
    //delay_us(2);
    //等待数据线拉低应答
    while(SDA){
        //如果在该时间内仍未拉低
        ackTime ++;
        if(ackTime > 100)
        {
            //认为非应答 停止信号
            IIC_Stop_TM1650();
            return 1;
        }
    }
    SCL = 0;
    return 0 ;
}
/******************************************************************************
 ** 
 ** Function Name: void IIC_WrByte_TM1650(uint8_t number)
 ** Function:     	
 ** Input Ref:
 ** Return Ref:
 ** 
 ******************************************************************************/
void IIC_WrByte_TM1650(uint8_t oneByte)
{
    //定义一个计数变量
    uint8_t i;
    SCL =0;
    delay_us(1);
    SET_SDA_OUT_TM1650();
    //将时钟线拉低允许数据改变
    //    SCL = 0;
    //按位发送数据
    for(i = 0;i < 8; i ++)
    {
    
    if((oneByte&0x80))            //高位在前发送数据
        SDA=1;
    else                     //  TM1650_DIO_L;
        SDA= 0;
          
        //delay_us(1);
        SCL=1;
        //delay_us(1);
        SCL=0;//TM1650_CLK_L;
       // delay_us(1);
        oneByte<<=1;        

    }
   //  delay_us(1);
}
/******************************************************************************
 ** 
 ** Function Name: void TM1650_Set(u8 add,u8 dat)
 ** Function:     数码管显示	
 ** Input Ref:
 ** Return Ref:
 ** 
 ******************************************************************************/
void TM1650_Set(uint8_t add,uint8_t dat) 
{
	//写显存必须从高地址开始写
	IIC_Start_TM1650();
  IIC_WrByte_TM1650(add); //第一个显存地址
	IIC_Ack_TM1650();
	IIC_WrByte_TM1650(dat);
	IIC_Ack_TM1650();
	IIC_Stop_TM1650();
}

/******************************************************************************
 ** 
 ** Function Name: Init_Tm1650(void)
 ** Function:    
 ** Input Ref:
 ** Return Ref:
 ** 
 ******************************************************************************/
void Init_Tm1650(void)
{
	IIC_Init_TM1650();
	//delay_10us(10);			//需要延时一小段时间，否则开显示会无响应
	TM1650_Set(0x48,0x31);//初始化为5级灰度，开显示

  #if 1
//	TM1650_Set(0x68,segNumber[7]);//初始化为5级灰度，开显示
   

	//TM1650_Set(0x6A,segNumber[8]);//初始化为5级灰度，开显示


  //TM1650_Set(0x6C,segNumber[1]);//初始化为5级灰度，开显示

	
  // TM1650_Set(0x6E,segNumber[2]);//初始化为5级灰度，开显示
    #endif 
	
}
/******************************************************************************
 ** 
 ** Function Name: void LEDDisplay_Run0(void)
 ** Function:    
 ** Input Ref:
 ** Return Ref:
 ** 
 ******************************************************************************/
void LEDDisplay_Run0(void)
{
  uint8_t disp=0;
  TM1650_Set(0x68,segNumber[11]);//初始化为5级灰度，开显示
  COM0_RUN =0;
  
  TM1650_Set(0x68,segNumber[1]);//初始化为5级灰度，开显示
   
 
  
}