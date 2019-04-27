//公司：合肥炜煌电子有限公司
//网址：www.hfwhdz.com
/*===================================================================================================  
文件功能描述：控制实现SD卡数据读写
====================================================================================================*/

//******************包含头文件***************************
#include <reg52.h>

//**************控制端口定义********************
//定义SD卡引脚定义
sbit SD_DO  = P1^0;
sbit SD_CLK = P1^1;
sbit SD_DI  = P1^2;
sbit SD_CS  = P1^3;

//定义触摸控制翻页
sbit PEN    = P3^7;


//******************全局变量***************************
unsigned char xdata DATA[512];//定义512字节缓冲区，注意需要使用 xdata关键字




//================================================================================================
//	实现功能：	写一字节到SD卡,模拟SPI总线方式
//	输入参数：	n 待写入数据
//================================================================================================
void SdWrite(unsigned char n)
{

	unsigned char i;

    for(i=0;i<8;i++)
	{
	 SD_CLK=0;
	 SD_DI=(n&0x80);
	 n<<=1;
	 SD_CLK=1;
	}
	
    SD_DI=1; 
} 

//================================================================================================
//	实现功能：	从SD卡读一字节,模拟SPI总线方式
//	返回参数：	n 读取的8位数据
//================================================================================================
unsigned char SdRead()
{
	unsigned char n,i;
    for(i=0;i<8;i++)
	{
	 SD_CLK=0;
	 SD_CLK=1;
	 n<<=1;
	 if(SD_DO) n|=1;
	}
	return n;
}

//================================================================================================
//实现功能：读取SD卡的响应
//返回参数：response 响应标志
//================================================================================================
unsigned char SdResponse()
{
    unsigned char response,time=0;;
    
    do
    {  
      response=SdRead();
      time++;
    }
    while((response==0xff)&&(time<100));//100次以内尝试读取响应

    return response;      //返回读取的数据

} 

//================================================================================================
//实现功能：发命令到SD卡
//================================================================================================
void SdCommand(unsigned char command, unsigned long argument, unsigned char CRC)
{ 
        unsigned char i,a[6];
        
        a[0]=command|0x40;
        a[1]=((argument&0xff000000)>>24);
        a[2]=((argument&0x00ff0000)>>16);
        a[3]=((argument&0x0000ff00)>>8);
        a[4]=  argument&0x000000ff;
        a[5]=  CRC;
        
        for(i=0;i<6;i++)
        {   
	     SdWrite(a[i]);
        }
}

//================================================================================================
//实现功能：初始化SD卡（参考SD卡初始化时序）
//================================================================================================
unsigned char SdInit(void)
{
	unsigned char i,time=0;
	unsigned char response=0x01;
       
        //send CMD0,如果response=0x01则SD卡成功转到SPI模式 
        do
        {
         SD_CS=1;
	 for(i=0;i<=9;i++)       //复位时最少写入74个时钟周期
	     SdWrite(0xff);
	 SD_CS=0;                //复位后CS拉低
        
         SdCommand(0x00,0,0x95); //Send Command 0 to put MMC in SPI mode
         response=SdResponse();
         
         time++;
        }
        while((response!=0x01)&&(time<100));   //等待命令CMD0的响应
        if(time==100)return 0;   //100次仍然不能得到命令CMD0的响应,超时退出

        
        //SD卡在进入SPI模式后，激活命令为Cmd1
	do
	{
	 SdWrite(0xff);          //补偿8个时钟
	 SdCommand(0x01,0,0xff); //send CMD1
	 response=SdResponse();
	} while(response!=0x00); //等待返回0x00

        //最后CS拉高
	SD_CS=1;
	SdWrite(0xff);           //补偿8个时钟
	return 1;                //初始化成功，返回成功信号1
}

//================================================================================================
//实现功能：从SD卡指定地址读取数据,一次最多512字节（参考SD卡读时序）
//================================================================================================
unsigned char SdReadBlock(unsigned char *Block, unsigned long address,unsigned int len)
{
	unsigned int count;

	SD_CS=0;       //使能SD卡

        SdCommand(17,address,0xff);//send command 17读扇区命令
       
	if(SdResponse()==00)//command was a success - now send data
	{
  	 while(SdRead()!=0xfe); //等待数据的起始令牌号
	 for(count=0;count<len;count++) *Block++=SdRead(); //读512个数据 
	
	 SD_CS=1;
	 return 1;
	}
	return 0;
}