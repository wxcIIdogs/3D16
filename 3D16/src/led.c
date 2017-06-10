#include "led.h"

//����LED���Դ�,����˫����
u16 DisBuf[2][256] = {0};
//DisBuf[1][0]-[1][15]:�������һ������Դ�
//DisBuf[1][240]-[1][255]:�������һ������Դ�


//��ʼ��LED����ؿ�������
void LEDInit(void)
{
	u8 i;
	for(i=0;i<16;i++)
	{
		//��ʼ��RD0-RD15Ϊ��������0
		GPIOInit(GPIOD,i,GPIO_OUT,GPIO_LOW);
		//��ʼ��L0-L15Ϊ��������0
		GPIOInit(GPIOE,i,GPIO_OUT,GPIO_LOW);
		
	}
	
	//��ʼ��GA��GB��GC��GDΪ��������0
	for(i=0;i<4;i++)
    {
		GPIOInit(GPIOC,i,GPIO_OUT,GPIO_LOW);
    }
	
	//��ʼ��154EN����
	GPIOInit(GPIOB,9,GPIO_OUT,GPIO_LOW);
}

const u16 MapTable[16]=
{
	1<<15,1<<14,1<<13,1<<12,
	1<<11,1<<10,1<<9,1<<8,
	1<<7,1<<6,1<<5,1<<4,
	1<<3,1<<2,1<<1,1<<0
};

//��16��߶���Ϣת��Ϊ�Դ��е�����
void UpdateDisplayBuf(u8 *high)
{
	u16 i,j;
	
	for(i=0;i<256;i++)	//�����ʱ�Դ�
    {
    	DisBuf[0][i] = 0;
    }
	for(i=0;i<16;i++)
	{
		high[i] = high[i]>16?16:high[i];	//��ֹ���ݳ���16
		/* for(j=0;j<high[i];j++)			//���ݸ߶�ֵ�޸���ʱ�Դ�
		{
			DisBuf[0][16*j+i] |= 0x0001;
		} */
		//������ʾ��Ϊ����
		for(j=0;j<high[i];j++)			//���ݸ߶�ֵ�޸���ʱ�Դ�
		{
			DisBuf[0][16*j] |= MapTable[i];
		}
		
	}
	
	for(i=0;i<256;i++)	//������ʱ�Դ浽��ʽ�Դ�,ȡ�����������Ͳ�����λ��
    {
    	DisBuf[1][i] = ~DisBuf[0][i];
    }
}

//�����Դ�ˢ��LED
//ÿ����һ��ˢ��һ����,�ڶ�ʱ���ж���1ms����һ��
void LEDRefresh(void)
{
	static u16 CNT_Surface=0;
	u16 i;
	
	Disable154();		//��ֹ154���
	
	//ˢ��һ���������
	for(i=0;i<16;i++)	//��һ�����16������д������
    {
		GPIOWritePort(PORT_RD,DisBuf[1][CNT_Surface*16+i]);	//д��һ������������
		
    	GPIOSetBit(PORT_L,i);	//�������ݽ���������
		__ASM("NOP");__ASM("NOP");__ASM("NOP");__ASM("NOP");__ASM("NOP");
		GPIOResetBit(PORT_L,i);	//��������
    }
	
	//ѡ��һ����
	PORT_G->ODR = (PORT_G->ODR & 0xfffffff0) | (CNT_Surface & 0x000f);
	
	Enable154();	//ʹ��154���
	
	CNT_Surface++;	//�������1
	if(CNT_Surface >= 16)	//����16�㣬�ٻص���1��
	{
		CNT_Surface = 0;
	}
}

