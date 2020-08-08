#include "input.h"

void GPIO_Init(void)
{

	WPUA = 0B00000000;				//配置上拉情况
	WPUB = 0B00000000;

	WPUD = 0B00000000;
	
	TRISA = 0B00000000;				//配置IO状态 
	TRISB = 0B00000000;     		//GPIO B 输出
	
	TRISD = 0B00000000;
}