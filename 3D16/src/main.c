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
	
	Timer2Init(25);		//25us����һ��AD����
	TimeStart();		//������ʱ������ʼ��������
	
	
	while(1)
	{
		if(CollectionFinish)	//���ݲɼ����
		{
			CollectionFinish = 0;
			//����Ԥ����
			for(i=0;i<65;i++)
			{
				dd[i].real = ADData[i];
			}
			//����FFT����
			//��������ת��Ϊ�����ĸ߶�
			ProcessFFT();
			//���ݸ߶���Ϣ��ˢ���Դ�
			UpdateDisplayBuf(LEDHigh);
			USARTSendStrInt(USART1,LEDHigh,16);
			TimeStart();	//��ʼ��һ�ֲɼ�
		}
		Delay1ms(100);
		//printf("%d\r\n",CollectionFinish);
	}
}

//SysTick�жϺ���
void SysTick_Handler(void)
{
	SystemTime++;
	if(SystemTime <= 60000)	//60S���ڣ�ˢ��LED
	{
		LEDRefresh();
	}
}

