#include "system.h"

u32 SystemTime = 0;	//1ms��1

void SystemInit(void)
{
	SystemClockInit();
//	SystemDebugSet();	//���������ر�JTAG�ڣ�ֻ����SWD���Կ�
}

void SystemClockInit(void)
{
	//���ⲿ����
	RCC->CR |= 1<<16;
	
	//�ȴ��ⲿʱ���ȶ�
	while( (RCC->CR & (1<<17)) == 0);
	
	//�����ⲿʱ��Ϊ���໷����ʱ��
	RCC->CFGR |= 1<<16;
	
	//�������໷��Ƶ�ͱ�Ƶֵ
	RCC->CFGR &= ~(0x0000000f<<18);
	RCC->CFGR |= (7<<18);	//9��Ƶ
	
	//ʹ�����໷
	RCC->CR |= 1<<24;
	
	//�ȴ����໷����ȶ�
	while( (RCC->CR & (1<<25)) == 0);
	
	// APB Low speed��APB1�� clock,���ܳ���36M
	RCC->CFGR |= 1<<10;
	
	//flash�ȴ�״̬���ã�2���ȴ�����
	FLASH->ACR |= 2;	//two wait states, if 48 MHz < SYSCLK �� 72 MHz

	//����ϵͳʱ��Ϊ���໷���ʱ��
	RCC->CFGR |= 2;
	
	//�ȴ�ϵͳʱ���л�Ϊ���໷���ʱ��
	while( (RCC->CFGR & 0x0000000c) != 0x00000008);
}

void SystemDebugSet(void)	//�ر�JTAG������SW���ͷ�3������
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;	//ʹ�����Ÿ�������
	AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;//ֻ����SW�ӿڣ��ͷŲ���JTAG���ţ���PA15��PB3��PB4
	
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
