#include "adc.h"
#include "timer.h"

u16 ADData[65];
u8 CollectionFinish = 0;

//AD使用定时器2触发转换，转换完成后，DMA自动将数据存到ADData[64]里面，完成64次转换后，置位采集完成标志
void ADCInit(void)
{
	RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;	//ADC_CLOCK:APB2CLK/6=16M
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;	//开启ADC1时钟
	ADC1->CR2 |= ADC_CR2_ADON;	//打开ADC
	ADC1->CR1 |= (ADC_CR1_DUALMOD_1|ADC_CR1_DUALMOD_2);	//序列转换模式
	ADC1->CR1 |= ADC_CR1_DISCEN;	//Discontinuous mode on regular channels enabled
	ADC1->CR2 |= ADC_CR2_EXTTRIG;	//Conversion on external event enabled
	ADC1->CR2 |= ADC_CR2_EXTSEL_0 | ADC_CR2_EXTSEL_1;	//Timer2 CC2 event trig
	ADC1->CR2 |= ADC_CR2_DMA;	//DMA mode enabled
	ADC1->CR2 |= ADC_CR2_RSTCAL;
	ADC1->CR2 |= ADC_CR2_CAL;
	while(ADC1->CR2 & ADC_CR2_CAL);	//等待校准完成
	
	ADC1->SQR3 |= 6;		//ADC channe1
	
	//将PA6引脚设置为AD输入脚
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL &= ~(0x0000000f<<(4*6));		//初始化AD引脚
	
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	DMA1_Channel1->CPAR = (u32)&(ADC1->DR);
	DMA1_Channel1->CMAR = (u32)ADData;
	DMA1_Channel1->CNDTR = 65;
	DMA1_Channel1->CCR |= (1<<10) | (1<<8);	//16bit
	
	DMA1_Channel1->CCR |= DMA_CCR1_MINC|DMA_CCR1_TCIE;	//内存地址增长，传输完成产生中断
	
	NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	
	DMA1_Channel1->CCR |= DMA_CCR1_EN;	//使能DMA
}

void DMA1_Channel1_IRQHandler(void)
{
	TimeStop();
	
	DMA1_Channel1->CCR &= ~DMA_CCR1_EN;	//禁能DMA
	
	DMA1_Channel1->CMAR = (u32)ADData;
	DMA1_Channel1->CNDTR = 65;
	
	DMA1_Channel1->CCR |= DMA_CCR1_EN;	//使能DMA
	
	CollectionFinish = 1;
	
	DMA1->IFCR = DMA1->ISR;
}
