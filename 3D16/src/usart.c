#include "usart.h"
#include "stdio.h"
USARTControl USART1Control;
USARTControl USART2Control;
USARTControl USART3Control;

const u8 HexTable[]="0123456789ABCDEF";

//默认为异步，软件控制，1位起始位，8位数据，1位停止位
void USARTInit(USART_TypeDef * USART,u32 Baud)
{
	Baud *= 16;
	switch((u32)USART)
	{
		case (u32)USART1:
		{
			RCC->APB2ENR |= RCC_APB2ENR_USART1EN;	//使能USART1时钟
			USART->BRR = ((APB2CLOCK/Baud)<<4) | ((APB2CLOCK%Baud)*16/Baud);	//设置波特率
			//配置引脚为复用功能
			RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	//开A口时钟
			GPIOA->CRH = (GPIOA->CRH & 0xfffff00f) | (0x000008b0);	//设置A9,A10为USART1_TX和USART1_RX
			GPIOA->ODR |= 1<<10;
			
			USART1Control.Sending = 0;
			USART1Control.SendCount = 0;
			USART1Control.Receiving = 0;
			USART1Control.ReceiveCount = 0;
			
		}break;
		case (u32)USART2:
		{
			RCC->APB1ENR |= RCC_APB1ENR_USART2EN;	//使能USART2时钟
			USART->BRR = ((APB1CLOCK/Baud)<<4) | ((APB1CLOCK%Baud)*16/Baud);	//设置波特率
			//配置引脚为复用功能
			RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	//开A口时钟
			GPIOA->CRL = (GPIOA->CRL & 0xffff00ff) | (0x00008b00);	//设置A2,A3为USART2_TX和USAR2_RX
			GPIOA->ODR |= 1<<3;
			
			USART2Control.Sending = 0;
			USART2Control.SendCount = 0;
			USART2Control.Receiving = 0;
			USART2Control.ReceiveCount = 0;
			
		}break;
		case (u32)USART3:
		{
			RCC->APB1ENR |= RCC_APB1ENR_USART3EN;	//使能USART3时钟
			USART->BRR = ((APB1CLOCK/Baud)<<4) | ((APB1CLOCK%Baud)*16/Baud);	//设置波特率
			//配置引脚为复用功能
			RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;	//开B口时钟
			GPIOB->CRH = (GPIOB->CRH & 0xffff00ff) | (0x00008b00);	//设置B10,B11为USART3_TX和USAR3_RX
			GPIOB->ODR |= 1<<11;
			
			USART3Control.Sending = 0;
			USART3Control.SendCount = 0;
			USART3Control.Receiving = 0;
			USART3Control.ReceiveCount = 0;
			
		}break;
		default:;
	}
	
	USART->CR1 |= USART_CR1_UE;	//使能USART
	USART->CR1 |= USART_CR1_TE;	//使能发送
	USART->CR1 |= USART_CR1_RE;	//使能接收
	
//	USART->CR2;	//默认
//	USART->CR3;	//默认
//	USART->GTPR;//默认
	
}

void USARTSendByte(USART_TypeDef * USART,u8 value)
{
	while((USART->SR & USART_SR_TXE) == 0);	//等待数据寄存器为空
	USART->DR = value;
	while((USART->SR & USART_SR_TC) == 0);	//等待发送完成
	USART->SR &= ~(USART_SR_TC);	//清除发送完成标志
}

u8 USARTGetByte(USART_TypeDef * USART)
{
	while((USART->SR & USART_SR_RXNE) == 0);	//等待接收数据
	return USART->DR;
}

void USARTSendStr(USART_TypeDef * USART,u8* str,u16 lenth)
{
	if(lenth)
	{
		while(lenth--)
		{
			USARTSendByte(USART,*str++);
		}
	}
	else
	{
		while(*str)
		{
			USARTSendByte(USART,*str++);
		}
	}
}


