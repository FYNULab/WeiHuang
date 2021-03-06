/***************************************************************************************************    
工程名称：	HC595_led
功能描述：	数据通过138译码输出，驱动8位发光二极管实现流水灯。
硬件连接：  用8位线将J13与J20连接，4位线分别将J8_0与J21_A、J8_1与J21_B、J8_2与J21_C、J8_3与J21_EN连接
维护记录：  2011-8-22
***************************************************************************************************/
#include "reg51.h"       //包含头文件
sbit HC138_A=P1^0;       //表示用SH_CP等效于P1^5, P1^0就是指头文件里定义的P1寄存器的第5BIT
sbit HC138_B=P1^1;        
sbit HC138_C=P1^2; 
sbit HC138_EN=P1^3;        

#define uchar unsigned char
#define uint  unsigned int
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
void main()             //主函数
{
 uchar  i,j;            //定义变量i,j;
 while(1)               //进入while死循环
  {                     
   j=0x01;              //0x01即为二进制0000 0001,即最低位为1,其他位为0
   for(i=0;i<8;i++)     //进入for循环,循环8次
     {
      P1=i;             //i变化从0-7，依次赋给P1的低3位HC138_A、HC138_B、HC138_C，实现LED逐位点亮
      HC138_EN=1;       //HC138_EN置高，译码数据
      delay(5000);      //延时，修改延时即可修改时间间隔 
     }    
  }         
}