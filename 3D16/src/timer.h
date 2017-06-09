#ifndef __TIMER_H__
#define __TIMER_H__

#include "common.h"

void Timer2Init(u32 us);
void TimeStart(void);
void TimeStop(void);
void SetTime(u32 us);

#endif
