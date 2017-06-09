#ifndef __LED_H__
#define __LED_H__

#include "common.h"
#include "gpio.h"

#define PORT_L	GPIOE
#define PORT_G	GPIOC
#define PORT_RD	GPIOD

#define Enable154()		GPIOResetBit(GPIOB,9)
#define Disable154()	GPIOSetBit(GPIOB,9)

void LEDInit(void);
void UpdateDisplayBuf(u8 *high);
void LEDRefresh(void);

#endif
