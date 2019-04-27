#include <reg51.h>
#include"intrins.h"      //_nop_();延时函数用
#define uchar unsigned char
#define uint unsigned int

sbit DS1302_SCLK =P1^0;  //时钟
sbit DS1302_IO   =P1^1;  //数据输入输出
sbit DS1302_RST  =P1^2;  //复位/片选线

struct S_TIME
	{
		uchar SEC;			//00~59
		uchar MIN;			//00~59
		uchar HR;			//00-23
		uchar DATE;			//00-31
		uchar MONTH;		//01-12
		uchar DAY;			//01-07
		uchar YEAR;			//00-99
	}TIME={0x01,0x01,0x01,0x07,0x05,0x05,0x10};//初始化时间参数

uchar dtime[7][2]={	    
                     {0x20,0x20},	//秒 十位和个位
					 {0x20,0x20},	//分 十位和个位
					 {0x20,0x20},	//时 十位和个位
					 {0x20,0x20},	//日 十位和个位
					 {0x20,0x20},	//月 十位和个位
					 {0x20,0x20},	//周 十位和个位
					 {0x20,0x20}	//年 十位和个位
                 	};
/******************************************************************** 
//函数名称: DS1302_WByte
//函数功能: 往DS1302 写入 1 Byte 数据
//输入值:   ndata: 寄存器的数据或地址
//返回值:   无
***********************************************************************/
  void DS1302_WByte(uchar ndata) 
{  
  uchar i;
  for(i=8; i>0; i--)                   //循环8次写入8位数据
  {
    DS1302_IO = (bit)(ndata&0x01);     //取最低位数据，从0位至7位依次传送
    DS1302_SCLK = 1;                   //给一个脉冲，将数据写入1302                 
    _nop_();
    DS1302_SCLK = 0;                   
    ndata>>=1;                         //即ndata = ndata >> 1; 
  } 
}
/******************************************************************** 
//函数名称: DS1302_RByte 
//函数功能: 从DS1302读取 1 Byte数据
//输入值:   无
//返回值:   ndata:读取的数据
***********************************************************************/
  uchar DS1302_RByte(void) 
{ 
   uchar i;
   uchar ndata=0;
   for(i=8;i>0;i--)                 //循环8次读出8位数据 
	{
	 DS1302_IO=1;                   //初始化数据IO
	 ndata>>=1; 			        //即ndata = ndata >> 1; 
	 if(DS1302_IO) ndata|=0x80;     //从数据口读取1位数据
     DS1302_SCLK = 1;               //给一个脉冲
     _nop_();
     DS1302_SCLK = 0;                     

	 }
     return (ndata);                //返回结果
}
/******************************************************************** 
//函数名称: DS1302_Wdata 
//函数功能: 往DS1302某地址写入某数据
//输入值:   nAddr: DS1302地址, ndata: 要写的数据
//返回值:   无
***********************************************************************/
void DS1302_Wdata(uchar nAddr, uchar ndata)
{
  DS1302_RST=0;              
  DS1302_SCLK=0;
  DS1302_RST=1;
  DS1302_WByte(nAddr);       // 写1Byte地址
  DS1302_WByte(ndata);       // 写1Byte数据 
  DS1302_SCLK=1;
  DS1302_RST=0;
} 
/******************************************************************** 
//函数名称: DS1302_Rdata 
//函数功能: 从DS1302某地址读取数据
//输入值:   nAddr: DS1302地址
//返回值:   ndata: 读取的数据
***********************************************************************/
uchar DS1302_Rdata(uchar nAddr)
{
    uchar ndata;
 	DS1302_RST=0;
	DS1302_SCLK=0;
	DS1302_RST=1;
    DS1302_WByte(nAddr);       /* 地址，命令 */
    ndata = DS1302_RByte();    /* 读1Byte数据 */
    DS1302_SCLK=1;
	DS1302_RST=0;
    return(ndata);
}
/******************************************************************** 
//函数名称: InitDS1302
//函数功能: DS1302初始时间设定
//输入值:   无
//返回值:   无
***********************************************************************/
void InitDS1302(void)
{
DS1302_Wdata(0x8e,0x00); //控制命令,WP=0,写操作
DS1302_Wdata(0x90,0xa5);  
/*
地址0x90为充电寄存器，可以对充电电流进行限制，写入
内容高4位固定为1010（其他组合均不能充电），低4
位的首2位是选择内部降压二极管的个数的，01代表在
充电回路串入1个二极管，10代表串入2个；最后2位可
设定串入的电阻的数值：01为2k欧，10为4k欧，11为8k欧。
*/
DS1302_Wdata(0x80,TIME.SEC);  //秒
DS1302_Wdata(0x82,TIME.MIN);  //分
DS1302_Wdata(0x84,TIME.HR);   //时
DS1302_Wdata(0x86,TIME.DATE); //日
DS1302_Wdata(0x88,TIME.MONTH);//月
DS1302_Wdata(0x8a,TIME.DAY);  //星期
DS1302_Wdata(0x8c,TIME.YEAR); //年

DS1302_Wdata(0x8e,0x80); //控制命令,WP=1,写保护
}
/******************************************************************** 
//函数名称: GetDS1302
//函数功能: DS1302当前时间读取
//输入值:   无
//返回值:   无
***********************************************************************/
   void GetDS1302(void)
	{
		TIME.SEC	= DS1302_Rdata(0x81);    //从DS1302读取秒数据
        dtime[0][0]=(TIME.SEC>>4)+0x30;      //十位
		dtime[0][1]=(TIME.SEC&0x0F)+0x30;    //个位   

		TIME.MIN	= DS1302_Rdata(0x83);    //从DS1302读取分数据
        dtime[1][0]=(TIME.MIN>>4)+0x30;      //十位  
		dtime[1][1]=(TIME.MIN&0x0F)+0x30;    //个位

		TIME.HR		= DS1302_Rdata(0x85);    //从DS1302读取时数据
        dtime[2][0]=(TIME.HR>>4)+0x30;       //十位  
		dtime[2][1]=(TIME.HR&0x0F)+0x30;     //个位

		TIME.DATE	= DS1302_Rdata(0x87);    //从DS1302读取日数据
        dtime[3][0]=(TIME.DATE>>4)+0x30;     //十位  
		dtime[3][1]=(TIME.DATE&0x0F)+0x30;   //个位

		TIME.MONTH	= DS1302_Rdata(0x89);    //从DS1302读取月数据
        dtime[4][0]=(TIME.MONTH>>4)+0x30;    //十位  
		dtime[4][1]=(TIME.MONTH&0x0F)+0x30;  //个位

		TIME.DAY	= DS1302_Rdata(0x8b);    //从DS1302读取星期数据
        dtime[5][0]=(TIME.DAY>>4)+0x30;      //十位  
		dtime[5][1]=(TIME.DAY&0x0F)+0x30;    //个位

		TIME.YEAR	= DS1302_Rdata(0x8d);    //从DS1302读取年数据
        dtime[6][0]=(TIME.YEAR>>4)+0x30;     //十位  
		dtime[6][1]=(TIME.YEAR&0x0F)+0x30;   //个位
	}


