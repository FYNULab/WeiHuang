#include "reg51.h"       //����ͷ�ļ�

sbit LE1=P2^0;           
sbit LE2=P2^1; 
unsigned char wei, i;

unsigned char code dis[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                   //  0    1    2    3    4    5    6    7    
                     0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};//0~F�Ķ���
                   //  8    9    A    B    C    D    E    F            

void Dislay(void)
{
	 P1=~wei;                 //���ڹ������͵�ƽ��ѡͨĳ1λ��������ȡ��
     LE1=1;                   //����λ
     LE1=0;                   //�Ͽ�����,λѡ573��Q7~Q0�Ա��� 
     wei<<=i;                 //����һλ 

     P1=dis[i];               //0~7�ı���
     LE2=1;                   //�������
     LE2=0;                   //�Ͽ�����,��ѡ573��Q7~Q0�Ա���
     i++;                     //i��1
     
     if(i>=8){i=0;wei=0x01;}  //��8λ,ɨ�赽��8λʱ������ɨ��
}

void Init_Timer(void)
{
	
	TMOD = 0x01;

	TH0 = 0xf8;
	TL0 = 0x2f;

	TR0 = 1;
	ET0 = 1;

	EA = 1;

}

void Timer0(void) interrupt 1
{
	
	TH0 = 0xf8;
	TL0 = 0x2f;
	
	Dislay();	
		
}

main()
{
	Init_Timer();

	while(1);
}







