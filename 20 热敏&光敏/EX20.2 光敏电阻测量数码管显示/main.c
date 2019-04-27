/***************************************************************************************************    
工程名称：	Light_smg
功能描述：  实现光敏电阻测光强度，数码管低三位显示结果。
硬件连接：  用8位杜邦线将J8与J12连接，2位杜邦线分别将J11_0与J15_DS1、J11_1与J15_DS2连接,
            1位杜邦线将J19_AIN0与J15_DT连接,2位杜邦线分别将J11_7与J17_SDA、J11_6与J17_SCL连接。
维护记录：  2011-8-22
***************************************************************************************************/
#include "reg51.h"          //包含头文件
#define uchar unsigned char
#define uint  unsigned int

sbit LE1=P2^0;           //位选573锁存器使能
sbit LE2=P2^1;           //段选573锁存器使能

extern uchar Read_AD(uchar chn); 

uchar code dis[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};//0~9的段码
                   //  0    1    2    3    4    5    6    7  8    9      
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
//数码管显示函数
//**************************************************************************************************
smg_show(uchar n)
{ 
      //百位
     P1=0xdf;                 //0xbf=1101 1111,即选通个位
     LE1=1;                   //锁存位
     LE1=0;                   //断开锁存,位选573的Q7~Q0仍保持 
     P1=dis[n/100];            //dis[n/10]为0~9的编码
     LE2=1;                   //锁存段码
     LE2=0;                   //断开锁存,段选573的Q7~Q0仍保持
     delay(10);               //延时保持一下,延时过大会闪动，延时过小会有重影    

     P1=0x00;LE2=1;LE2=0;     //清除P1口数据，以免造重影
      //显示十位
     P1=0xbf;                 //0xbf=1011 1111,即选通个位
     LE1=1;                   //锁存位
     LE1=0;                   //断开锁存,位选573的Q7~Q0仍保持 
     P1=dis[(n%100)/10];            //dis[n/10]为0~9的编码
     LE2=1;                   //锁存段码
     LE2=0;                   //断开锁存,段选573的Q7~Q0仍保持
     delay(10);               //延时保持一下,延时过大会闪动，延时过小会有重影    

     P1=0x00;LE2=1;LE2=0;     //清除P1口数据，以免造重影
     //显示个位
     P1=0x7f;                 //0xbf=0111 1111,即选通十分位
     LE1=1;                   //锁存位
     LE1=0;                   //断开锁存,位选573的Q7~Q0仍保持 
     P1=dis[n%10];            //0~9的编码
     LE2=1;                   //锁存段码
     LE2=0;                   //断开锁存,段选573的Q7~Q0仍保持 
     delay(10);               //延时保持一下,延时过大会闪动，延时过小会有重影  

     P1=0x00;LE2=1;LE2=0;     //清除P1口数据，以免造重影   
          
}
//**************************************************************************************************
//主函数
//**************************************************************************************************
void main()
{	
   uchar temp;
   while(1)                         //死循环
    {
       temp=Read_AD(0);             //采集0通道模拟电压值
       smg_show(temp);              //显示光强

	}
}