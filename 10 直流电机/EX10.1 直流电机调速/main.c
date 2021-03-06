//公司：北方蓝芯科技开发有限公司
//网址：www.hrbnbc.com
//更多教程可登陆技术论坛www.hrbnbc.com/bbs对应版块下载，
//所有教程一律免费下载
/***************************************************************************************************    
工程名称：	Dcmotor
功能描述：  通过按键S20和S18分别控制直流电机速度加与减,数码管显示速度等级
硬件连接：  用8位杜邦线将J10与J12连接，用2位杜邦线分别将J11_0与J15_DS1及J11_1与J15_DS2连接，
            用1位杜邦线将J8_0与J18_DCMT连接，用1位杜邦线将J9_7与J7_S18连接，
            用1位杜邦线将J9_6与J7_S20连接，将直流电机连接到JP7接口
维护记录：  2011-8-22
***************************************************************************************************/
#include "reg51.h"       //包含头文件

sbit S20=P3^6;           //定义S20控制IO，电机转速加大
sbit S18=P3^7;           //定义S18控制IO，电机转速减小
sbit Dcout=P1^0;         //定义电机控制IO
sbit LE1=P2^0;           //位选573锁存器使能
sbit LE2=P2^1;           //段选573锁存器使能

#define uchar unsigned char
#define uint  unsigned int
 
uchar num;               //记录定时中断次数
uchar speed;             //电机转速



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
//定时器中断子程序
//**************************************************************************************************
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
//**************************************************************************************************
//定时器初始化
//**************************************************************************************************
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
//数码管显示函数
//**************************************************************************************************
smg_show(uchar n)
{
  uchar i;
  for(i=0;i<10;i++)
   {
      //显示个位
     P0=0xbf;                 //0xbf=1011 1111,即选通个位
     LE1=1;                   //锁存位
     LE1=0;                   //断开锁存,位选573的Q7~Q0仍保持 
     P0=dis[n/10];            //dis[n/10]为0~9的编码
     LE2=1;                   //锁存段码
     LE2=0;                   //断开锁存,段选573的Q7~Q0仍保持
     delay(100);              //延时保持一下,延时过大会闪动，延时过小会有重影    

     P0=0x00;LE2=1;LE2=0;     //清除P1口数据，以免造重影
     //显示十分位
     P0=0x7f;                 //0xbf=0111 1111,即选通十分位
     LE1=1;                   //锁存位
     LE1=0;                   //断开锁存,位选573的Q7~Q0仍保持 
     P0=dis[n%10];            //0~9的编码
     LE2=1;                   //锁存段码
     LE2=0;                   //断开锁存,段选573的Q7~Q0仍保持 
     delay(100);              //延时保持一下,延时过大会闪动，延时过小会有重影  

     P0=0x00;LE2=1;LE2=0;     //清除P1口数据，以免造重影   
   }             
}

//**************************************************************************************************
//主函数
//**************************************************************************************************
main()
{
   T0_init();
   speed=10;                                  //初始化转速，占空比=speed/20=1/2
   while(1)                                   //死循环，等待定时器中断
     {
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
    smg_show(speed);                          //数码管最低两位显示转速
     } 
}