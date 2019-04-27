//公司：北方蓝芯科技开发有限公司
//网址：www.hrbnbc.com
/*===================================================================================================  
工程名称：  Ex1
功能描述：  控制TFT实现汉字，字符显示，和画点功能。
硬件连接：  查看和修改接口定义在NBCTFT.C中,请仔细检查接口连线。
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
      ----------------------------------------
维护记录：  2012-3-30
====================================================================================================*/

//******************包含头文件***************************

#include "io430.h"
#include "nbc430.h"
#include"NBCTFT.h"   //包含TFT驱动头文件

//******************全局变量***************************

#define White          0xFFFF   //LCD color
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

unsigned int Device_code;      //TFT控制IC型号

//**************声明外部函数和变量********************

extern void delayms(unsigned int count);


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
}

//================================================================================================
//	函数名称：	主函数
//	实现功能：	控制TFT实现汉字，字符显示.
//	参数：		无
//	返回值：	无
//================================================================================================
main()
{
  unsigned int i;
  
  WDTCTL = WDTPW + WDTHOLD;// Stop watchdog timer to prevent time out reset  
  init_clock();
  IO_init();               //IO初始化 
  Device_code=0x9320;      //TFT控制IC型号 
  TFT_Initial();           //初始化LCD	 

  while(1)                 //循环
      {
	   //5种颜色将屏分为5个区域
	   Show_RGB(0,240,0,64,Blue);
	   Show_RGB(0,240,64,128,Green);
	   Show_RGB(0,240,128,192,Magenta);
	   Show_RGB(0,240,192,256,Red);
	   Show_RGB(0,240,256,320,Yellow);

          //第1个区域显示16x16汉字
	  LCD_PutString(24,16,"北方蓝芯科技开发有限公司",White,Blue);
          LCD_PutString(24,40,"   蓝芯科技  与您同行",White,Blue);

	   //第2个区域显示ascii字符中的数字和符号
	  LCD_PutString(80,72,"0123456789",Black,Green);
	   LCD_PutString(16,96,")(`,./<>';:[]{}|?-=+*&^%$",Black,Green);

	   //第3个区域显示ascii字符中字母
	   LCD_PutString(16,136,"abcdefghijklmnopqrstuvwxyz",Blue,Magenta);
	  LCD_PutString(16,156,"ABCDEFGHIJKLMNOPQRSTUVWXYZ",Blue,Magenta);

	   //第4个区域显示TFT控制器IC型号
	   LCD_PutString(16,200,"The Device IC Of TFT Is:",Black,Red);
	   LCD_PutString(96,224,"ILI",Black,Red);
	   LCD_PutChar(120, 224, 0x30+(Device_code>>12), Black, Red);
	   LCD_PutChar(128, 224, 0x30+((Device_code>>8)&0x000f), Black, Red);
	   LCD_PutChar(136, 224, 0x30+((Device_code>>4)&0x000f), Black, Red);
	   LCD_PutChar(144, 224, 0x30+(Device_code&0x000f), Black, Red);
       
	   //第5个区域打点实现画线
           for(i=312;i>264;i--){ Put_pixel(68,i,Blue);}
	   for(i=68;i<172;i++) { Put_pixel(i,264,Blue);}
           for(i=264;i<312;i++){ Put_pixel(172,i,Blue);} 
	   for(i=172;i>68;i--) { Put_pixel(i,312,Blue);}
 
	   delayms(20000);  //显示一段时间
	   CLR_Screen(Red); //用背景色清屏
	  }
 
  }



