#include "gpio.h"

//////////////////////////////////////////////////////
/*
GPIO初始化函数
依次输入：端口，引脚号，方向（输入输出），初始状态（无拉电阻，上、下拉）
*/

void GPIOInit(GPIO_TypeDef * Port,u8 Index,u8 Direction,u8 Status)
{
	u8 temp;	//
	u32* CRP;	//指向CRL或CRH寄存器
	switch((u32)Port)
	{
		case (u32)GPIOA:
		{
			RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	//开A口时钟
		}break;
		case (u32)GPIOB:
		{
			RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;	//开B口时钟
		}break;
		case (u32)GPIOC:
		{
			RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;	//开C口时钟
		}break;
		case (u32)GPIOD:
		{
			RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;	//开D口时钟
		}break;
		case (u32)GPIOE:
		{
			RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;	//开E口时钟
		}break;
		
		default:;
	}
	
	if(Index<8)
	{
		temp = Index<<2;
		CRP = (u32*)&(Port->CRL);
	}
	else
	{
		temp = (Index-8)<<2;
		CRP = (u32*)&(Port->CRH);
	}
	
	if(Direction)	//判断IO口方向
	{
		if( ((u32)Port == (u32)GPIOC) && (Index == 13) )
		{
			*CRP = (*CRP & (~(0x0000000f<<temp))) | (0x00000002<<temp);		//设置为输出，推挽模式,最大速度2MHz
		}
		else
		{
			*CRP = (*CRP & (~(0x0000000f<<temp))) | (0x00000003<<temp);		//设置为输出，推挽模式,最大速度50MHz
		}
		if(Status)	//设置引脚输出相应电平
		{
			Port->ODR |= 1<<Index;
		}
		else
		{
			Port->ODR &= ~(1<<Index);
		}
	}
	else
	{

		*CRP &= ~(0x00000003<<temp);	//设置为输入模式
		
		if(Status==GPIO_UP)	//设置上下拉电阻
		{
			*CRP = (*CRP & (~(0x0000000c<<temp))) | (0x00000008<<temp);	//使能上下拉功能
			Port->ODR |= 1<<Index;	//设置上拉
		}
		else if(Status==GPIO_DOWN)
		{
			*CRP = (*CRP & (~(0x0000000c<<temp))) | (0x00000008<<temp);	//使能上下拉功能
			Port->ODR |= 1<<Index;	//设置下拉
		}
		else if(Status==GPIO_NOPULL)
		{
			*CRP = (*CRP & (~(0x0000000c<<temp))) | (0x00000004<<temp);	//悬空输入，无上下拉
		}
		
	}
}

//读端口
u32 GPIOReadPort(GPIO_TypeDef* Port)
{
	return Port->IDR;
}

//写端口
void GPIOWritePort(GPIO_TypeDef* Port,u32 GPIOData)
{
	Port->ODR = GPIOData & 0x0000ffff;
}

//读某个引脚
u8 GPIOReadBit(GPIO_TypeDef* Port,u8 Index)
{
	return (Port->IDR & (1<<Index))?1:0;
}

//写某个引脚
void GPIOWriteBit(GPIO_TypeDef* Port,u8 Index,u8 GPIOData)
{
	if(GPIOData)
	{
		Port->BSRR = 1<<Index;
	}
	else
	{
		Port->BSRR = 1<<(16+Index);
	}
}

//将某个引脚清0
void GPIOResetBit(GPIO_TypeDef* Port,u8 Index)
{
	Port->BSRR = 1<<(16+Index);
}

//将某个引脚置1
void GPIOSetBit(GPIO_TypeDef* Port,u8 Index)
{
	Port->BSRR = 1<<Index;
}

//将某个引脚电平翻转
void GPIOTurnBit(GPIO_TypeDef* Port,u8 Index)
{
	Port->ODR ^= 1<<Index;
}
