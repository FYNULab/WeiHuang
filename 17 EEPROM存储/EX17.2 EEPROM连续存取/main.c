/***************************************************************************************************    
工程名称：	IIC_SEQ
功能描述：	向EEPROM连续存储多个字节数据，然后读取出来并在发光二极管上显示,实现流水灯功能。
硬件连接：  用8位杜邦线将J8与J12连接，2位杜邦线分别将J11_6与J17_SCL、J11_7与J17_SDA连接
维护记录：  2011-8-22
***************************************************************************************************/
#include "reg51.h"      //包含头文件
#include"intrins.h"     //_nop_();延时函数用

sbit sda=P2^7;          //数据线
sbit scl=P2^6;          //时钟线

#define uchar unsigned char
#define uint  unsigned int

uchar temp[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

uchar temp1[8];

//**************************************************************************************************
//延时
//**************************************************************************************************
delay(uint time)              //int型数据为16位,所以最大值为65535            
 {
  uint  i,j;                  //定义变量i,j,用于循环语句 
  for(i=0;i<time;i++)         //for循环,循环50*time次
     for(j=0;j<50;j++);       //for循环,循环50次
 }
//**************************************************************************************************
//启动(SCL为高，SDA由高变为低是一个开始条件)
//**************************************************************************************************
void start()  
{	
	sda=1;    //数据线置高,
	_nop_();  //延时
	scl=1;    //时钟线置高
	_nop_();  //延时
	sda=0;    //数据线置低,由高变低
	_nop_();  //延时
	scl=0;    //时钟线置低,准备发送或接收数据,总线进入忙状态(I2C总线在空闲状态时，SDA与SCL均被置高)
	_nop_();  //延时
}
//**************************************************************************************************
//停止（SCL为高，SDA由低变为高是一个结束条件）
//**************************************************************************************************
void stop()   
{
	sda=0;                   //数据线置低
	_nop_();                 //延时
	scl=1;                   //时钟线置高
	_nop_();                 //延时
	sda=1;                   //数据线置高，由低变高
	_nop_();                 //延时
}
//**************************************************************************************************
//检测应答(所有的地址和数据字都是以8bit,在第9个时钟周期，从器件发出"0"信号来作为收到一个字的应答信号)
//**************************************************************************************************
void checkACK()                   //主器件检测从器件是否返回应答
{
	scl=1;                        //时钟线置高
	_nop_();                      //延时
	while(sda==1);                //等待第9个时钟周期器件发出的响应信号"0"
	scl=0;                        //时钟线置低
	_nop_();                      //延时
}
//**************************************************************************************************
//发送应答(发送方为主器件，接收方为从器件，控制器作为从器件接收完1数据时，发送应答信号
//**************************************************************************************************
void sendACK(bit ACK)                   
{
    if(ACK)sda=1;            //如果i位为1则发送1,即发送"非应答信号"
      else sda=0;            //如果i位为0则发送0,即发送"应答信号"	
    scl=1;                   //时钟线置高,给一个脉冲
	_nop_();                 //延时
	scl=0;                   //时钟线置低
	_nop_();                 //延时
}
//**************************************************************************************************
//写一字节
//**************************************************************************************************
void send_byte(uchar date)       //写一个8位字
{
	uchar i,temp;                //定义局部变量
	temp=date;                   //待发8位数据赋予temp
	for(i=0;i<8;i++)             //循环8次，每次写入1位，从最高位开始发送
	{
        if(temp&0x80)sda=1;      //如果temp最高位为1则发送1
          else sda=0;            //如果temp最高位为0则发送0
	    _nop_();                 //延时
		scl=1;                   //给一个脉冲，发送sda当前这位数据
		_nop_();                 //延时,需大于4us(参考数据手册时序图) 
		_nop_();              
		_nop_();                 
		_nop_();                 
		_nop_();                
		scl=0;                   //时钟线置低,准备下一脉冲
	    _nop_();                 //延时,需大于4.7us(参考数据手册时序图) 
		_nop_();              
		_nop_();                 
		_nop_();                 
		_nop_();  
		temp=temp<<1;            //左移1位，准备好下1位待发送的数据
	}
	checkACK();                  //查询是否返回应答信号
}
//**************************************************************************************************
//读一字节
//**************************************************************************************************
uchar receive_byte()         //读一个8位字
{
	uchar i,temp;            //定义局部变量
	sda=1;                   //设置数据线为输入
	_nop_();                 //延时
	for(i=0;i<8;i++)         //循环8次，每次读取1位，从最高位开始接收
	{
		scl=1;               //给一脉冲，准备发送1位数据            
		_nop_();             //延时,需大于4us(参考数据手册时序图) 
		_nop_();              
		_nop_();                 
		_nop_();                 
		_nop_();
		temp=(temp<<1)|sda;  //读取1位数据,放在temp最低位
		scl=0;               //准备给下1个脉冲
	    _nop_();             //延时,需大于4.7us(参考数据手册时序图) 
		_nop_();              
		_nop_();                 
		_nop_();                 
		_nop_();       
	}
    sendACK(0);              //发送应答信号
	return temp;             //返回读取的8位数据
}
//**************************************************************************************************
//向某I2C器件的某字地址开始连续存储n个数据
//**************************************************************************************************
void write_nword(uchar device_add,uchar word_add,uchar n,uchar *date)  
{
	uchar i;
     for(i=0;i<n;i++)
      {
       start();                  //启动
	   send_byte(device_add);    //选择从器件地址,RW位为0，即选择写命令
	   send_byte(word_add);      //写字地址
	   send_byte(*date);         //写数据
	   stop();                   //停止 
       delay(100);               //延时 
       word_add++;               //字地址加1
       date++;                   //数据地址加1
      }       
}
//**************************************************************************************************
//向某I2C器件的某字地址开始连续读取n个数据
//**************************************************************************************************
void read_nword(uchar device_add,uchar word_add,uchar n,uchar *date)   //读出一个存储的数据
{
	        uchar i;
            start();                 //启动
	        send_byte(device_add);   //选择从器件地址,RW位为0，即选择写命令
	        send_byte(word_add);     //写字地址
	        start();                 //启动
	        send_byte(device_add+1); //选择从器件地址,RW位为1，即选择读命令
            for(i=0;i<n;i++)
             {
     	      *date=receive_byte();  //读数据
    	      date++;                //数据地址加1
             }  
            sendACK(1);              //发送非应答信号
	        stop();                  //停止    
}
//**************************************************************************************************
//主函数
//**************************************************************************************************
void main()
{	
   uchar i;                      //定义变量
   write_nword(0xa0,0,8,temp);   //向I2C器件a0的第0个地址开始连续写8个字节的数据
   while(1)                      //死循环
    {
   read_nword(0xa0,0,8,temp1);   //向I2C器件a0的第0个地址开始连续读8个字节的数据,存放在temp1数组中
	 for(i=0;i<8;i++)            //循环显示数据
      {
       P1=~temp1[i];             //送数据到P1口显示	   
       delay(2000);              //延时
       temp1[i]=0;               //temp1[i]清0,8次循环后数组全部为0
      }
	}
}