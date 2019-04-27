//公司：北方蓝芯科技开发有限公司
//网址：www.hrbnbc.com
/*===================================================================================================  
工程名称：  Ex3 TP_MOT
功能描述：  触摸控制直流电机调速
硬件连接：  查看和修改接口定义在NBCTFT.C与TP.C中,请仔细检查接口连线后上电测试。
      ----------------------------------------
     |     --------TFT控制接线----------      |
     |	                                      |
     |	        D10~D17   接   P4             |
     |          RS        接   P35	      |
     |		RW   	  接   P34            |
     |		RD        接   P33            |
     |		CS        接   P32	      |
     |		RES       接   P31            |
     |	                                      |
     |		LE        接   P30            |
     |                                        |
     |     --------电源供电接线---------      |
     |	        GND       接   电源负极       |
     |	        VIN       接   电源正极(5V)   |
     |----------------------------------------|
     | 如果在NBC开发板上使用，以上线不需用飞  |
     | 线连接，直接插接在开发板上的12864接口  |
     | 后只需连接以下接线                     |
     |----------------------------------------|
     |     --------触摸控制接线---------      |
     |          PEN       接   P12	      |
     |		TPDO   	  接   P13            |
     |		BUSY      接   P14            |
     |		TPDI      接   P15	      |
     |		TPCS      接   P16	      |
     |	        CLK       接   P17	      |
     |     --------直流电机控制接线---------  |
     |          J18_DCMT  接   J8_0(P10)      |
      ----------------------------------------
维护记录：  2012-3-14
====================================================================================================*/

//******************包含头文件***************************
#include "io430.h"
#include "nbc430.h"
#include"NBCTFT.h"   //包含TFT驱动头文件
#include "TP.H"
#include <in430.h>    //引用本征函数需包含本头文件

//**************触摸控制端口定义********************																				   
#define Penirq  P12          //触碰触摸屏时，ADS7843Penirq引脚由未触摸时的高电平变为低电平
#define Dcout   P10          //定义电机控制IO

//******************全局变量***************************
unsigned int Device_code;

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

unsigned char temp,speed,num;



//***********************************************************************************
//定时器初始化
//***********************************************************************************
TimerA_init()
{
     //定时器A设置
     CCTL0 = CCIE;                  //设置捕获/比较控制寄存器中CCIE位为1，CCR0捕获/比较功能中断为允许。
     CCR0 = 4;                      //实现4x(1/4096)=0.001S秒的定时,捕获/比较控制寄存器CCR0初值为4。
     TACTL = TASSEL_1+ID_3 + MC_1;  //时钟源选择为ACLK(32768HZ)、8分频、增计数模式(0-CCR0溢出),32768/8=4096 HZ.
     _EINT();                       //开总中断
}
//***********************************************************************************
//中断服务子程序
//***********************************************************************************
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A0 (void)
{
	_DINT();     	        //关总中断	
	if(num==speed)Dcout=0;  //如果进入中断speed次时，Dcout置低，占空比=speed/20,speed越大,则转速越快
         num++;                 //次数加1
         if(num==20)            //满20次为一周期，即T=20次*1ms=20ms
          {
           num=0;               //重置num为0,开始下一周期
           Dcout=1;             //Dcout置高
          }
        _EINT();                //开总中断
}

//**************************************************************************************************
//TFT界面初始化
//**************************************************************************************************
void init_interface()
{
    CLR_Screen(Blue);                                 //用背景色清屏

    Show_RGB(14,46,272,304,Magenta);                  //0
    LCD_PutString(28,280,"0",Black,Magenta);

    Show_RGB(74,166,272,304,Magenta);                 //Enter
    LCD_PutString(100,280,"Enter",Black,Magenta);


    Show_RGB(14,46,208,240,Magenta);                  //1
    LCD_PutString(28,216,"1",Black,Magenta);

    Show_RGB(14,46,144,176,Magenta);                  //4
    LCD_PutString(28,152,"4",Black,Magenta);

    Show_RGB(14,46,80,112,Magenta);                   //7
    LCD_PutString(28,88,"7",Black,Magenta);



    Show_RGB(74,106,208,240,Magenta);                 //2
    LCD_PutString(88,216,"2",Black,Magenta);

    Show_RGB(74,106,144,176,Magenta);                 //5
    LCD_PutString(88,152,"5",Black,Magenta);

    Show_RGB(74,106,80,112,Magenta);                  //8
    LCD_PutString(88,88,"8",Black,Magenta);



    Show_RGB(134,166,208,240,Magenta);                //3
    LCD_PutString(148,216,"3",Black,Magenta);

    Show_RGB(134,166,144,176,Magenta);                //6
    LCD_PutString(148,152,"6",Black,Magenta);

    Show_RGB(134,166,80,112,Magenta);                 //9
    LCD_PutString(148,88,"9",Black,Magenta);



    Show_RGB(194,226,80,176,Magenta);                 //+
    LCD_PutString(206,120,"+",Black,Magenta);

    Show_RGB(194,226,208,304,Magenta);                //-
    LCD_PutString(206,248,"-",Black,Magenta);



    LCD_PutString(44,16,"NBC电机控制演示系统",White,Blue);
    LCD_PutString(32,40,"The speed now is : ___",White,Blue);

                             
    LCD_PutChar(192, 38, 0x30+speed, Red,Blue);       //显示开机速度等级值
} 

