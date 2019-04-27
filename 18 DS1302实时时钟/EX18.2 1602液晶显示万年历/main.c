/***************************************************************************************************    
工程名称：	DS1302_1602
功能描述：	读取DS1302中的万年历数据，通过1602液晶显示。
硬件连接：  用3位杜邦线分别将J8_0与J7_CLK、J8_1与J7_I/O、J8_2与J7_RST连接,插上1602液晶
维护记录：  2011-8-22
***************************************************************************************************/
//声明外部函数
extern InitLCD();  
extern xian1();
extern xian2();
extern InitDS1302();
extern GetDS1302();
//************************************************************************************
//功能：主程序
//************************************************************************************
main()
{
   InitLCD();	   //初始化1602
   InitDS1302();   //测试断开电源时，电池供电功能     
   while(1)	       //进入死循环，防止看门狗复位
   {   
    GetDS1302();   //提取时间参数 
    xian1();	   //显示第一行	
    xian2();	   //显示第二行	    
   }
}