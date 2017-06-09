#include "timer.h"

void Timer2Init(u32 us)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->ARR = APB1CLOCK/500000*us-1;	//设置周期
	
	TIM2->CCMR1 |= 7<<12 | 1<<11;	//CH2:PWM mode 2
//	TIM2->CCMR2 |= 7<<4 | 1<<3;	//CH3:PWM mode 2
	TIM2->CCR2 = TIM2->ARR>>2;	//设置占空比
//	TIM2->CCR3 = TIM2->ARR>>2;	//设置占空比
	
	TIM2->CCER |= 1<<4 ;	//使能通道2输出
//	TIM2->CCER |= 1<<4 | 1<<8;	//使能通道2和通道3的输出

	
//	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
//	GPIOA->CRL = ( GPIOA->CRL & (~(0x0000000f<<8))) | (0x0000000b<<8);
}
void TimeStart(void)
{
	TIM2->CR1 |= TIM_CR1_CEN;
}

void TimeStop(void)
{
	TIM2->CR1 &= ~TIM_CR1_CEN;
}

void SetTime(u32 us)
{
	TIM2->ARR = APB1CLOCK/500000*us-1;
	TIM2->CCR2 = TIM2->ARR>>1;
}

//void TimerInit(TIM_TypeDef* TIMER)
//{
//	switch((u32)TIMER)
//	{
//		case (u32)TIM1:
//		{
//			RCC->APB1ENR |= RCC_APB2ENR_TIM1EN;
//		}break;
//		case (u32)TIM2:
//		{
//			RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
//		}break;
//		case (u32)TIM3:
//		{
//			RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
//		}break;
//		case (u32)TIM4:
//		{
//			RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
//		}break;
//		default:;
//	}
//}




