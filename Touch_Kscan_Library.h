//**********************************************************
//**********************************************************
//此文件由CTouch V1.05创建
//文 件 名:Touch_Kscan_Library.h
//创建时间:2016年06月29日 09:45:50
//**********************************************************
//**********************************************************

#ifndef	_TOUCH_KSCAN_LIBRARY_H_
#define	_TOUCH_KSCAN_LIBRARY_H_


//接口函数声明
extern void CheckTouchKey(void);	//按键扫描主过程函数,建议在2.5ms~5ms内调用
extern void KeyStopClear();			//暂停扫描按键,清除所有按键状态

//每个按键对应于寄存器的1个位,由高至低一一对应
//KeyFlag[0]对应于Key7,...,Key1,Key0
//KeyFlag[1]对应于Key15,...,Key9,Key8
//按键有效时相应位为1,无效时为0
extern volatile unsigned char KeyFlag[2];

//防水模式选择,不定义则为组合键模式
#define		WaterMode

//按键消抖次数
#define		C_KEY_SHAKE		2

//待检测按键总数
#define		C_KEY_TOTAL		12

//使能的按键通道
#define		C_KEY_CHANNEL	0xFFF

//各触摸检测通道对应的按键序号,未使用的通道设置为0x80
const unsigned char Table_KeyEnable[3][4] = {
	0x0B,			//触摸通道1检测按键12
	0x07,			//触摸通道2检测按键8
	0x03,			//触摸通道3检测按键4
	0x0A,			//触摸通道4检测按键11
	0x06,			//触摸通道5检测按键7
	0x02,			//触摸通道6检测按键3
	0x09,			//触摸通道7检测按键10
	0x05,			//触摸通道8检测按键6
	0x01,			//触摸通道9检测按键2
	0x00,			//触摸通道10检测按键1
	0x08,			//触摸通道11检测按键9
	0x04			//触摸通道12检测按键5
};

//灵敏度选择,调整长短线后再用这个修正灵敏度差异,设定值不可低于5
const unsigned char Table_KeyDown[] = {
	8,				//按键1灵敏度设定值  //定时器
	6,				//按键2灵敏度设定值  //up key
	6,				//按键3灵敏度设定值  //down 可有
	7,//10,				//按键4灵敏度设定值 //run key
	7,//10,				//按键5灵敏度设定值  //setup key 
	7,//12,				//按键6灵敏度设定值
	7,//14,				//按键7灵敏度设定值
	11,				//按键8灵敏度设定值
	12,				//按键9灵敏度设定值
	14,				//按键10灵敏度设定值
	14,				//按键11灵敏度设定值
	14				//按键12灵敏度设定值
};






/***********************************************************
下面的内容请勿修改
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