void USARTSendByteInt(USART_TypeDef * USART,u8 value)
{
	switch((u32)USART)
	{
		case (u32)USART1:
		{
			while(USART1Control.Sending);	//等待串口占用被释放
			USART1Control.Sending = 1;		//设置占用标志位
			USART1Control.SendCount = 1;	//设置要发送的字节数
		}break;
		case (u32)USART2:
		{
			while(USART2Control.Sending);
			USART2Control.Sending = 1;
			USART2Control.SendCount = 1;
		}break;
		default:;
		case (u32)USART3:
		{
			while(USART3Control.Sending);
			USART3Control.Sending = 1;
			USART3Control.SendCount = 1;
		}break;
	}
	USART->DR = value;	//载入要发送的数据
}

void USARTSendStrInt(USART_TypeDef * USART,u8* str,u16 lenth)
{
	switch((u32)USART)
	{
		case (u32)USART1:
		{
			while(USART1Control.Sending);	//等待串口占用被释放
			USART1Control.Sending = 1;		//设置占用标志位
			if(lenth)	//如果指定了数据长度，按指定长度发送。如果没指定数据长度，自动计算字符串长度
			{
				USART1Control.SendCount = lenth;	
			}
			else
			{
				USART1Control.SendCount = strlen((char*)str);
			}
			USART1Control.SendBuffer = str;				//设置发送缓冲区指针
			USART1->DR = *USART1Control.SendBuffer++;	//载入第一个数据，并向后移动指针
		}break;
		case (u32)USART2:
		{
			while(USART2Control.Sending);
			USART2Control.Sending = 1;
			if(lenth)
			{
				USART2Control.SendCount = lenth;	
			}
			else
			{
				USART2Control.SendCount = strlen((char*)str);
			}
			USART2Control.SendBuffer = str;
			USART2->DR = *USART2Control.SendBuffer++;
		}break;
		case (u32)USART3:
		{
			while(USART3Control.Sending);
			USART3Control.Sending = 1;
			if(lenth)
			{
				USART3Control.SendCount = lenth;	
			}
			else
			{
				USART3Control.SendCount = strlen((char*)str);
			}
			USART3Control.SendBuffer = str;
			USART3->DR = *USART3Control.SendBuffer++;
		}break;
		default:;
	}
}


//函数只开启TCIE和RXNEIE中断
void EnableUSARTInt(USART_TypeDef * USART)
{
	switch((u32)USART)
	{
		case (u32)USART1:
		{
			NVIC_EnableIRQ(USART1_IRQn);	//允许内核接收USART1中断
		}break;
		case (u32)USART2:
		{
			NVIC_EnableIRQ(USART2_IRQn);	//允许内核接收USART2中断
		}break;
		case (u32)USART3:
		{
			NVIC_EnableIRQ(USART3_IRQn);	//允许内核接收USART2中断
		}break;
		default:;
	}
	USART->CR1 |= USART_CR1_TCIE;	//使能发送完成中断
	USART->CR1 |= USART_CR1_RXNEIE;	//使能接收完成中断
}

//函数只关闭TCIE和RXNEIE中断
void DisableUSARTInt(USART_TypeDef * USART)
{
	switch((u32)USART)
	{
		case (u32)USART1:
		{
			NVIC_DisableIRQ(USART1_IRQn);	//禁止内核接收USART1中断
		}break;
		case (u32)USART2:
		{
			NVIC_DisableIRQ(USART2_IRQn);	//禁止内核接收USART2中断
		}break;
		case (u32)USART3:
		{
			NVIC_DisableIRQ(USART2_IRQn);	//禁止内核接收USART2中断
		}break;
		default:;
	}
	USART->CR1 &= ~USART_CR1_TCIE;		//禁止发送完成中断
	USART->CR1 &= ~USART_CR1_RXNEIE;	//禁止接收完成中断
}


