#include "input.h"

void GPIO_Init(void)
{

	WPUA = 0B00000000;				//配置上拉情况 1-使能上拉   0-禁止下拉
	WPUB = 0B00000000;

	WPUD = 0B00000000;
	
	TRISA = 0B00000001;				//配置IO状态 
	TRISB = 0B00000000;     		//GPIO B 输出
	
	TRISD = 0B00000000;
	PORTD = 0B10000011;
	
}