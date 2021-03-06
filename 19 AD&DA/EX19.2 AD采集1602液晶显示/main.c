/***************************************************************************************************    
工程名称：	AD_1602
功能描述：  AD采集0通道的模拟电压值，通过1602液晶显示电压值。
硬件连接：  用8位杜邦线将J8与J12连接，2位杜邦线分别将J11_7与J17_SDA、J11_6与J17_SCL连接。
            用1位跳帽短接J19的AIN0与VIN。将1602液晶接口对应插接到P3接口，注意1-16脚不要插反。
维护记录：  2011-8-22
***************************************************************************************************/
#include "reg51.h"          //包含头文件
#define uchar unsigned char
#define uint  unsigned int

extern uchar Read_AD(uchar chn);
extern show();
extern InitLCD();

uchar ge,shifen;
float volt;
//**************************************************************************************************
//延时函数
//**************************************************************************************************
delay(uint time)         //int型数据为16位,所以最大值为65535            
 {
  uint  i,j;             //定义变量i,j,用于循环语句 
  for(i=0;i<time;i++)    //for循环,循环50*time次
     for(j=0;j<100;j++); //for循环,循环50次
 }
//**************************************************************************************************
//主函数
//**************************************************************************************************
void main()
{	
   uchar temp; 
   InitLCD();
   while(1)                         //死循环
    {
       temp=Read_AD(0);             //采集0通道模拟电压值
       volt=(float)temp*4.8/256;    //计算电压值，4.8是基准电压值，float将结果强制转换为浮点型
       ge=(int)volt;                //分离得到电压值的个位
       shifen=(int)(volt*10)%10;    //分离得到电压值的十分位
      
       delay(2000);                 //延时
	}
}