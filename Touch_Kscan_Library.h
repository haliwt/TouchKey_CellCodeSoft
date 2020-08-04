//**********************************************************
//**********************************************************
//���ļ���CTouch V1.05����
//�� �� ��:Touch_Kscan_Library.h
//����ʱ��:2016��06��29�� 09:45:50
//**********************************************************
//**********************************************************

#ifndef	_TOUCH_KSCAN_LIBRARY_H_
#define	_TOUCH_KSCAN_LIBRARY_H_


//�ӿں�������
extern void CheckTouchKey(void);	//����ɨ�������̺���,������2.5ms~5ms�ڵ���
extern void KeyStopClear();			//��ͣɨ�谴��,������а���״̬

//ÿ��������Ӧ�ڼĴ�����1��λ,�ɸ�����һһ��Ӧ
//KeyFlag[0]��Ӧ��Key7,...,Key1,Key0
//KeyFlag[1]��Ӧ��Key15,...,Key9,Key8
//������Чʱ��ӦλΪ1,��ЧʱΪ0
extern volatile unsigned char KeyFlag[2];

//��ˮģʽѡ��,��������Ϊ��ϼ�ģʽ
#define		WaterMode

//������������
#define		C_KEY_SHAKE		2

//����ⰴ������
#define		C_KEY_TOTAL		12

//ʹ�ܵİ���ͨ��
#define		C_KEY_CHANNEL	0xFFF

//���������ͨ����Ӧ�İ������,δʹ�õ�ͨ������Ϊ0x80
const unsigned char Table_KeyEnable[3][4] = {
	0x0B,			//����ͨ��1��ⰴ��12
	0x07,			//����ͨ��2��ⰴ��8
	0x03,			//����ͨ��3��ⰴ��4
	0x0A,			//����ͨ��4��ⰴ��11
	0x06,			//����ͨ��5��ⰴ��7
	0x02,			//����ͨ��6��ⰴ��3
	0x09,			//����ͨ��7��ⰴ��10
	0x05,			//����ͨ��8��ⰴ��6
	0x01,			//����ͨ��9��ⰴ��2
	0x00,			//����ͨ��10��ⰴ��1
	0x08,			//����ͨ��11��ⰴ��9
	0x04			//����ͨ��12��ⰴ��5
};

//������ѡ��,���������ߺ�����������������Ȳ���,�趨ֵ���ɵ���5
const unsigned char Table_KeyDown[] = {
	10,				//����1�������趨ֵ
	7,				//����2�������趨ֵ
	9,				//����3�������趨ֵ
	10,				//����4�������趨ֵ
	10,				//����5�������趨ֵ
	12,				//����6�������趨ֵ
	14,				//����7�������趨ֵ
	11,				//����8�������趨ֵ
	12,				//����9�������趨ֵ
	14,				//����10�������趨ֵ
	14,				//����11�������趨ֵ
	14				//����12�������趨ֵ
};






/***********************************************************
��������������޸�
***********************************************************/
unsigned int KeyResSum[C_KEY_TOTAL];
unsigned int KeyOldValue0[C_KEY_TOTAL];
unsigned int KeyOldValue1[C_KEY_TOTAL];
unsigned int KeyOldValue2[C_KEY_TOTAL];
unsigned int KeyReference[C_KEY_TOTAL];

const unsigned char KeyTimerWidth = 230;
const unsigned int KeyChannelSel = C_KEY_CHANNEL;
const unsigned char KeyTotalNumber = C_KEY_TOTAL;
const unsigned char KeyShakeCounter = C_KEY_SHAKE;

#ifdef WaterMode
const unsigned char WaterModeSel = 1;
#else
const unsigned char WaterModeSel = 0;
#endif

#endif
