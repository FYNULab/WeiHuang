//��˾���Ϸ�쿻͵������޹�˾
//��ַ��www.hfwhdz.com
/*===================================================================================================  
�������ƣ�  Ex3 TP_MOT
����������  ��������ֱ���������
Ӳ�����ӣ�  �鿴���޸Ľӿڶ�����NBCTFT.C��TP.C��,����ϸ���ӿ����ߺ��ϵ���ԡ�
      ----------------------------------------
     |     --------TFT���ƽ���----------      |
     |	        D10~D17   ��   P0             |
     |          RS        ��   P2^5;	      |
     |			RW   	  ��   P2^4;          |
     |			RD        ��   P2^3;          |
     |		    CS        ��   P2^2;	      |
     |			RES       ��   P2^1;	      |
     |	                                      |
     |			LE        ��   P2^0;          |
     |	                                      |
     |     --------��Դ�������---------      |
     |	        GND       ��   ��Դ����       |
     |	        VIN       ��   ��Դ����(5V)   |
     |----------------------------------------|
     | �����NBC��������ʹ�ã������߲����÷�  |
     | �����ӣ�ֱ�Ӳ���ڿ������ϵ�12864�ӿ�  |
     | ��ֻ���������½���                     |
     |----------------------------------------|
     |                                        |
     |     --------�������ƽ���---------      |
     |          PEN       ��   P3^2;	      |
     |			TPDO   	  ��   P3^3;          |
     |			BUSY      ��   P3^4;          |
     |		    TPDI      ��   P3^5;	      |
     |			TPCS      ��   P3^6;	      |
	 |			CLK       ��   P3^7;	      |
     |                                        |
     |     --------ֱ��������ƽ���---------  |
     |          J18_DCMT  ��   J8_0(P1^0);	  |
	  ----------------------------------------
ά����¼��  2012-3-14
====================================================================================================*/

//******************����ͷ�ļ�***************************
#include <reg51.h>
#include <TP.H>
#include <NBCTFT.H>


//**************�������ƶ˿ڶ���********************																				   
sbit Penirq =P3^2;          //����������ʱ��ADS7843Penirq������δ����ʱ�ĸߵ�ƽ��Ϊ�͵�ƽ
sbit Dcout=P1^0;            //����������IO

//******************ȫ�ֱ���***************************
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
//��ʱ���ж��ӳ���
//**************************************************************************************************
t0() interrupt 1 using 1
{
  TH0=0xfc;               //���ö�ʱ����ֵ,��ʱ1ms
  TL0=0x18;
   if(num==speed)Dcout=0; //��������ж�speed��ʱ��Dcout�õͣ�ռ�ձ�=speed/10,speedԽ��,��ת��Խ��
  num++;                  //������1
   if(num==10)            //��10��Ϊһ���ڣ���T=10��*1ms=10ms
    {
     num=0;               //����numΪ0,��ʼ��һ����
     Dcout=1;             //Dcout�ø�
    }
}
//**************************************************************************************************
//��ʱ����ʼ��
//**************************************************************************************************
T0_init()
{
   TMOD=0x01;    //0b00000001  �õ��Ƕ�ʱ��0�������ڷ�ʽ1��16λ�Ĵ�����
                 //Ҫʵ�ֶ�ʱ1ms,(65536-T0)x12/12000000=0.001s��T0=15536��0xfc18
   TH0=0xfc;     //װ��ʱ����ֵ��8λ
   TL0=0x18;     //װ��ʱ����ֵ��8λ
   TR0=1;        //������ʱ��0
   ET0=1;        //����ʱ��0�ж�
   EA=1;         //�����ж�
}


//**************************************************************************************************
//TFT�����ʼ��
//**************************************************************************************************
void init_interface()
{
    CLR_Screen(Blue);                                 //�ñ���ɫ����

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



	LCD_PutString(44,16,"NBC���������ʾϵͳ",White,Blue);
	LCD_PutString(32,40,"The speed now is : ___",White,Blue);

                             
    LCD_PutChar(192, 38, 0x30+speed, Red,Blue);       //��ʾ�����ٶȵȼ�ֵ
} 

