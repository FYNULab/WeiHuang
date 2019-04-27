
/***************************************************************************************************    
工程名称：	smg_0~9
功能描述：	控制数码管最低位显示0~9循环变化
硬件连接：  用8位杜邦线将J8与J12连接，2位杜邦线分别将J11_0与J15_DS1、J11_1与J15_DS2连接。
维护记录：  2011-8-22
***************************************************************************************************/
#include "reg51.h"       //包含头文件

sbit LE1=P2^0;           //位选573锁存器使能
sbit LE2=P2^1;           //段选573锁存器使能


#define uchar unsigned char
#define uint  unsigned int

uchar code dis[16]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                   //  0    1    2    3    4    5    6    7    
                     0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};//0~F的段码
                   //  8    9    A    B    C    D    E    F  


//**************************************************************************************************
//延时函数
//**************************************************************************************************
delay(uint time)         //int型数据为16位,所以最大值为65535            
 {
  uint  i,j;             //定义变量i,j,用于循环语句 
  for(i=0;i<time;i++)    //for循环,循环50*time次
     for(j=0;j<50;j++);  //for循环,循环50次
 }
//**************************************************************************************************
//主函数
//**************************************************************************************************
void main()               //主函数
{
 uchar i; 
 while(1)                 //进入while死循环
  {
     P1=0x7f;             //0x7f=0111 1111,即选通数码管最低位
     LE1=1;               //锁存位
     LE1=0;               //断开锁存,位选573的Q7~Q0仍为0x7f
     
     for(i=0;i<10;i++)    //如果要实现0～F循环，只需要将10改为16
      {
       P1=dis[i];         //0~9的编码
       LE2=1;             //锁存段码
       LE2=0;             //断开锁存,段选573的Q7~Q0仍为dis[i]
       delay(5000);       //延时保持一下
      }
   }         
}