#ifndef	__SYSTEM_H__
#define	__SYSTEM_H__

#include "common.h"

//AHB:8*12=96M,APB1=48M,APB2=96M

void SystemInit(void);
void SystemClockInit(void);
void SystemDebugSet(void);
void Delay1ms(u32 t);

//extern u32 SystemTime;
extern u8 KeyValue;
extern u32 KeyTime;

#endif
