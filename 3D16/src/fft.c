#include "fft.h"

struct Compx dd[65]; //FFT���� 
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


//�����˷�
void ee(struct Compx b1,u8 b2)
{ 
	temp.real=b1.real*iw[2*b2]-b1.imag*iw[2*b2+1];
	temp.imag=b1.real*iw[2*b2+1]+b1.imag*iw[2*b2];
}

//�˷�����  
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

//���ٸ���Ҷ�任  
void FFT(struct Compx *xin,u8 N)
{
	u8  fftnum,i,j,k,l,m,n,disbuff,dispos,dissec;
	struct Compx t;
	fftnum = N;                         //����Ҷ�任����
	for(m=1;(fftnum=fftnum/2)!=1;m++);//���M��ֵ 
	for(k=0;k<=N-1;k++)               //��λ����
	{
		n = k;
		j = 0; 
		for(i=m;i>0;i--)             //����
		{
			j=j+((n%2)<<(i-1));
			n=n/2;
		} 
		if(k<j)
		{
			t=xin[1+j];
			xin[1+j]=xin[1+k];
			xin[1+k]=t;
		}//��������
	}  
	for(l=1;l<=m;l++)                //FFT����
	{
		disbuff=mypow(2,l);          //��õ������
		dispos=disbuff/2;            //��õ�������֮��ľ���
		for(j=1;j<=dispos;j++)
		{
			for(i=j;i<N;i=i+disbuff) //����M�����еĵ���
			{
				dissec=i+dispos;     //��õڶ����λ��
				ee(xin[dissec],(u16)(j-1)*(u16)N/disbuff);//�����˷�
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
//�����ʾ���ݵ�ת������,�������������ٶȿ����Լ�������¶���
//��fft���ݽ��д���,�õ�����Ƶ�ʶεĵ�ѹ��ֵ
void ProcessFFT(void)
{ 
    u8 pt=0; 

    for(pt=1;pt<65;pt++)
    {
        dd[pt].imag=0;                //�����鲿 
    }
    FFT(dd,64);                       //�Ե�ǰ���ݽ��и���Ҷ�任
	 
    for(pt=1;pt<65;pt++)
    {                                      
        dd[pt].real=sqrt(dd[pt].real*dd[pt].real+dd[pt].imag*dd[pt].imag);//ȡ������ 
    }
	
	//�洢16��LED�ĸ߶�
	for(pt=2;pt<34;pt+=2)
	{
		LEDHigh[pt/2-1] = map(dd[pt].real,0,512,0,16);
		//LEDHigh[pt/2-1] = dd[pt].real/16;
	}
	
	
	
//ԭʼ���룺
/*	
	if(Menu==1)
	{
		for(pt=2;pt<34;pt+=2)	 
	    {       
			for(tmp=(dd[pt].real/32)+1,LEDBuf[pt]=0xFF;tmp>=1;tmp--)//tmp>1;���������λ��һ�г���
			{
			 	LEDBuf[pt]<<=1;			
			}
			
			LEDBuf[pt]=~(LEDBuf[pt]);
	 
		}
	}
	else if(Menu==2)
	{

		//�����
		for(pt=2;pt<34;pt+=2)	
	    {       
			tmp=(dd[pt].real/32)+1;
			if(RefreshFlag[pt]<tmp)		            //ˢ������,ȡ�ϴ�߶�ֵ �洢��ʾ  
			{
				for(LEDBuf[pt]=0xFF;tmp>1;tmp--)	//tmp>1;������  ���λ��һ�г���;{}һ��Ҳ��ִ�У�
				{
				 	LEDBuf[pt]<<=1;	
					
				}
				RefreshFlag[pt]=(dd[pt].real/32)+1;
			}
			else
			{
			 	if(RefreshFlag[pt]>1)RefreshFlag[pt]--;                //���������ٶȿ���  �ı�ֵ���Ըı��½��ٶ�
			 	for(LEDBuf[pt]=0xFF,tmp=RefreshFlag[pt];tmp>1;tmp--)   //tmp>1;������ ���λ��һ�г���
				{
				 	LEDBuf[pt]<<=1;						
				}
			}
		}
	} 
	else if(Menu==0)
	{

		//�����Ƶ��
		for(pt=2;pt<34;pt+=2)	 
	    {       
			tmp=(dd[pt].real/32)+1;
			if(RefreshFlag[pt]<tmp)		          //ˢ������,ȡ�ϴ�߶�ֵ �洢��ʾ  
			{
				for(LEDBuf[pt]=0xFF;tmp>=1;tmp--)//tmp>1;������  ���λ��һ�г���;{}һ��Ҳ��ִ�У�
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
