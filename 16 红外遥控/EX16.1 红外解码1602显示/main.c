/***************************************************************************************************    
工程名称：	IFR_1602
功能描述：  按下遥控器上的任意按键，实现红外遥控器解码，1602液晶显示解码结果。
硬件连接：  用1位杜邦线将J9_2与J5_IFR连接,将1602液晶接口对应插接到P4接口。
维护记录：  2011-8-22
***************************************************************************************************/
#include "reg51.h"          //包含头文件
#include "1602.h"
#define uchar unsigned char
#define uint  unsigned int

extern uchar Read_AD(uchar chn);


sbit IFR_in=P3^2;                     //红外触发外部中断管脚

char code Tab[]="0123456789+-<>||CHEQ"; //1602液晶显示0-F的编码
uchar dis2[16];                       //解码结果
uchar  IRQ_num;                       //定时中断进入次数,用判断高低电平的计时
uchar  T_num[33];
bit IRF_ok;                           //解码1次完成标志位
bit irpro_ok,IRF_ok;
unsigned char decode[4];
#define K0  ( decode[2]==0x16 ) // 0
#define K1  ( decode[2]==0x0c ) // 1
#define K2  ( decode[2]==0x18 ) //2
#define K3  ( decode[2]==0x5e ) // 3
#define K4  ( decode[2]==0x08 ) // 4
#define K5  ( decode[2]==0x1c ) // 5
#define K6  ( decode[2]==0x5a ) // 6
#define K7  ( decode[2]==0x42 ) // 7
#define K8  ( decode[2]==0x52 ) // 8
#define K9  ( decode[2]==0x4a ) // 9
#define K10 ( decode[2]==0x19 ) // 100+
#define K11 ( decode[2]==0x0d ) //200+
#define K12 ( decode[2]==0x07 ) //-
#define K13 ( decode[2]==0x15) // +
#define K14 ( decode[2]==0x09 ) // EQ
#define K15 ( decode[2]==0x44 ) // <<
#define K16 ( decode[2]==0x40 ) //>>
#define K17 ( decode[2]==0x43 ) // >||
#define K18 ( decode[2]==0x45 ) // CH-
#define K19 ( decode[2]==0x46 ) // CH
#define K20 ( decode[2]==0x47 ) // CH+
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
//定时中断0处理函数
//**************************************************************************************************
void IRQ_timer0(void) interrupt 1 using 1
{
  IRQ_num++;   //计数1个周期时间，即2个下降沿之间的时间
}

