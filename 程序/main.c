#include "reg51.h"       //包含头文件

sbit LE1=P2^0;           
sbit LE2=P2^1; 
unsigned char wei, i;

unsigned char code dis[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                   //  0    1    2    3    4    5    6    7    
                     0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};//0~F的段码
                   //  8    9    A    B    C    D    E    F            

void Dislay(void)
{
	 P1=~wei;                 //由于共阴，低电平来选通某1位，所以需取反
     LE1=1;                   //锁存位
     LE1=0;                   //断开锁存,位选573的Q7~Q0仍保持 
     wei<<=i;                 //左移一位 

     P1=dis[i];               //0~7的编码
     LE2=1;                   //锁存段码
     LE2=0;                   //断开锁存,段选573的Q7~Q0仍保持
     i++;                     //i加1
     
     if(i>=8){i=0;wei=0x01;}  //共8位,扫描到第8位时，重新扫描
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