//**************************************************************************************************
//触摸键盘扫描控制函数
//**************************************************************************************************
void key_scan()
{
    if((lx>14)&&(lx<46)&&(ly>272)&&(ly<304))           //按下0
	{
	  Show_RGB(14,46,272,304,Red);                     //按下部分变色突出显示
	  LCD_PutString(28,280,"0",Black,Red);      
  
	  temp=0;                                          //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);  //显示速度等级值

	  while((P1IN&0x04)==0x00);                                //等待触摸释放
	  Show_RGB(14,46,272,304,Magenta);                 //释放后恢复显示颜色
          LCD_PutString(28,280,"0",Black,Magenta);

	}



	if((lx>14)&&(lx<46)&&(ly>208)&&(ly<240))           //按下1
	{
	  Show_RGB(14,46,208,240,Red);                     //按下部分变色突出显示
          LCD_PutString(28,216,"1",Black,Red);
  
	  temp=1;                                          //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);  //显示速度等级值

	  while((P1IN&0x04)==0x00);                        //等待触摸释放
	  Show_RGB(14,46,208,240,Magenta);                 //释放后恢复显示颜色
          LCD_PutString(28,216,"1",Black,Magenta); 

	}


      if((lx>74)&&(lx<106)&&(ly>208)&&(ly<240))            //按下2
	{
	  Show_RGB(74,106,208,240,Red);                    //按下部分变色突出显示
          LCD_PutString(88,216,"2",Black,Red);
  
	  temp=2;                                          //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);  //显示速度等级值

	  while((P1IN&0x04)==0x00);                        //等待触摸释放
	  Show_RGB(74,106,208,240,Magenta);                //释放后恢复显示颜色
          LCD_PutString(88,216,"2",Black,Magenta); 

	}



    if((lx>134)&&(lx<166)&&(ly>208)&&(ly<240))              //按下3
	{
	  Show_RGB(134,166,208,240,Red);                    //按下部分变色突出显示
      LCD_PutString(148,216,"3",Black,Red);
  
	  temp=3;                                          //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutString(192,38,"3",Black,Magenta);         //显示速度等级值

	  while((P1IN&0x04)==0x00);                        //等待触摸释放
	  Show_RGB(134,166,208,240,Magenta);               //释放后恢复显示颜色
          LCD_PutString(148,216,"3",Black,Magenta);

	}



	if((lx>14)&&(lx<46)&&(ly>144)&&(ly<176))           //按下4
	{
	  Show_RGB(14,46,144,176,Red);                     //按下部分变色突出显示
      LCD_PutString(28,152,"4",Black,Red);
  
	  temp=4;                                          //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta); //显示速度等级值

	  while((P1IN&0x04)==0x00);                       //等待触摸释放
	  Show_RGB(14,46,144,176,Magenta);                //释放后恢复显示颜色
          LCD_PutString(28,152,"4",Black,Magenta);

	}



	if((lx>74)&&(lx<106)&&(ly>144)&&(ly<176))           //按下5
	{
	  Show_RGB(74,106,144,176,Red);                     //按下部分变色突出显示    
          LCD_PutString(88,152,"5",Black,Red);

	  temp=5;                                           //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //显示速度等级值

	  while((P1IN&0x04)==0x00);                         //等待触摸释放
	  Show_RGB(74,106,144,176,Magenta);                 //释放后恢复显示颜色                
          LCD_PutString(88,152,"5",Black,Magenta);

	}




	if((lx>134)&&(lx<166)&&(ly>144)&&(ly<176))          //按下6
	{
	  Show_RGB(134,166,144,176,Red);                    //按下部分变色突出显示    
          LCD_PutString(148,152,"6",Black,Red);

	  temp=6;                                           //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //显示速度等级值

	  while((P1IN&0x04)==0x00);                         //等待触摸释放
	  Show_RGB(134,166,144,176,Magenta);                //释放后恢复显示颜色 
          LCD_PutString(148,152,"6",Black,Magenta);

	}



	if((lx>14)&&(lx<46)&&(ly>80)&&(ly<112))             //按下7
	{
	  Show_RGB(14,46,80,112,Red);                       //按下部分变色突出显示    
          LCD_PutString(28,88,"7",Black,Red);

	  temp=7;                                           //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //显示速度等级值

	  while((P1IN&0x04)==0x00);                         //等待触摸释放
	  Show_RGB(14,46,80,112,Magenta);                   //释放后恢复显示颜色 
          LCD_PutString(28,88,"7",Black,Magenta);

	}



	if((lx>74)&&(lx<106)&&(ly>80)&&(ly<112))            //按下8
	{
	  Show_RGB(74,106,80,112,Red);                      //按下部分变色突出显示    
          LCD_PutString(88,88,"8",Black,Red);

	  temp=8;                                           //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //显示速度等级值

	  while((P1IN&0x04)==0x00);                         //等待触摸释放
	  Show_RGB(74,106,80,112,Magenta);                  //释放后恢复显示颜色 
          LCD_PutString(88,88,"8",Black,Magenta);

	}



	if((lx>134)&&(lx<166)&&(ly>80)&&(ly<112))           //按下9
	{
	  Show_RGB(134,166,80,112,Red);                     //按下部分变色突出显示    
          LCD_PutString(148,88,"9",Black,Red);

	  temp=9;                                           //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //显示速度等级值

	  while((P1IN&0x04)==0x00);                         //等待触摸释放
	  Show_RGB(134,166,80,112,Magenta);                 //释放后恢复显示颜色
          LCD_PutString(148,88,"9",Black,Magenta);

	}





	if((lx>74)&&(lx<166)&&(ly>272)&&(ly<304))           //按下Enter
	{                  
	  Show_RGB(74,166,272,304,Red);                     //按下部分变色突出显示 
          LCD_PutString(100,280,"Enter",Black,Red);

	  speed=temp;                                       //速度等级赋给speed
	  LCD_PutChar(192, 38, 0x30+speed, Red,Blue);       //显示速度等级值

	  while((P1IN&0x04)==0x00);                         //等待触摸释放
	  Show_RGB(74,166,272,304,Magenta);                 //释放后恢复显示颜色
          LCD_PutString(100,280,"Enter",Black,Magenta);

	}



	
	if((lx>194)&&(lx<226)&&(ly>80)&&(ly<176))           //按下+
	{
	  Show_RGB(194,226,80,176,Red);                     //按下部分变色突出显示    
          LCD_PutString(206,120,"+",Black,Red);
     
	  if(speed<9)speed++;                               //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+speed, Red,Blue);       //显示速度等级值

	  while((P1IN&0x04)==0x00);                         //等待触摸释放
	  Show_RGB(194,226,80,176,Magenta);                 //释放后恢复显示颜色
          LCD_PutString(206,120,"+",Black,Magenta);

	}



	if((lx>194)&&(lx<226)&&(ly>208)&&(ly<304))          //按下-
	{
	  Show_RGB(194,226,208,304,Red);                    //按下部分变色突出显示    
          LCD_PutString(206,248,"-",Black,Red);
     
	  if(speed>0)speed--;                               //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+speed, Red,Blue);       //显示速度等级值

	  while((P1IN&0x04)==0x00);                         //等待触摸释放
	  Show_RGB(194,226,208,304,Magenta);                //释放后恢复显示颜色
          LCD_PutString(206,248,"-",Black,Magenta);

	}
} 

