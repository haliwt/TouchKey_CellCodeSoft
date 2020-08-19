#ifndef __TM1620_H__
#define __TM1620_H__
#include <cms.h>
#include <stdio.h>
#include "input.h"

#define SDA 	RA0   //Iinput
#define SCL     RA2  //OUTPUT 



#define STB_TM1620  	    RD0	               //片选信号

#define ModeDisTM1620      0x02
		//设置显示模式，6位8段--上电默认是7段8段
#define AddrAutoAdd		   0x40//写显示，自动累加地址  
#define AddrFixed 		   0x44//写显示，固定地址  
 

#define Addr01H  			0xC1//地址01H
#define Addr02H  			0xC3//地址02H
#define Addr03H  			0xC5//地址03H
#define Addr04H  			0xC7//地址04H
#define Addr05H  			0xC9//地址05H
#define Addr06H  			0xCB//地址06H

 
 
#define OpenDisTM1620  0x88//开显示
#define CloseDisTM1620 0x80//关显示
 
#define Addr00H  			0xC0//地址00H
#define Addr01H  			0xC1//地址01H
#define Addr02H  			0xC2//地址02H
#define Addr03H  			0xC3//地址03H
#define Addr04H  			0xC4//地址04H
#define Addr05H  			0xC5//地址05H
#define Addr06H  			0xC6//地址06H
#define Addr07H  			0xC7//地址07H
#define Addr08H  			0xC8//地址08H
#define Addr09H  			0xC9//地址09H
#define Addr0AH  			0xCA//地址0AH
#define Addr0BH  			0xCB//地址0BH


#define OpenDisTM1620  0x88//开显示
#define CloseDisTM1620 0x80//关显示
 
#define Set1_16TM1620 0X00//设置脉冲宽度为 1/16 
#define Set2_16TM1620 0X01//设置脉冲宽度为 2/16 
#define Set4_16TM1620 0X02//设置脉冲宽度为 4/16 
#define Set10_16TM1620 0X03//设置脉冲宽度为 10/16 
#define Set11_16TM1620 0X04//设置脉冲宽度为 11/16 
#define Set12_16TM1620 0X05//设置脉冲宽度为 12/16 
#define Set13_16TM1620 0X06//设置脉冲宽度为 13/16 
#define Set14_16TM1620 0X07//设置脉冲宽度为 14/16 

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
//extern const unsigned char segNumber[];

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





void  delay_14us(uint16_t n);
void Tm1620Dis(void);
void Tm1620SentData(uint8_t TM1620Data);


#endif 