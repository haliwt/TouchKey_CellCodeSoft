#include "input.h"

void GPIO_Init(void)
{

	WPUA = 0B00000000;				//����������� 1-ʹ������   0-��ֹ����
	WPUB = 0B00000000;

	WPUD = 0B00000000;
	
	TRISA = 0B00000001;				//����IO״̬ 
	TRISB = 0B00000000;     		//GPIO B ���
	
	TRISD = 0B00000000;
	PORTD = 0B10000011;
	
}