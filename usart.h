#ifndef __USART_H__
#define __USART_H__
#include <cms.h>
#include "input.h"
#include "tm1620.h"


#define PortTx    RA1

volatile uint8_t gBaudTime;  //g--globe全局变量 
 //extern volatile uint8_t gBaudTime ;  //WT.EDIT 2020.08.24//
void SendData_0(void);
void SendData_1(void);

void WriteByte( uint8_t sdata ) ;//波特率9600

void USART_SendData(uint8_t data);//模拟串口函数

uint8_t BCC(void);

void VirtualCOM_ByteSend(uint8_t Data);


#endif 