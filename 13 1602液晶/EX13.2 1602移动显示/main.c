/***************************************************************************************************    
工程名称：	1602
功能描述：  实现1602液晶显示，液晶第一行显示"  Welcome  To  "，第二行显示" www.hfwhdz.com "。
硬件连接：  将1602液晶接口对应插接到P4接口，注意1-16脚不要插反（有些厂家的液晶顺序是反的）。
维护记录：  2011-8-22
***************************************************************************************************/
#include "reg51.h"          //包含头文件
#define uchar unsigned char
#define uint  unsigned int

sbit rs=P2^5;    //命令/数据选择
sbit rw=P2^4;    //读写口
sbit  e=P2^3;    //锁存控制

uchar data  dis1[16]={"   Welcome To   "};     //显示两行
uchar data  dis2[16]={" www.hfwhdz.com "};

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
//向LCD写一命令
//**************************************************************************************************
wcode(uchar t)
{
  rs=0;           // 写的是命令
  rw=0;           // 写状态
  e=1;            //使能
  P0=t;           //写入命令 
  delay(20);      //等待写入,如果时间太短，会导致液晶无法显示
  e=0;            //数据的锁定
}
//**************************************************************************************************
//向LCD写一数据
//**************************************************************************************************
wdata(uchar t)
{
  rs=1;          // 写的是数据
  rw=0;          // 写状态
  e=1;           //使能
  P0=t;          //写入数据
  delay(20);     //等待写入,如果时间太短，会导致液晶无法显示
  e=0;           //数据的锁定
}
//**************************************************************************************************
//LCD显示第一行
//**************************************************************************************************
xian1()
{
  uchar i;
  wcode(0x80);          //设置第一行显示地址
  for(i=0;i<16;i++)     //循环16次，写完1行
    {
      wdata(dis1[i]);   //写入该行数据
    }
}
//**************************************************************************************************
//LCD显示第二行
//**************************************************************************************************
xian2()
{
   uchar i;
   wcode(0xc0);        //设置第二行显示地址
   for(i=0;i<16;i++)   //循环16次，写完1行
    {
      wdata(dis2[i]);  //写入该行数据
    }
}
//**************************************************************************************************
//LCD 初始化
//**************************************************************************************************
InitLCD()
   {  		 
   wcode(0x01);	  //清屏
   wcode(0x06);   //输入方式控制,增量光标不移位
   wcode(0x0e);   //显示开关控制
   wcode(0x38);   //功能设定:设置16x2显示，5x7显示,8位数据接口     	
   }  
//**************************************************************************************************
//主函数
//**************************************************************************************************
main()
{  
   uchar i;
   InitLCD();	   //初始化1602  
   xian1();	       //显示第一行	   
   xian2();        //显示第二行		   
   while(1) 	   //进入死循环，防止看门狗复位
    {
     for(i=0;i<16;i++)
       {
        wcode(0x18); //每输入一次码，向左移位一次
        delay(3000); //滚动速度设置
       }
    }
}
