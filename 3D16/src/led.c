#include "led.h"

//所有LED的显存,采用双缓冲
u16 DisBuf[2][256] = {0};
//DisBuf[1][0]-[1][15]:最下面的一个面的显存
//DisBuf[1][240]-[1][255]:最上面的一个面的显存


//初始化LED的相关控制引脚
void LEDInit(void)
{
	u8 i;
	for(i=0;i<16;i++)
	{
		//初始化RD0-RD15为输出，输出0
		GPIOInit(GPIOD,i,GPIO_OUT,GPIO_LOW);
		//初始化L0-L15为输出，输出0
		GPIOInit(GPIOE,i,GPIO_OUT,GPIO_LOW);
		
	}
	
	//初始化GA，GB，GC，GD为输出，输出0
	for(i=0;i<4;i++)
    {
		GPIOInit(GPIOC,i,GPIO_OUT,GPIO_LOW);
    }
	
	//初始化154EN引脚
	GPIOInit(GPIOB,9,GPIO_OUT,GPIO_LOW);
}

const u16 MapTable[16]=
{
	1<<15,1<<14,1<<13,1<<12,
	1<<11,1<<10,1<<9,1<<8,
	1<<7,1<<6,1<<5,1<<4,
	1<<3,1<<2,1<<1,1<<0
};

//将16组高度信息转换为显存中的数据
void UpdateDisplayBuf(u8 *high)
{
	u16 i,j;
	
	for(i=0;i<256;i++)	//清除临时显存
    {
    	DisBuf[0][i] = 0;
    }
	for(i=0;i<16;i++)
	{
		high[i] = high[i]>16?16:high[i];	//防止数据超过16
		/* for(j=0;j<high[i];j++)			//根据高度值修改临时显存
		{
			DisBuf[0][16*j+i] |= 0x0001;
		} */
		//更换显示面为正面
		for(j=0;j<high[i];j++)			//根据高度值修改临时显存
		{
			DisBuf[0][16*j] |= MapTable[i];
		}
		
	}
	
	for(i=0;i<256;i++)	//复制临时显存到正式显存,取反，交换亮和不亮的位置
    {
    	DisBuf[1][i] = ~DisBuf[0][i];
    }
}

//根据显存刷新LED
//每调用一次刷新一个面,在定时器中断里1ms调用一次
void LEDRefresh(void)
{
	static u16 CNT_Surface=0;
	u16 i;
	
	Disable154();		//禁止154输出
	
	//刷新一个面的数据
	for(i=0;i<16;i++)	//向一个面的16个灯柱写入数据
    {
		GPIOWritePort(PORT_RD,DisBuf[1][CNT_Surface*16+i]);	//写入一个灯柱的数据
		
    	GPIOSetBit(PORT_L,i);	//允许数据进入锁存器
		__ASM("NOP");__ASM("NOP");__ASM("NOP");__ASM("NOP");__ASM("NOP");
		GPIOResetBit(PORT_L,i);	//锁存数据
    }
	
	//选中一个面
	PORT_G->ODR = (PORT_G->ODR & 0xfffffff0) | (CNT_Surface & 0x000f);
	
	Enable154();	//使能154输出
	
	CNT_Surface++;	//层计数加1
	if(CNT_Surface >= 16)	//超过16层，再回到第1层
	{
		CNT_Surface = 0;
	}
}

