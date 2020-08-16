#ifndef __I2C_H__
#define __I2C_H__
#include <cms.h>
#include "input.h"


#define SDA 	RA0   //Iinput
#define SCL     RA2  //OUTPUT 
#define SET_SDA_OUT_TM1650()       (TRISA &= ~(1<<0))//(TRISA |=( 1<<0))
#define SET_SDA_IN_TM1650() 	   (TRISA |=(1<<0))

#define  TM1650_ORDER      0x48        //数字指令 写数据

/**************************************************
	*XinTong LED Display Data Data:2020.07.23

	    //                a
        //            ---------
        //           |         |
        //         f |         | b
        //           |    g    |
        //            ---------
        //           |         |
        //         e |         | c
        //           |    d    |
        //            ---------   O <- h
**************************************************/


#define   seg_a  0x01       //seg_a   //seg_e = 0x10
#define   seg_b  0x02      //seg_b , //seg_f = 0x20
#define   seg_c  0x04      //seg_c , //seg_g = 0x40
#define   seg_d  0x08      //seg_d , //seg_d = 0x08
#define   seg_e  0x10      //seg_e , //seg_c = 0x04
#define   seg_f  0x20      //seg_f   //seg_a = 0x01
#define   seg_g  0x40      //seg_g   //seg_b = 0x02 
#define   seg_h  0x80      //

const unsigned char segNumber[]={
         seg_a+seg_b+seg_c+seg_d+seg_e+seg_f,        // char "0"  0x00
         seg_b+seg_c,                                // char "1"  0x01
         seg_a+seg_b+seg_d+seg_e+seg_g,              // char "2"  0x02
         seg_a+seg_b+seg_c+seg_d+seg_g,              // char "3"  0x03
         seg_b+seg_c+seg_f+seg_g,                    // char "4"  0x04
         seg_a+seg_c+seg_d+seg_f+seg_g,              // char "5"  0x05
         seg_a+seg_c+seg_d+seg_e+seg_f+seg_g,        // char "6"  0x06
         seg_a+seg_b+seg_c+seg_f,                    // char "7"  0x07
         seg_a+seg_b+seg_c+seg_d+seg_e+seg_f+seg_g,  // char "8"  0x08
         seg_a+seg_b+seg_c+seg_d+seg_f+seg_g,        // char "9"  0x09
         seg_h,                                      // char "."  0x0A
         0                                          // Dont't display 0x0b
         
};


//extern const unsigned char segNumber[];

void IIC_Init_TM1650(void);
void IIC_Start_TM1650(void);
void IIC_Stop_TM1650(void);
void IIC_Ack_TM1650(void);
void IIC_NAck_TM1650(void);
uint8_t IIC_Wait_Ack_TM1650(void);
void IIC_WrByte_TM1650(uint8_t number);
void TM1650_Set(uint8_t add,uint8_t dat) ;
void Init_Tm1650(void);


void delay_us(uint16_t m);
void delay_10us(uint16_t n);
 void  delay_14us(uint16_t n);

void LEDDisplay_Run0(void);
void LEDDisplay_Run1(void);
void LEDDisplay_Function(uint8_t number);




#endif 
