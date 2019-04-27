
//定义触摸控制翻页
#define PEN     P12
//===========================================================
//定义512字节缓冲区，注意需要使用 xdata关键字
extern unsigned char   DATA[512];
//===========================================================
//写一字节到SD卡,模拟SPI总线方式
void SdWrite(unsigned char n);

//===========================================================
//从SD卡读一字节,模拟SPI总线方式
unsigned char SdRead();

//============================================================
//检测SD卡的响应
unsigned char SdResponse();

//================================================================
//发命令到SD卡
void SdCommand(unsigned char command, unsigned long argument, unsigned char CRC);

//================================================================
//初始化SD卡
unsigned char SdInit(void);

//================================================================
//往SD卡指定地址写数据,一次最多512字节
unsigned char SdWriteBlock(unsigned char *Block, unsigned long address,int len);


//=======================================================================
//从SD卡指定地址读取数据,一次最多512字节
unsigned char SdReadBlock(unsigned char *Block, unsigned long address,unsigned int len);