//**************************************************************************************************
//外部中断0处理函数
//**************************************************************************************************
void IRQ_EX0(void) interrupt 0 
{
  static uchar  i;         //接收红外信号处理
  static bit flag;         //第二次进入中断时，完成第1个周期计时
if(flag)                         
   {
    if(IRQ_num<63&&IRQ_num>=33)//按下一次按键，先来一个13.5ms(9ms+4.5ms)引导码
                        i=0;   //数组重新开始存储接下来的32位数的周期长度
    		T_num[i]=IRQ_num;  //存储每个每个周期的持续时间，用来判断高低电平
    		IRQ_num=0;         //定时中断次数置0，以便开始计数下一个周期的持续时间
    		i++;               //i加1
   			 if(i==33)
      			{
	  			 IRF_ok=1;     //解码1次完成，置标志位
				 i=0;          //数组重新开始存储接下来的32位数的周期长度
	  			}
          }
  	 else
		{
		IRQ_num=0;             //定时中断次数置0   
		flag=1;                //第一次进入中断,置标志位
		}
}
//**************************************************************************************************
//定时0初始化
//**************************************************************************************************
void timer0_init(void)//定时器0初始化
{

  TMOD=0x02; //定时器0工作方式2，自动装入时间常数的8位定时器/计数器
  TH0=0x00; 
  TL0=0x00;  //置初值,(256-0)x12/12000000=0.256ms
  ET0=1;     //开定时器0中断
  TR0=1;     //启动定时器0
}
//**************************************************************************************************
//外部中断0初始化
//**************************************************************************************************
void EX0init(void)
{
 IT0 = 1;   //指定外部中断0下降沿触发，INT0 (P3.2)
 EX0 = 1;   //使能外部中断
 EA = 1;    //开总中断
}
//**************************************************************************************************
//32位二进制得4字节数据
//**************************************************************************************************
void bit_byte(void) 
{ 
  uchar i, j, k=1;
  uchar temp,value;
  for(i=0;i<4;i++)        //循环4次得4字节
     {
      for(j=1;j<=8;j++)   //循环取8位得1字节
         {
          value>>=1;      //左移1位,依次从低位到高位
          temp=T_num[k];  //由于第1个数据为引导码，舍去，所以从第2开即是提取后面的32位
          if(temp>7)      //大于7*0.256=1.8ms为1,周期1.125ms表示"0"；周期2.25ms表示"1"
          value|=0x80; //为1则最高位赋1
          k++;            //k加1，准备提取下1位
         }
     decode[i]=value;     //获取到1字节
     value=0;             //清0，准备提取下1字节        
     } 
}
//**************************************************************************************************
//码值处理函数
//**************************************************************************************************
void code_pro(void)
{
       //处理16位用户码
       dis2[0] = Tab[decode[0]/16];   //取用户码第1字节的高4位
	   dis2[1] = Tab[decode[0]%16];   //取用户码第1字节的低4位
	   dis2[2] = Tab[decode[1]/16];   //取用户码第2字节的高4位
	   dis2[3] = Tab[decode[1]%16];   //取用户码第2字节的低4位

	   dis2[4] = '-';                 //符号
	   dis2[5] = '(';                 //符号
	   dis2[6] = Tab[decode[2]/16];   //处理数据码的高4位
	   dis2[7] = Tab[decode[2]%16];   //处理数据码的低4位
	   dis2[8] = '/';                 //符号
	   dis2[9] = Tab[decode[3]/16];   //处理数据反码的高4位,用于核对数据是否接收准确
	   dis2[10] = Tab[decode[3]%16];  //处理数据反码的低4位
	   dis2[11] =')';                 //符号
}
unsigned int GetKeyValue()
{		//unsigned int i;
 	//	i=adaverage[7];
	   // bit_byte() ;
	if K1 return 1;
	if K2 return 2;
	if K3 return 3;
	if K4 return 4;
	if K5 return 5;
	if K6 return 6;
	if K7 return 7;
	if K8 return 8;
	if K9 return 9;
	if K10 return 10;
	if K11 return 11;
	if K12 return 12;
	if K13 return 13;
	if K14 return 14;
	if K15 return 15;
	if K16 return 16;
	if K17 return 17;
	if K18 return 18;
	if K19 return 19;
	if K20 return 20;
	return 0;
}
void Adjust (void)
{
 unsigned int i;
 	i=	 GetKeyValue();

	switch (i)
	{
	   case (0):	wcode(0xc4); wdata(Tab[0]);				   break;
	   case (1):	wcode(0xc4); wdata(Tab[1]);				   break;
	   case (2):	wcode(0xc4); wdata(Tab[2]);				   break;
	   case (3):	wcode(0xc4); wdata(Tab[3]);				   break;
	   case (4):	wcode(0xc4); wdata(Tab[4]);				   break;
	   case (5):	wcode(0xc4); wdata(Tab[5]);				   break;
	   case (6):	wcode(0xc4); wdata(Tab[6]);				   break;
	   case (7):	wcode(0xc4); wdata(Tab[7]);				   break;
	   case (8):	wcode(0xc4); wdata(Tab[8]);				   break;
	   case (9):	wcode(0xc4); wdata(Tab[9]);				   break;
	   case (10):	wcode(0xc4); wdata(Tab[2]);				   break;
	   case (11):					   break;
	   case (12):	wcode(0xc4); wdata(Tab[11]);				   break;
	   case (13):	wcode(0xc4); wdata(Tab[10]);				   break;
	   case (14):					   break;
	   case (15):	wcode(0xc4); wdata(Tab[12]);				   break;
	   case (16):	wcode(0xc4); wdata(Tab[13]);				   break;
	   case (17):	wcode(0xc4); wdata(Tab[14]);				   break;
	   case (18):					   break;
	   case (19):					   break;
	   case (20):					   break;

	}
	 
}	  
//**************************************************************************************************
//主函数
//**************************************************************************************************
void main(void)
{
 EX0init();            //初始化外部中断0
 timer0_init();        //初始化定时器0
 InitLCD();            //初始化液晶

 while(1)              //死循环
   {
    if(IRF_ok)         //接收完1次后处理
	  {   
	   bit_byte();     //提取到32位数据
       //code_pro();     //码值处理
		 Adjust () ;
       //show();         //显示码值
 	   IRF_ok=0;       //清标志位
	  } 	  
   }
}
  
