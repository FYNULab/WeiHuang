/***************************************************************************************************    
工程名称：	hw_1602
功能描述：  将卡片遮挡红外对管，1602液晶显示遮挡次数，每挡一次，次数加1。 
硬件连接：  将1602液晶接口对应插接到P4接口，注意1-16脚不要插反（有些厂家的液晶顺序是反的）。
维护记录：  2011-8-22
***************************************************************************************************/
#include "reg51.h"          //包含头文件
#define uchar unsigned char
#define uint  unsigned int

sbit rs=P2^5;        //命令/数据选择
sbit rw=P2^4;        //读写口
sbit  e=P2^3;        //锁存控制
sbit duiguan=P2^0;   //红外对管
 uchar speed; 
 sbit S20=P3^6;           //定义S20控制IO，电机转速加大
sbit S18=P3^7;           //定义S18控制IO，电机转速减小
sbit Dcout=P1^0;     
uchar data  dis1[16]={" the number is: "};     //第一行
uint Data[10] = {0x0030,0x0031,0x0032,0x0033,0x0034,0x0035,0x0036,0x0037,0x0038,0x0039}; 

uchar num;
//**************************************************************************************************
//延时函数
//**************************************************************************************************
delay(uint time)         //int型数据为16位,所以最大值为65535            
 {
  uint  i,j;             //定义变量i,j,用于循环语句 
  for(i=0;i<time;i++)    //for循环,循环50*time次
     for(j=0;j<100;j++); //for循环,循环50次
 }
 t0() interrupt 1 using 1
{
  TH0=0xfc;               //重置定时器初值,定时1ms
  TL0=0x18;
   if(num==speed)Dcout=0; //如果进入中断speed次时，Dcout置低，占空比=speed/20,speed越大,则转速越快
  num++;                  //次数加1
   if(num==20)            //满20次为一周期，即T=20次*1ms=20ms
    {
     num=0;               //重置num为0,开始下一周期
     Dcout=1;             //Dcout置高
    }
}
T0_init()
{
   TMOD=0x01;    //0b00000001  用的是定时器0，工作在方式1（16位寄存器）
                 //要实现定时1ms,(65536-T0)x12/12000000=0.001s得T0=15536即0xfc18
   TH0=0xfc;     //装定时器初值高8位
   TL0=0x18;     //装定时器初值低8位
   TR0=1;        //启动定时器0
   ET0=1;        //开定时器0中断
   EA=1;         //开总中断
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
   uint shi,ge,bai;    
	if(num<10)          //1位数时
   {
   	wcode(0xcc);        //设置第二行显示地址
   	wdata(Data[num]);   //显示次数   
   }

	if(num>=10)         //2位数时
   {
   	shi=num/10;         //分离num得到十位数
	ge=num%10;          //分离num得到个位数
   	wcode(0xcc);        //设置第二行显示地址
   	wdata(Data[shi]);   //显示次数十位数 
	wdata(Data[ge]);    //显示次数个位数 

	if(num>=100)        //3位时
    {
	 
		 bai=num/100;         //分离num得到百位数	     
		 shi=num%100/10;      //分离num得到十位数	  
		 ge=num%100%10;       //分离num得到个位数
		 wcode(0xcc);         //设置第二行显示地址 
		 wdata(Data[bai]);    //显示次数百位数 
	 	 wdata(Data[shi]);    //显示次数十位数 
         wdata(Data[ge]);     //显示次数个位数  
	}                         //千位可自行添加
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
    InitLCD();          //初始化1602
    xian1();	        //显示第一行	 
    duiguan=1;          //初始化i/0口（红外对管） 
	T0_init();
   speed=10;                                  //初始化转速，占空比=speed/20=1/2
  
   	while(1)
  {
    if(duiguan==1)      //判断是否被挡住 （挡住执行）  
    {
	  delay(20);        //去抖
      while(duiguan==1);//延时后再判断 若未被挡住执行
	  num++;            //计数加1
    }
    xian2();          //显示计数

       if(S20==0)                             //按键扫描，判断加速键是否按下
         {   
           delay(20);                         //延时消抖
           while(S20==0);       //等待按键释放,按一次只能加1，屏蔽此句时，按下按键不松时，会不断递增
           if(speed<20)speed++;else speed=0;  //转速加1
         }
       
       if(S18==0)                             //按键扫描，判断减速键是否按下
         {   
           delay(20);                         //延时消抖
           while(S18==0);     //等待按键释放,按一次只能减1，屏蔽此句时，按下按键不松时，会不断递减
           if(speed>0)speed--;else speed=20;  //转速减1 
         }           
  
}
}
