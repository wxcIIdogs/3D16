#ifndef __FFT_H__
#define __FFT_H__

#include "common.h"

struct Compx
{
	float real;
	float imag;
};

extern struct Compx dd[65]; //FFT数据
extern u8 LEDHigh[16];		//FFT后的16组结果，用来指示灯高度

void ProcessFFT(void);

#endif

