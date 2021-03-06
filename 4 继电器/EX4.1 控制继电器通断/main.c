//公司：北方蓝芯科技开发有限公司
//网址：www.hrbnbc.com
//更多教程可登陆技术论坛www.hrbnbc.com/bbs对应版块下载，
//所有教程一律免费下载
/***************************************************************************************************    
工程名称：	K2
功能描述：	控制继电器通断，同时发出"啪啪"声
硬件连接：  用1位杜邦线将J11_7与J18_K2连接
维护记录：  2011-8-22
***************************************************************************************************/
#include "reg51.h"       //包含头文件

sbit K2=P2^7;            //定义继电器控制IO

#define uchar unsigned char
#define uint  unsigned int

//**************************************************************************************************
//延时函数
//**************************************************************************************************
delay(uint time)              //int型数据为16位,所以最大值为65535            
 {
  uint  i,j;                  //定义变量i,j,用于循环语句 
  for(i=0;i<time;i++)         //for循环,循环50*time次
     for(j=0;j<50;j++);       //for循环,循环50次
 }

//**************************************************************************************************
//主函数
//**************************************************************************************************
void main()                   //主函数
{
     while(1)                 //进入while死循环
      {
        K2=0;                 //断开继电器
        delay(5000);          //延时
        K2=1;                 //导通继电器
        delay(5000);          //延时
      }         
}