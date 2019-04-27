//公司：合肥炜煌电子有限公司
//网址：www.hfwhdz.com
/*===================================================================================================  
工程名称：  Ex3 TP_MOT
功能描述：  触摸控制直流电机调速
硬件连接：  查看和修改接口定义在NBCTFT.C与TP.C中,请仔细检查接口连线后上电测试。
      ----------------------------------------
     |     --------TFT控制接线----------      |
     |	        D10~D17   接   P0             |
     |          RS        接   P2^5;	      |
     |			RW   	  接   P2^4;          |
     |			RD        接   P2^3;          |
     |		    CS        接   P2^2;	      |
     |			RES       接   P2^1;	      |
     |	                                      |
     |			LE        接   P2^0;          |
     |	                                      |
     |     --------电源供电接线---------      |
     |	        GND       接   电源负极       |
     |	        VIN       接   电源正极(5V)   |
     |----------------------------------------|
     | 如果在NBC开发板上使用，以上线不需用飞  |
     | 线连接，直接插接在开发板上的12864接口  |
     | 后只需连接以下接线                     |
     |----------------------------------------|
     |                                        |
     |     --------触摸控制接线---------      |
     |          PEN       接   P3^2;	      |
     |			TPDO   	  接   P3^3;          |
     |			BUSY      接   P3^4;          |
     |		    TPDI      接   P3^5;	      |
     |			TPCS      接   P3^6;	      |
	 |			CLK       接   P3^7;	      |
     |                                        |
     |     --------直流电机控制接线---------  |
     |          J18_DCMT  接   J8_0(P1^0);	  |
	  ----------------------------------------
维护记录：  2012-3-14
====================================================================================================*/

//******************包含头文件***************************
#include <reg51.h>
#include <TP.H>
#include <NBCTFT.H>


//**************触摸控制端口定义********************																				   
sbit Penirq =P3^2;          //触碰触摸屏时，ADS7843Penirq引脚由未触摸时的高电平变为低电平
sbit Dcout=P1^0;            //定义电机控制IO

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



