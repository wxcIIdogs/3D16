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
	SysTick_Config(72000);	//定时1ms

	USARTInit(USART1,115200);	//初始化串口1
	EnableUSARTInt(USART1);		//使能串口1中断
	USARTSendStrInt(USART1,"System run to main...\r\n",0);
	
	ADCInit();		//初始化ADC
	LEDInit();		//初始化LED相关控制引脚
	
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
			//发送调试信息
			USARTSendStrInt(USART1,LEDHigh,16);
			TimeStart();	//开始下一轮采集
		}
		Delay1ms(100);
		//printf("%d\r\n",CollectionFinish);
	}
}


//SysTick中断函数，1ms中断
void SysTick_Handler(void)
{
	SystemTime++;
	LEDRefresh();
}
