#ifndef	__COMMON_H__
#define	__COMMON_H__

#include "stm32f10x.h"                  // Device header
#include "math.h"
#include "string.h"

/*	官方头文件已定义
#define	u8	unsigned	char
#define	s8	char
#define	u16	unsigned	short
#define	s16	short
#define	u32	unsigned	int
#define	s32	int
*/

#define AHBCLOCK	72000000
#define APB1CLOCK	36000000
#define	APB2CLOCK	72000000

#define DEBUG 	(1)

// user's define

typedef struct
{
	u8 TrigType;	//触发类型
	s16 Level;		//触发电平
	u8 AxisType;	//坐标轴显示类型
	u8 XInterval;	//横轴间隔,pixs
	u8 YInterval;	//纵轴间隔,pixs
	
	u16 ProportionV;	//电压比例,mV/Pix
	u16 ProPortionT;	//时间比例,us/Pix
	
	u32 SamplingRate;	//采样率,Hz
	float Gain;	//电压增益
}OSCControlType;

typedef struct
{
	u8 ValidFlag;	//数据有效标志位，针对周期信号
	s16 Vtop;		//顶部参考电压，mV
	s16 Vbase;		//底部参考电压，mV
	s16 Amplitude;	//振幅,mV
	s16	VoltageMax;	//电压最大值,mV
	s16 VoltageMid;	//电压平均值,mV
	s16 VoltageMin;	//电压最小值,mV
	s16 Vpp;		//电压峰峰值,mV
	u32 RiseTime;	//上升时间,us
	u32 FallTime;	//下降时间,us
	u32 Frequency;	//频率,Hz
	u32 Width;		//正脉宽,us
	u32 Period;		//周期,us
	u16 Duty;		//占空比,%
}SignalInfoType;

typedef struct
{
	u8 Page;
	u8 Row;
	u8 Setting;
}MenuType;


#endif