//**************************************************************************************************
//时钟初始化
//**************************************************************************************************
/*3种时钟源:1、DCO：内部RC振荡器 2、XTAL1（LFXTAL1）:片外时精确低频时32768Hz,产生ACLK.
          3、XTAL2：片外时钟2，精确高频时钟8MHz,产生MCLK和SMCLK。
输出时钟: 1、MCLK：系统主时钟 2、SMCLK：子系统时钟 3、ACLK：辅助时钟
系统上电时，默认使用DCO时钟做为MCLK，SMCLK；使用XTAL1做为ACLK。XTAL2没有启振，没有
被使用。系统时钟可以选择使用MCK，SMCLK，ACLK其中的一种。需要系统启动后，用软件设置。*/
void init_clock()
{
    uchar i;
    BCSCTL1 &= ~XT2OFF;  //控制XT2振荡器开启(XT2OFF=0)
    BCSCTL2 |=SELM1+SELS;//选择主要时钟 MCLK:XTAL2, SMCLK:XTAL2,不分频
    //清除振荡器错误中断标志，系统启动时不稳定造成的，必须要复位
    do
    {
        IFG1 &= ~OFIFG; //清除振荡器失效标志
        i = 255;        
        while(i--);     //延时等待至少50us
    }while(IFG1&OFIFG); //判断XT2是否起振
}

//***********************************************************************************
//IO初始化操作
//***********************************************************************************
void IO_init(void)
{
	P4DIR=0xff;		  //设置P4为输出
	P4=0xff;                  //设置P4初始化为高电平
	
	P3DIR=0xff;		  //设置P3为输出
	P3=0xff;                  //设置P3初始化为高电平

        P1DIR=0xff;		  //设置P1为输出
	P1=0xff;                  //设置P1初始化为高电平
}

//================================================================================================
//	函数名称：	主函数
//	实现功能：	触摸控制直流电机调速
//	参数：		无
//	返回值：	无
//================================================================================================
main()
{	
        Device_code=0x9320;
        WDTCTL = WDTPW + WDTHOLD;// Stop watchdog timer to prevent time out reset  
        init_clock();
        IO_init();               //IO初始化 
        
	speed=5;

	TFT_Initial();      //tft初始化
	start_7843();       //初始化触摸控制IC
	TimerA_init();	    //定时器初始化
	init_interface();   //初始化人机控制界面
    
   	while(1)
	{
    	 if(Getpix()==1)key_scan();    //扫描触摸按键实现电机调速
        }
}



