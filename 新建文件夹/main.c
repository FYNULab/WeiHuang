
#include "key.h"       //包含头文件
#include "smg.h"


uchar keyvalue;
int timecounter=0;


//定时器中断子程序*********************************************************************************************
t0() interrupt 1  
{
	TH0 = 0xfc;
	TL0 = 0x17;
	if(++timecounter>=1000) {
	timecounter=1;	
	 display();		  }
		
}

//定时器中断初始化函数**************************************************************************************************
void T0_init(void)        //扫描频率为100HZ,即需定时1/100=00.01s
{
	TMOD = 0x01;

	TH0 = (65535-100)/256;
	TL0 = (65535-100)%256;

	TR0 = 1;
	ET0 = 1;

	EA = 1;
}
//**************************************************************************************************
//主函数
//**************************************************************************************************
void main()                        //主函数
{
     T0_init();                    //初始化定时器中断
     while(1)
	 {
		 
	 }          
}