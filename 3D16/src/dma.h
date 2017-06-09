#ifndef __DMA_H__
#define __DMA_H__

#include "common.h"

#define M2M	(0x01)	//Memory to memory mode
#define M2P	(0x02)	//Memory to Peripheral mode
#define P2M	(0x03)	//Peripheral to memory mode

#define P8M8	(0x10)
#define P8M16	(0x20)
#define P8M32	(0x30)
#define P16M8	(0x40)
#define P16M16	(0x50)
#define P16M32	(0x60)
#define P32M8	(0x70)
#define P32M16	(0x80)
#define P32M32	(0x90)


void DMAInit(u8 Channel,u8 DMAMode,u8 DataWidth);

void StartDMA(u32* PeripheralAddress,u32* MemoryAddress,u16 Count);

#endif
