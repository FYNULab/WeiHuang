
/***************************************************************************************************    
工程名称：	smg
功能描述：	控制数码管最低位显示字母"A"
硬件连接：  用8位杜邦线将J8与J12连接，2位杜邦线分别将J11_0与J15_DS1、J11_1与J15_DS2连接。
维护记录：  2011-8-22
***************************************************************************************************/
#include "reg51.h"       //包含头文件

sbit LE1=P2^0;           //位选573锁存器使能
sbit LE2=P2^1;           //段选573锁存器使能


#define uchar unsigned char
#define uint  unsigned int

//**************************************************************************************************
//主函数
//**************************************************************************************************
void main()               //主函数
{ 
 while(1)                 //进入while死循环
  {
     P1=0x7f;             //0x7f=0111 1111,即选通数码管最低位
     LE1=1;               //锁存位
     LE1=0;               //断开锁存,位选573的Q7~Q0仍为0x7f

     P1=0x33;             //0x77为"A"的编码
     LE2=1;               //锁存段码
     LE2=0;               //断开锁存,段选573的Q7~Q0仍为0x77
   }         
}