//**************************************************************************************************
//定时器中断子程序
//**************************************************************************************************
t0() interrupt 1 using 1
{
  TH0=0xfc;               //重置定时器初值,定时1ms
  TL0=0x18;
   if(num==speed)Dcout=0; //如果进入中断speed次时，Dcout置低，占空比=speed/10,speed越大,则转速越快
  num++;                  //次数加1
   if(num==10)            //满10次为一周期，即T=10次*1ms=10ms
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

	  while(Penirq==0);                                //等待触摸释放
	  Show_RGB(14,46,272,304,Magenta);                 //释放后恢复显示颜色
      LCD_PutString(28,280,"0",Black,Magenta);

	}



	if((lx>14)&&(lx<46)&&(ly>208)&&(ly<240))           //按下1
	{
	  Show_RGB(14,46,208,240,Red);                     //按下部分变色突出显示
      LCD_PutString(28,216,"1",Black,Red);
  
	  temp=1;                                          //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);  //显示速度等级值

	  while(Penirq==0);                                //等待触摸释放
	  Show_RGB(14,46,208,240,Magenta);                 //释放后恢复显示颜色
      LCD_PutString(28,216,"1",Black,Magenta); 

	}


    if((lx>74)&&(lx<106)&&(ly>208)&&(ly<240))          //按下2
	{
	  Show_RGB(74,106,208,240,Red);                    //按下部分变色突出显示
      LCD_PutString(88,216,"2",Black,Red);
  
	  temp=2;                                          //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);  //显示速度等级值

	  while(Penirq==0);                                //等待触摸释放
	  Show_RGB(74,106,208,240,Magenta);                //释放后恢复显示颜色
      LCD_PutString(88,216,"2",Black,Magenta); 

	}



    if((lx>134)&&(lx<166)&&(ly>208)&&(ly<240))          //按下3
	{
	  Show_RGB(134,166,208,240,Red);                    //按下部分变色突出显示
      LCD_PutString(148,216,"3",Black,Red);
  
	  temp=3;                                          //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutString(192,38,"3",Black,Magenta);         //显示速度等级值

	  while(Penirq==0);                                //等待触摸释放
	  Show_RGB(134,166,208,240,Magenta);               //释放后恢复显示颜色
      LCD_PutString(148,216,"3",Black,Magenta);

	}



	if((lx>14)&&(lx<46)&&(ly>144)&&(ly<176))           //按下4
	{
	  Show_RGB(14,46,144,176,Red);                     //按下部分变色突出显示
      LCD_PutString(28,152,"4",Black,Red);
  
	  temp=4;                                          //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta); //显示速度等级值

	  while(Penirq==0);                                //等待触摸释放
	  Show_RGB(14,46,144,176,Magenta);                 //释放后恢复显示颜色
      LCD_PutString(28,152,"4",Black,Magenta);

	}



	if((lx>74)&&(lx<106)&&(ly>144)&&(ly<176))           //按下5
	{
	  Show_RGB(74,106,144,176,Red);                     //按下部分变色突出显示    
      LCD_PutString(88,152,"5",Black,Red);

	  temp=5;                                           //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //显示速度等级值

	  while(Penirq==0);                                 //等待触摸释放
	  Show_RGB(74,106,144,176,Magenta);                 //释放后恢复显示颜色                
      LCD_PutString(88,152,"5",Black,Magenta);

	}




	if((lx>134)&&(lx<166)&&(ly>144)&&(ly<176))          //按下6
	{
	  Show_RGB(134,166,144,176,Red);                    //按下部分变色突出显示    
      LCD_PutString(148,152,"6",Black,Red);

	  temp=6;                                           //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //显示速度等级值

	  while(Penirq==0);                                 //等待触摸释放
	  Show_RGB(134,166,144,176,Magenta);                //释放后恢复显示颜色 
      LCD_PutString(148,152,"6",Black,Magenta);

	}



	if((lx>14)&&(lx<46)&&(ly>80)&&(ly<112))             //按下7
	{
	  Show_RGB(14,46,80,112,Red);                       //按下部分变色突出显示    
      LCD_PutString(28,88,"7",Black,Red);

	  temp=7;                                           //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //显示速度等级值

	  while(Penirq==0);                                 //等待触摸释放
	  Show_RGB(14,46,80,112,Magenta);                   //释放后恢复显示颜色 
      LCD_PutString(28,88,"7",Black,Magenta);

	}



	if((lx>74)&&(lx<106)&&(ly>80)&&(ly<112))            //按下8
	{
	  Show_RGB(74,106,80,112,Red);                      //按下部分变色突出显示    
      LCD_PutString(88,88,"8",Black,Red);

	  temp=8;                                           //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //显示速度等级值

	  while(Penirq==0);                                 //等待触摸释放
	  Show_RGB(74,106,80,112,Magenta);                  //释放后恢复显示颜色 
      LCD_PutString(88,88,"8",Black,Magenta);

	}



	if((lx>134)&&(lx<166)&&(ly>80)&&(ly<112))           //按下9
	{
	  Show_RGB(134,166,80,112,Red);                     //按下部分变色突出显示    
      LCD_PutString(148,88,"9",Black,Red);

	  temp=9;                                           //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //显示速度等级值

	  while(Penirq==0);                                 //等待触摸释放
	  Show_RGB(134,166,80,112,Magenta);                 //释放后恢复显示颜色
      LCD_PutString(148,88,"9",Black,Magenta);

	}





	if((lx>74)&&(lx<166)&&(ly>272)&&(ly<304))           //按下Enter
	{                  
	  Show_RGB(74,166,272,304,Red);                     //按下部分变色突出显示 
      LCD_PutString(100,280,"Enter",Black,Red);

	  speed=temp;                                       //速度等级赋给speed
	  LCD_PutChar(192, 38, 0x30+speed, Red,Blue);     //显示速度等级值

	  while(Penirq==0);                                 //等待触摸释放
	  Show_RGB(74,166,272,304,Magenta);                 //释放后恢复显示颜色
      LCD_PutString(100,280,"Enter",Black,Magenta);

	}



	
	if((lx>194)&&(lx<226)&&(ly>80)&&(ly<176))           //按下+
	{
	  Show_RGB(194,226,80,176,Red);                     //按下部分变色突出显示    
      LCD_PutString(206,120,"+",Black,Red);
     
	  if(speed<9)speed++;                               //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+speed, Red,Blue);       //显示速度等级值

	  while(Penirq==0);                                 //等待触摸释放
	  Show_RGB(194,226,80,176,Magenta);                 //释放后恢复显示颜色
      LCD_PutString(206,120,"+",Black,Magenta);

	}



	if((lx>194)&&(lx<226)&&(ly>208)&&(ly<304))          //按下-
	{
	  Show_RGB(194,226,208,304,Red);                     //按下部分变色突出显示    
      LCD_PutString(206,248,"-",Black,Red);
     
	  if(speed>0)speed--;                               //速度等级赋给临时变量temp,直到Enter按下后才赋给speed

	  LCD_PutChar(192, 38, 0x30+speed, Red,Blue);       //显示速度等级值

	  while(Penirq==0);                                 //等待触摸释放
	  Show_RGB(194,226,208,304,Magenta);                //释放后恢复显示颜色
      LCD_PutString(206,248,"-",Black,Magenta);

	}
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
	speed=5;

	TFT_Initial();      //tft初始化
	start_7843();       //初始化触摸控制IC
	T0_init();	        //定时器初始化
	init_interface();   //初始化人机控制界面
    
   	while(1)
	{
    	if(Getpix()==1)key_scan();    //扫描触摸按键实现电机调速
    }
}