//**************************************************************************************************
//��������ɨ����ƺ���
//**************************************************************************************************
void key_scan()
{
    if((lx>14)&&(lx<46)&&(ly>272)&&(ly<304))           //����0
	{
	  Show_RGB(14,46,272,304,Red);                     //���²��ֱ�ɫͻ����ʾ
	  LCD_PutString(28,280,"0",Black,Red);      
  
	  temp=0;                                          //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);  //��ʾ�ٶȵȼ�ֵ

	  while(Penirq==0);                                //�ȴ������ͷ�
	  Show_RGB(14,46,272,304,Magenta);                 //�ͷź�ָ���ʾ��ɫ
      LCD_PutString(28,280,"0",Black,Magenta);

	}



	if((lx>14)&&(lx<46)&&(ly>208)&&(ly<240))           //����1
	{
	  Show_RGB(14,46,208,240,Red);                     //���²��ֱ�ɫͻ����ʾ
      LCD_PutString(28,216,"1",Black,Red);
  
	  temp=1;                                          //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);  //��ʾ�ٶȵȼ�ֵ

	  while(Penirq==0);                                //�ȴ������ͷ�
	  Show_RGB(14,46,208,240,Magenta);                 //�ͷź�ָ���ʾ��ɫ
      LCD_PutString(28,216,"1",Black,Magenta); 

	}


    if((lx>74)&&(lx<106)&&(ly>208)&&(ly<240))          //����2
	{
	  Show_RGB(74,106,208,240,Red);                    //���²��ֱ�ɫͻ����ʾ
      LCD_PutString(88,216,"2",Black,Red);
  
	  temp=2;                                          //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);  //��ʾ�ٶȵȼ�ֵ

	  while(Penirq==0);                                //�ȴ������ͷ�
	  Show_RGB(74,106,208,240,Magenta);                //�ͷź�ָ���ʾ��ɫ
      LCD_PutString(88,216,"2",Black,Magenta); 

	}



    if((lx>134)&&(lx<166)&&(ly>208)&&(ly<240))          //����3
	{
	  Show_RGB(134,166,208,240,Red);                    //���²��ֱ�ɫͻ����ʾ
      LCD_PutString(148,216,"3",Black,Red);
  
	  temp=3;                                          //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutString(192,38,"3",Black,Magenta);         //��ʾ�ٶȵȼ�ֵ

	  while(Penirq==0);                                //�ȴ������ͷ�
	  Show_RGB(134,166,208,240,Magenta);               //�ͷź�ָ���ʾ��ɫ
      LCD_PutString(148,216,"3",Black,Magenta);

	}



	if((lx>14)&&(lx<46)&&(ly>144)&&(ly<176))           //����4
	{
	  Show_RGB(14,46,144,176,Red);                     //���²��ֱ�ɫͻ����ʾ
      LCD_PutString(28,152,"4",Black,Red);
  
	  temp=4;                                          //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta); //��ʾ�ٶȵȼ�ֵ

	  while(Penirq==0);                                //�ȴ������ͷ�
	  Show_RGB(14,46,144,176,Magenta);                 //�ͷź�ָ���ʾ��ɫ
      LCD_PutString(28,152,"4",Black,Magenta);

	}



	if((lx>74)&&(lx<106)&&(ly>144)&&(ly<176))           //����5
	{
	  Show_RGB(74,106,144,176,Red);                     //���²��ֱ�ɫͻ����ʾ    
      LCD_PutString(88,152,"5",Black,Red);

	  temp=5;                                           //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //��ʾ�ٶȵȼ�ֵ

	  while(Penirq==0);                                 //�ȴ������ͷ�
	  Show_RGB(74,106,144,176,Magenta);                 //�ͷź�ָ���ʾ��ɫ                
      LCD_PutString(88,152,"5",Black,Magenta);

	}




	if((lx>134)&&(lx<166)&&(ly>144)&&(ly<176))          //����6
	{
	  Show_RGB(134,166,144,176,Red);                    //���²��ֱ�ɫͻ����ʾ    
      LCD_PutString(148,152,"6",Black,Red);

	  temp=6;                                           //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //��ʾ�ٶȵȼ�ֵ

	  while(Penirq==0);                                 //�ȴ������ͷ�
	  Show_RGB(134,166,144,176,Magenta);                //�ͷź�ָ���ʾ��ɫ 
      LCD_PutString(148,152,"6",Black,Magenta);

	}



	if((lx>14)&&(lx<46)&&(ly>80)&&(ly<112))             //����7
	{
	  Show_RGB(14,46,80,112,Red);                       //���²��ֱ�ɫͻ����ʾ    
      LCD_PutString(28,88,"7",Black,Red);

	  temp=7;                                           //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //��ʾ�ٶȵȼ�ֵ

	  while(Penirq==0);                                 //�ȴ������ͷ�
	  Show_RGB(14,46,80,112,Magenta);                   //�ͷź�ָ���ʾ��ɫ 
      LCD_PutString(28,88,"7",Black,Magenta);

	}



	if((lx>74)&&(lx<106)&&(ly>80)&&(ly<112))            //����8
	{
	  Show_RGB(74,106,80,112,Red);                      //���²��ֱ�ɫͻ����ʾ    
      LCD_PutString(88,88,"8",Black,Red);

	  temp=8;                                           //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //��ʾ�ٶȵȼ�ֵ

	  while(Penirq==0);                                 //�ȴ������ͷ�
	  Show_RGB(74,106,80,112,Magenta);                  //�ͷź�ָ���ʾ��ɫ 
      LCD_PutString(88,88,"8",Black,Magenta);

	}



	if((lx>134)&&(lx<166)&&(ly>80)&&(ly<112))           //����9
	{
	  Show_RGB(134,166,80,112,Red);                     //���²��ֱ�ɫͻ����ʾ    
      LCD_PutString(148,88,"9",Black,Red);

	  temp=9;                                           //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+temp, Black,Magenta);   //��ʾ�ٶȵȼ�ֵ

	  while(Penirq==0);                                 //�ȴ������ͷ�
	  Show_RGB(134,166,80,112,Magenta);                 //�ͷź�ָ���ʾ��ɫ
      LCD_PutString(148,88,"9",Black,Magenta);

	}





	if((lx>74)&&(lx<166)&&(ly>272)&&(ly<304))           //����Enter
	{                  
	  Show_RGB(74,166,272,304,Red);                     //���²��ֱ�ɫͻ����ʾ 
      LCD_PutString(100,280,"Enter",Black,Red);

	  speed=temp;                                       //�ٶȵȼ�����speed
	  LCD_PutChar(192, 38, 0x30+speed, Red,Blue);     //��ʾ�ٶȵȼ�ֵ

	  while(Penirq==0);                                 //�ȴ������ͷ�
	  Show_RGB(74,166,272,304,Magenta);                 //�ͷź�ָ���ʾ��ɫ
      LCD_PutString(100,280,"Enter",Black,Magenta);

	}



	
	if((lx>194)&&(lx<226)&&(ly>80)&&(ly<176))           //����+
	{
	  Show_RGB(194,226,80,176,Red);                     //���²��ֱ�ɫͻ����ʾ    
      LCD_PutString(206,120,"+",Black,Red);
     
	  if(speed<9)speed++;                               //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+speed, Red,Blue);       //��ʾ�ٶȵȼ�ֵ

	  while(Penirq==0);                                 //�ȴ������ͷ�
	  Show_RGB(194,226,80,176,Magenta);                 //�ͷź�ָ���ʾ��ɫ
      LCD_PutString(206,120,"+",Black,Magenta);

	}



	if((lx>194)&&(lx<226)&&(ly>208)&&(ly<304))          //����-
	{
	  Show_RGB(194,226,208,304,Red);                     //���²��ֱ�ɫͻ����ʾ    
      LCD_PutString(206,248,"-",Black,Red);
     
	  if(speed>0)speed--;                               //�ٶȵȼ�������ʱ����temp,ֱ��Enter���º�Ÿ���speed

	  LCD_PutChar(192, 38, 0x30+speed, Red,Blue);       //��ʾ�ٶȵȼ�ֵ

	  while(Penirq==0);                                 //�ȴ������ͷ�
	  Show_RGB(194,226,208,304,Magenta);                //�ͷź�ָ���ʾ��ɫ
      LCD_PutString(206,248,"-",Black,Magenta);

	}
} 

//================================================================================================
//	�������ƣ�	������
//	ʵ�ֹ��ܣ�	��������ֱ���������
//	������		��
//	����ֵ��	��
//================================================================================================
main()
{	
    Device_code=0x9320;
	speed=5;

	TFT_Initial();      //tft��ʼ��
	start_7843();       //��ʼ����������IC
	T0_init();	        //��ʱ����ʼ��
	init_interface();   //��ʼ���˻����ƽ���
    
   	while(1)
	{
    	if(Getpix()==1)key_scan();    //ɨ�败������ʵ�ֵ������
    }
}



