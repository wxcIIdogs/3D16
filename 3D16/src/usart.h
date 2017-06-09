#ifndef __USART_H__
#define __USART_H__

#include "common.h"

typedef struct
{
	u8 Sending;
	u8* SendBuffer;
	u16 SendCount;
	u8 Receiving;
	u8* ReceiveBuffer;
	u16 ReceiveCount;
}USARTControl;

//默认为异步，软件控制，1位起始位，8位数据，1位停止位
//USART1:A9,A10为USART1_TX和USART1_RX
//USART2:A2,A3为USART2_TX和USAR2_RX
void USARTInit(USART_TypeDef * USART,u32 Baud);

//普通的串口发送函数，数据发送完才退出函数
void USARTSendByte(USART_TypeDef * USART,u8 value);
u8 USARTGetByte(USART_TypeDef * USART);
void USARTSendStr(USART_TypeDef * USART,u8* str,u16 lenth);

//使用中断的串口发送函数，函数执行完立即退出，不会等待数据发送完
void USARTSendByteInt(USART_TypeDef * USART,u8 value);
void USARTSendStrInt(USART_TypeDef * USART,u8* str,u16 lenth);


void EnableUSARTInt(USART_TypeDef * USART);		//函数只开启TCIE和RXNEIE中断
void DisableUSARTInt(USART_TypeDef * USART);	//函数只关闭TCIE和RXNEIE中断

void USARTSendHex(USART_TypeDef * USART,u8 value);

#endif
