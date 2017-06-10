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
	SysTick_Config(72000);	//��ʱ1ms

	USARTInit(USART1,115200);	//��ʼ������1
	EnableUSARTInt(USART1);		//ʹ�ܴ���1�ж�
	USARTSendStrInt(USART1,"System run to main...\r\n",0);
	
	ADCInit();		//��ʼ��ADC
	LEDInit();		//��ʼ��LED��ؿ�������
	
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
			//���͵�����Ϣ
			USARTSendStrInt(USART1,LEDHigh,16);
			TimeStart();	//��ʼ��һ�ֲɼ�
		}
		Delay1ms(100);
		//printf("%d\r\n",CollectionFinish);
	}
}


//SysTick�жϺ�����1ms�ж�
void SysTick_Handler(void)
{
	SystemTime++;
	LEDRefresh();
}
