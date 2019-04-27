/***************************************************************************************************    
工程名称：	stepmt_8
功能描述：  单、双八拍(A→AB→B→BC→C→CD→D→DA→A)方式驱动步进电机，实现电机正转。
硬件连接：  用4位杜邦线连接J11_0，J11_1，J11_2，J11_3和J18_A,J18_B,J18_C,J18_D,将步进电机连接到JP8
维护记录：  2011-8-22
***************************************************************************************************/
#include "reg51.h"       //包含头文件

sbit S_A=P2^0;           //定义步进电机A相
sbit S_B=P2^1;           //定义步进电机B相
sbit S_C=P2^2;           //定义步进电机C相
sbit S_D=P2^3;           //定义步进电机D相

#define uchar unsigned char
#define uint  unsigned int
 
uchar speed;             //电机转速
//**************************************************************************************************
//延时函数大约0.1ms
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
main()
{ 
    uint h,speed;          //定义变量                      
    speed=20;              //初始化速度
    while(1)               //死循环    
     {  
      h=512;               //循环512次，为360度
      while(h--)           //h递减至0时，循环结束
        {
             
             P2=0x01;      //等效于S_D=0,S_C=0,S_B=0,S_A=1
             delay(speed); //延时，修改此参数即可修改电机转速
             P2=0x03;      //等效于S_D=0,S_C=0,S_B=1,S_A=1
             delay(speed); 
             P2=0x02;      //i等效于S_D=0,S_C=0,S_B=1,S_A=0
             delay(speed); 
             P2=0x06;      //等效于S_D=0,S_C=1,S_B=1,S_A=0
             delay(speed); 
             P2=0x04;      //等效于S_D=0,S_C=1,S_B=0,S_A=0
             delay(speed); 
             P2=0x0c;      //等效于S_D=1,S_C=1,S_B=0,S_A=0
             delay(speed); 
             P2=0x08;      //等效于S_D=1,S_C=0,S_B=0,S_A=0
             delay(speed); 
             P2=0x09;      //等效于S_D=1,S_C=0,S_B=0,S_A=1
             delay(speed); 
         }
      
      }     
}