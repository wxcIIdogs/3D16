#include "fft.h"

struct Compx dd[65]; //FFT数据 
struct Compx temp;
u8 LEDHigh[16];
//u8 Menu = 1;
//u8 LEDBuf[65] = {0};
//u8 RefreshFlag[65] = {0};
 
const float iw[64]=
{
	1.000,0,0.9952,-0.0980,0.9808,-0.1951,0.9569,-0.2903,0.9239,-0.3827,0.8819,-0.4714,0.8315,-0.5556,
	0.7730,-0.6344,0.7071,-0.7071,0.6344,-0.7730,0.5556,-0.8315,0.4714,-0.8819,0.3827,-0.9239,0.2903,-0.9569,
	0.1951,-0.9808,0.0980,-0.9952,0.0,-1.0000,-0.0980,-0.9952,-0.1951,-0.9808,-0.2903,0.9569,-0.3827,-0.9239,
	-0.4714,-0.8819,-0.5556,-0.8315,-0.6344,-0.7730,-0.7071,-0.7071,-0.7730,-0.6344,-0.8315,-0.5556,-0.8819,-0.4714,
	-0.9239,-0.3827,-0.9569,-0.2903,-0.9808,-0.1951,-0.9952,-0.0980
};


//复数乘法
void ee(struct Compx b1,u8 b2)
{ 
	temp.real=b1.real*iw[2*b2]-b1.imag*iw[2*b2+1];
	temp.imag=b1.real*iw[2*b2+1]+b1.imag*iw[2*b2];
}

//乘方函数  
u16 mypow(u8 nbottom,u8 ntop)
{
    u16 result=1;
    u8 t;    
    for(t=0;t<ntop;t++)
	{
		result *= nbottom;
	}
    return result;
}

//快速傅立叶变换  
void FFT(struct Compx *xin,u8 N)
{
	u8  fftnum,i,j,k,l,m,n,disbuff,dispos,dissec;
	struct Compx t;
	fftnum = N;                         //傅立叶变换点数
	for(m=1;(fftnum=fftnum/2)!=1;m++);//求得M的值 
	for(k=0;k<=N-1;k++)               //码位倒置
	{
		n = k;
		j = 0; 
		for(i=m;i>0;i--)             //倒置
		{
			j=j+((n%2)<<(i-1));
			n=n/2;
		} 
		if(k<j)
		{
			t=xin[1+j];
			xin[1+j]=xin[1+k];
			xin[1+k]=t;
		}//交换数据
	}  
	for(l=1;l<=m;l++)                //FFT运算
	{
		disbuff=mypow(2,l);          //求得碟间距离
		dispos=disbuff/2;            //求得碟形两点之间的距离
		for(j=1;j<=dispos;j++)
		{
			for(i=j;i<N;i=i+disbuff) //遍历M级所有的碟形
			{
				dissec=i+dispos;     //求得第二点的位置
				ee(xin[dissec],(u16)(j-1)*(u16)N/disbuff);//复数乘法
				t=temp;
				xin[dissec].real=xin[i].real-t.real;
				xin[dissec].imag=xin[i].imag-t.imag;
				xin[i].real=xin[i].real+t.real;
				xin[i].imag=xin[i].imag+t.imag;
			}
		}
	}
} 

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//完成显示数据的转换工作,包括顶点下落速度控制以及顶点更新定格
//对fft数据进行处理,得到各个频率段的电压幅值
void ProcessFFT(void)
{ 
    u8 pt=0; 

    for(pt=1;pt<65;pt++)
    {
        dd[pt].imag=0;                //清零虚部 
    }
    FFT(dd,64);                       //对当前数据进行傅立叶变换
	 
    for(pt=1;pt<65;pt++)
    {                                      
        dd[pt].real=sqrt(dd[pt].real*dd[pt].real+dd[pt].imag*dd[pt].imag);//取均方根 
    }
	
	//存储16组LED的高度
	for(pt=2;pt<34;pt+=2)
	{
		LEDHigh[pt/2-1] = map(dd[pt].real,0,1024,0,16);
		//LEDHigh[pt/2-1] = dd[pt].real/16;
	}
	
	
	
//原始代码：
/*	
	if(Menu==1)
	{
		for(pt=2;pt<34;pt+=2)	 
	    {       
			for(tmp=(dd[pt].real/32)+1,LEDBuf[pt]=0xFF;tmp>=1;tmp--)//tmp>1;不保留最低位那一行常亮
			{
			 	LEDBuf[pt]<<=1;			
			}
			
			LEDBuf[pt]=~(LEDBuf[pt]);
	 
		}
	}
	else if(Menu==2)
	{

		//下落感
		for(pt=2;pt<34;pt+=2)	
	    {       
			tmp=(dd[pt].real/32)+1;
			if(RefreshFlag[pt]<tmp)		            //刷新数据,取较大高度值 存储显示  
			{
				for(LEDBuf[pt]=0xFF;tmp>1;tmp--)	//tmp>1;不保留  最低位那一行常亮;{}一次也不执行；
				{
				 	LEDBuf[pt]<<=1;	
					
				}
				RefreshFlag[pt]=(dd[pt].real/32)+1;
			}
			else
			{
			 	if(RefreshFlag[pt]>1)RefreshFlag[pt]--;                //顶端下落速度控制  改变值可以改变下降速度
			 	for(LEDBuf[pt]=0xFF,tmp=RefreshFlag[pt];tmp>1;tmp--)   //tmp>1;不保留 最低位那一行常亮
				{
				 	LEDBuf[pt]<<=1;						
				}
			}
		}
	} 
	else if(Menu==0)
	{

		//下落感频谱
		for(pt=2;pt<34;pt+=2)	 
	    {       
			tmp=(dd[pt].real/32)+1;
			if(RefreshFlag[pt]<tmp)		          //刷新数据,取较大高度值 存储显示  
			{
				for(LEDBuf[pt]=0xFF;tmp>=1;tmp--)//tmp>1;不保留  最低位那一行常亮;{}一次也不执行；
				{
				 	LEDBuf[pt]<<=1;	
					
				}
				RefreshFlag[pt]=(dd[pt].real/32)+1;
			
			}
			else
			{
			 	if(RefreshFlag[pt]>1)RefreshFlag[pt]--;                         
			 	for(LEDBuf[pt]=0xFF,tmp=RefreshFlag[pt];tmp>=1;tmp--)	
				{
				 	LEDBuf[pt]<<=1;					
				}
			}
			LEDBuf[pt]=~(LEDBuf[pt]);
		}
	}
*/	
}
