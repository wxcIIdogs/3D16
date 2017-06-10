#include "adc.h"
#include "timer.h"

u16 ADData[65];
u8 CollectionFinish = 0;

//ADʹ�ö�ʱ��2����ת����ת����ɺ�DMA�Զ������ݴ浽ADData[64]���棬���64��ת������λ�ɼ���ɱ�־
void ADCInit(void)
{
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;	//ADC_CLOCK:APB2CLK/6=16M
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;	//����ADC1ʱ��
	ADC1->CR2 |= ADC_CR2_ADON;	//��ADC
	ADC1->CR1 |= (ADC_CR1_DUALMOD_1|ADC_CR1_DUALMOD_2);	//����ת��ģʽ
	ADC1->CR1 |= ADC_CR1_DISCEN;	//Discontinuous mode on regular channels enabled
	ADC1->CR2 |= ADC_CR2_EXTTRIG;	//Conversion on external event enabled
	ADC1->CR2 |= ADC_CR2_EXTSEL_0 | ADC_CR2_EXTSEL_1;	//Timer2 CC2 event trig
	ADC1->CR2 |= ADC_CR2_DMA;	//DMA mode enabled
	ADC1->CR2 |= ADC_CR2_RSTCAL;
	ADC1->CR2 |= ADC_CR2_CAL;
	while(ADC1->CR2 & ADC_CR2_CAL);	//�ȴ�У׼���
	
	ADC1->SQR3 |= 6;		//ADC channe1
	
	//��PA6��������ΪAD�����
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL &= ~(0x0000000f<<(4*6));		//��ʼ��AD����
	
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	DMA1_Channel1->CPAR = (u32)&(ADC1->DR);
	DMA1_Channel1->CMAR = (u32)ADData;
	DMA1_Channel1->CNDTR = 65;
	DMA1_Channel1->CCR |= (1<<10) | (1<<8);	//16bit
	
	DMA1_Channel1->CCR |= DMA_CCR1_MINC|DMA_CCR1_TCIE;	//�ڴ��ַ������������ɲ����ж�
	
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	
	DMA1_Channel1->CCR |= DMA_CCR1_EN;	//ʹ��DMA
}

void DMA1_Channel1_IRQHandler(void)
{
	static u32 LoopCount = 0;
	LoopCount++;
	TimeStop();
	
	DMA1_Channel1->CCR &= ~DMA_CCR1_EN;	//����DMA
	
	DMA1_Channel1->CMAR = (u32)ADData;
	DMA1_Channel1->CNDTR = 65;
	
	DMA1_Channel1->CCR |= DMA_CCR1_EN;	//ʹ��DMA
	
	CollectionFinish = 1;
	
	if(LoopCount > 6014)
	{
		DMA1_Channel1->CCR &= 0xfffe;
	}
	DMA1->IFCR = DMA1->ISR;
}