void USARTSendHex(USART_TypeDef * USART,u8 value)
{
	static u8 HexBuf[5] = {'0','X','0','0',' '};
	if((u32)USART == (u32)USART1)
	{
		while(USART1Control.Sending);	//等待串口1占用被释放
		HexBuf[2] = HexTable[value/16];
		HexBuf[3] = HexTable[value%16];
		USARTSendStrInt(USART1,HexBuf,5);
	}
	else if((u32)USART == (u32)USART2)
	{
		while(USART2Control.Sending);	//等待串口2占用被释放
		HexBuf[2] = HexTable[value/16];
		HexBuf[3] = HexTable[value%16];
		USARTSendStrInt(USART2,HexBuf,5);
	}
	else if((u32)USART == (u32)USART3)
	{
		while(USART3Control.Sending);	//等待串口2占用被释放
		HexBuf[2] = HexTable[value/16];
		HexBuf[3] = HexTable[value%16];
		USARTSendStrInt(USART3,HexBuf,5);
	}
	
}


void USART1_IRQHandler(void)
{
	if(USART1->SR & USART_SR_TC)	//判断数据是否发送完成
	{
		USART1->SR &= ~USART_SR_TC;		//清除发送完成标志位
		if(USART1Control.SendCount>1)	//如果发送计数器大于1，说明还有数据待发送
		{
			USART1Control.SendCount--;
			USART1->DR = *USART1Control.SendBuffer++;	//载入待发送的数据，指针后移
		}
		else
		{
			USART1Control.SendCount = 0;	//
			USART1Control.Sending = 0;		//清除串口占用标志
		}
	}
	if(USART1->SR & USART_SR_RXNE)	//判断是否接收到数据
	{
		USART1->SR &= ~USART_SR_RXNE;	//清除读数据寄存器非空标志位
		//放置接收的处理代码，例如：
//		USART1Control.Receiving = 1;
//		USART1Control.ReceiveCount++;
//		USART1Control.ReceiveBuffer[USART1Control.ReceiveCount] = USART1->DR;
	}
}

void USART2_IRQHandler(void)
{
	if(USART2->SR & USART_SR_TC)	//判断数据是否发送完成
	{
		USART2->SR &= ~USART_SR_TC;		//清除发送完成标志位
		if(USART2Control.SendCount>1)	//如果发送计数器大于1，说明还有数据待发送
		{
			USART2Control.SendCount--;
			USART2->DR = *USART2Control.SendBuffer++;	//载入待发送的数据，指针后移
		}
		else
		{
			USART2Control.SendCount = 0;
			USART2Control.Sending = 0;		//清除串口占用标志
		}
	}
	if(USART2->SR & USART_SR_RXNE)	//判断是否接收到数据
	{
		USART2->SR &= ~USART_SR_RXNE;	//清除读数据寄存器非空标志位
		//放置接收的处理代码，例如：
//		USART2Control.Receiving = 1;
//		USART2Control.ReceiveCount++;
//		USART2Control.ReceiveBuffer[USART1Control.ReceiveCount] = USART1->DR;
	}
}

void USART3_IRQHandler(void)
{
	if(USART3->SR & USART_SR_TC)	//判断数据是否发送完成
	{
		USART3->SR &= ~USART_SR_TC;		//清除发送完成标志位
		if(USART3Control.SendCount>1)	//如果发送计数器大于1，说明还有数据待发送
		{
			USART3Control.SendCount--;
			USART3->DR = *USART3Control.SendBuffer++;	//载入待发送的数据，指针后移
		}
		else
		{
			USART3Control.SendCount = 0;
			USART3Control.Sending = 0;		//清除串口占用标志
		}
	}
	if(USART3->SR & USART_SR_RXNE)	//判断是否接收到数据
	{
		USART3->SR &= ~USART_SR_RXNE;	//清除读数据寄存器非空标志位
		//放置接收的处理代码，例如：
//		USART3Control.Receiving = 1;
//		USART3Control.ReceiveCount++;
//		USART3Control.ReceiveBuffer[USART3Control.ReceiveCount] = USART1->DR;
	}
}

int fputc(int ch, FILE *f)
{
	USARTSendByte(USART1, (unsigned char) ch);
	return (ch);
}














