#include "system.h"

u32 SystemTime = 0;	//1ms加1

void SystemInit(void)
{
	SystemClockInit();
//	SystemDebugSet();	//这个函数会关闭JTAG口，只保留SWD调试口
}

void SystemClockInit(void)
{
	//开外部振荡器
	RCC->CR |= 1<<16;
	
	//等待外部时钟稳定
	while( (RCC->CR & (1<<17)) == 0);
	
	//设置外部时钟为锁相环输入时钟
	RCC->CFGR |= 1<<16;
	
	//设置锁相环分频和倍频值
	RCC->CFGR &= ~(0x0000000f<<18);
	RCC->CFGR |= (7<<18);	//9倍频
	
	//使能锁相环
	RCC->CR |= 1<<24;
	
	//等待锁相环输出稳定
	while( (RCC->CR & (1<<25)) == 0);
	
	// APB Low speed（APB1） clock,不能超过36M
	RCC->CFGR |= 1<<10;
	
	//flash等待状态设置，2个等待周期
	FLASH->ACR |= 2;	//two wait states, if 48 MHz < SYSCLK ≤ 72 MHz

	//设置系统时钟为锁相环输出时钟
	RCC->CFGR |= 2;
	
	//等待系统时钟切换为锁相环输出时钟
	while( (RCC->CFGR & 0x0000000c) != 0x00000008);
}

void SystemDebugSet(void)	//关闭JTAG，保留SW，释放3个引脚
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;	//使能引脚复用设置
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;//只开启SW接口，释放部分JTAG引脚，如PA15，PB3，PB4
	
}

void Delay1ms(u32 t)
{
	u32 temp = SystemTime;
	if(SystemTime < 6003)
	{
		while((SystemTime-temp)<t);
	}
	else
	{
		while(SystemTime-6002);
	}
}
