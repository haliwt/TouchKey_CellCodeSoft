
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
 **			������������
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
    //SCL�ߵ�ƽ�ڼ䣬SDA����һ���½��ر�ʾ��ʼ�ź�
    SET_SDA_OUT_TM1650();
    SDA = 1;      //�������ȱ���Ϊ�ߣ���ʼ�ź�Ҫ�ÿڵ��½��� 
    asm("nop"); 
    SCL = 1;        //ʱ���߱���Ϊ��            
      asm("nop"); ;//��һ�����5us����ʱ��������������    
      asm("nop"); ;
      asm("nop"); ;
      asm("nop"); ;        
     SDA = 0;        //���������ͳ����½���           
      asm("nop"); ;
      asm("nop"); ;
      asm("nop"); ;      //��ʱ һС�ᣬ��֤�ɿ����½���
      asm("nop"); ;             
    SCL = 0;        //����ʱ���ߣ���֤����������������ı�    
        
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
    //SCL�ߵ�ƽ�ڼ䣬SDA����һ�������� ��ʾֹͣ
    SET_SDA_OUT_TM1650();
  SCL= 0;
    asm("nop"); 
      asm("nop"); 
      asm("nop"); 
      asm("nop"); 
    SDA = 0;    //��֤������Ϊ�͵�ƽ
  delay_us(2);
    SCL= 1;   //�ȱ�֤ʱ����Ϊ�ߵ�ƽ
    delay_us(1);    //��ʱ �Եõ�һ���ɿ��ĵ�ƽ�ź�            
    SDA = 1;        //�����߳���������           
    delay_us(2);    //��ʱ ��֤һ���ɿ��ĸߵ�ƽ   
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
 //������һֱ����Ϊ�͵�ƽ��ʱ���߳��������ؼ�ΪӦ��
 
  SET_SDA_OUT_TM1650();
  delay_us(1);
    SDA = 0;
    delay_us(1);
    SCL= 0;
    delay_us(2);
  SCL = 1;
  delay_us(2);
    //Ӧ����ɺ� ��ʱ�������� ���������޸�
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
    //��Ӧ���෴ ��Ӧ������Ϊ�����߱��ָߵ�ƽ����
  SET_SDA_OUT_TM1650();
  delay_us(1);
    SDA = 1;
    delay_us(1);
  SCL= 0;
  delay_us(2);
    SCL = 1;
    delay_us(2);
    //���Ҫ��ʱ�������� �������ݱ仯
    SCL = 0;
}
/******************************************************************************
 ** 
 ** Function Name: IIC_Wait_Ack_TM1650(void)
 ** Function:   �ȴ�Ӧ���ź�    	
 ** Input Ref:
 ** Return Ref: 0:Ӧ�� 1����Ӧ��
 **  
 ******************************************************************************/
uint8_t IIC_Wait_Ack_TM1650(void)
{
    //Ӧ��ȴ�����
    uint8_t ackTime = 0;
    //�Ƚ�������Ҫ���ó�����ģʽ������δ���֣���Ӧ���������½���
  SCL = 0;
  SET_SDA_OUT_TM1650();
    //delay_us(1);  
  SDA = 1;//
  //delay_us(2);
  SET_SDA_IN_TM1650();//�л�Ϊ����ģʽ
  
    //ʱ��������
    SCL = 1;
    //delay_us(2);
    //�ȴ�����������Ӧ��
    while(SDA){
        //����ڸ�ʱ������δ����
        ackTime ++;
        if(ackTime > 100)
        {
            //��Ϊ��Ӧ�� ֹͣ�ź�
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
    //����һ����������
    uint8_t i;
    SCL =0;
    delay_us(1);
    SET_SDA_OUT_TM1650();
    //��ʱ���������������ݸı�
    //    SCL = 0;
    //��λ��������
    for(i = 0;i < 8; i ++)
    {
    
    if((oneByte&0x80))            //��λ��ǰ��������
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
 ** Function:     �������ʾ	
 ** Input Ref:
 ** Return Ref:
 ** 
 ******************************************************************************/
void TM1650_Set(uint8_t add,uint8_t dat) 
{
	//д�Դ����Ӹߵ�ַ��ʼд
	IIC_Start_TM1650();
  IIC_WrByte_TM1650(add); //��һ���Դ��ַ
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
	//delay_10us(10);			//��Ҫ��ʱһС��ʱ�䣬������ʾ������Ӧ
	TM1650_Set(0x48,0x31);//��ʼ��Ϊ5���Ҷȣ�����ʾ

  #if 1
//	TM1650_Set(0x68,segNumber[7]);//��ʼ��Ϊ5���Ҷȣ�����ʾ
   

	//TM1650_Set(0x6A,segNumber[8]);//��ʼ��Ϊ5���Ҷȣ�����ʾ


  //TM1650_Set(0x6C,segNumber[1]);//��ʼ��Ϊ5���Ҷȣ�����ʾ

	
  // TM1650_Set(0x6E,segNumber[2]);//��ʼ��Ϊ5���Ҷȣ�����ʾ
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
  TM1650_Set(0x68,segNumber[11]);//��ʼ��Ϊ5���Ҷȣ�����ʾ
  COM0_RUN =0;
  
  TM1650_Set(0x68,segNumber[1]);//��ʼ��Ϊ5���Ҷȣ�����ʾ
   
 
  
}