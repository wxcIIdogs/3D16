#include "system.h"
#include "gpio.h"
#include "usart.h"
#include "timer.h"
#include "adc.h"
#include "fft.h"
#include "led.h"
#include "stdio.h"

extern u32 SystemTime;

int main(void)
{
	u16 i;
	SysTick_Config(72000);	//1ms

	USARTInit(USART1,115200);
	EnableUSARTInt(USART1);
	USARTSendStrInt(USART1,"System run to main...\r\n",0);
	
	ADCInit();
	LEDInit();
	
	Timer2Init(25);		//25us触发一次AD采样
	TimeStart();		//启动定时器，开始触发采样
	
	
	while(1)
	{
		if(CollectionFinish)	//数据采集完成
		{
			CollectionFinish = 0;
			//数据预处理
			for(i=0;i<65;i++)
			{
				dd[i].real = ADData[i];
			}
			//进行FFT运算
			//将运算结果转换为灯柱的高度
			ProcessFFT();
			//根据高度信息，刷新显存
			UpdateDisplayBuf(LEDHigh);
			USARTSendStrInt(USART1,LEDHigh,16);
			TimeStart();	//开始下一轮采集
		}
		Delay1ms(100);
		//printf("%d\r\n",CollectionFinish);
	}
}

//SysTick中断函数
void SysTick_Handler(void)
{
	SystemTime++;
	if(SystemTime <= 60000)	//60S以内，刷新LED
	{
		LEDRefresh();
	}
}

