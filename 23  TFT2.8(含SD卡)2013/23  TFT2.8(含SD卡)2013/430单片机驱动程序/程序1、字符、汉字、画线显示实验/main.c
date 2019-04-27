//��˾��������о�Ƽ��������޹�˾
//��ַ��www.hrbnbc.com
/*===================================================================================================  
�������ƣ�  Ex1
����������  ����TFTʵ�ֺ��֣��ַ���ʾ���ͻ��㹦�ܡ�
Ӳ�����ӣ�  �鿴���޸Ľӿڶ�����NBCTFT.C��,����ϸ���ӿ����ߡ�
      ----------------------------------------
     |     --------TFT���ƽ���----------      |
     |	                                      |
     |	        D10~D17   ��   P4             |
     |          RS        ��   P35	      |
     |		RW   	  ��   P34            |
     |		RD        ��   P33            |
     |		CS        ��   P32	      |
     |		RES       ��   P31            |
     |	                                      |
     |		LE        ��   P30            |
     |                                        |
     |     --------��Դ�������---------      |
     |	        GND       ��   ��Դ����       |
     |	        VIN       ��   ��Դ����(5V)   |
     |----------------------------------------|
     | �����NBC��������ʹ�ã������߲����÷�  |
     | �����ӣ�ֱ�Ӳ���ڿ������ϵ�12864�ӿ�  |
      ----------------------------------------
ά����¼��  2012-3-30
====================================================================================================*/

//******************����ͷ�ļ�***************************

#include "io430.h"
#include "nbc430.h"
#include"NBCTFT.h"   //����TFT����ͷ�ļ�

//******************ȫ�ֱ���***************************

#define White          0xFFFF   //LCD color
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

unsigned int Device_code;      //TFT����IC�ͺ�

//**************�����ⲿ�����ͱ���********************

extern void delayms(unsigned int count);


//**************************************************************************************************
//ʱ�ӳ�ʼ��
//**************************************************************************************************
/*3��ʱ��Դ:1��DCO���ڲ�RC���� 2��XTAL1��LFXTAL1��:Ƭ��ʱ��ȷ��Ƶʱ32768Hz,����ACLK.
          3��XTAL2��Ƭ��ʱ��2����ȷ��Ƶʱ��8MHz,����MCLK��SMCLK��
���ʱ��: 1��MCLK��ϵͳ��ʱ�� 2��SMCLK����ϵͳʱ�� 3��ACLK������ʱ��
ϵͳ�ϵ�ʱ��Ĭ��ʹ��DCOʱ����ΪMCLK��SMCLK��ʹ��XTAL1��ΪACLK��XTAL2û������û��
��ʹ�á�ϵͳʱ�ӿ���ѡ��ʹ��MCK��SMCLK��ACLK���е�һ�֡���Ҫϵͳ��������������á�*/
void init_clock()
{
    uchar i;
    BCSCTL1 &= ~XT2OFF;  //����XT2��������(XT2OFF=0)
    BCSCTL2 |=SELM1+SELS;//ѡ����Ҫʱ�� MCLK:XTAL2, SMCLK:XTAL2,����Ƶ
    //������������жϱ�־��ϵͳ����ʱ���ȶ���ɵģ�����Ҫ��λ
    do
    {
        IFG1 &= ~OFIFG; //�������ʧЧ��־
        i = 255;        
        while(i--);     //��ʱ�ȴ�����50us
    }while(IFG1&OFIFG); //�ж�XT2�Ƿ�����
}

//***********************************************************************************
//IO��ʼ������
//***********************************************************************************
void IO_init(void)
{
	P4DIR=0xff;		  //����P4Ϊ���
	P4=0xff;                  //����P4��ʼ��Ϊ�ߵ�ƽ
	
	P3DIR=0xff;		  //����P3Ϊ���
	P3=0xff;                  //����P3��ʼ��Ϊ�ߵ�ƽ
}

//================================================================================================
//	�������ƣ�	������
//	ʵ�ֹ��ܣ�	����TFTʵ�ֺ��֣��ַ���ʾ.
//	������		��
//	����ֵ��	��
//================================================================================================
main()
{
  unsigned int i;
  
  WDTCTL = WDTPW + WDTHOLD;// Stop watchdog timer to prevent time out reset  
  init_clock();
  IO_init();               //IO��ʼ�� 
  Device_code=0x9320;      //TFT����IC�ͺ� 
  TFT_Initial();           //��ʼ��LCD	 

  while(1)                 //ѭ��
      {
	   //5����ɫ������Ϊ5������
	   Show_RGB(0,240,0,64,Blue);
	   Show_RGB(0,240,64,128,Green);
	   Show_RGB(0,240,128,192,Magenta);
	   Show_RGB(0,240,192,256,Red);
	   Show_RGB(0,240,256,320,Yellow);

          //��1��������ʾ16x16����
	  LCD_PutString(24,16,"������о�Ƽ��������޹�˾",White,Blue);
          LCD_PutString(24,40,"   ��о�Ƽ�  ����ͬ��",White,Blue);

	   //��2��������ʾascii�ַ��е����ֺͷ���
	  LCD_PutString(80,72,"0123456789",Black,Green);
	   LCD_PutString(16,96,")(`,./<>';:[]{}|?-=+*&^%$",Black,Green);

	   //��3��������ʾascii�ַ�����ĸ
	   LCD_PutString(16,136,"abcdefghijklmnopqrstuvwxyz",Blue,Magenta);
	  LCD_PutString(16,156,"ABCDEFGHIJKLMNOPQRSTUVWXYZ",Blue,Magenta);

	   //��4��������ʾTFT������IC�ͺ�
	   LCD_PutString(16,200,"The Device IC Of TFT Is:",Black,Red);
	   LCD_PutString(96,224,"ILI",Black,Red);
	   LCD_PutChar(120, 224, 0x30+(Device_code>>12), Black, Red);
	   LCD_PutChar(128, 224, 0x30+((Device_code>>8)&0x000f), Black, Red);
	   LCD_PutChar(136, 224, 0x30+((Device_code>>4)&0x000f), Black, Red);
	   LCD_PutChar(144, 224, 0x30+(Device_code&0x000f), Black, Red);
       
	   //��5��������ʵ�ֻ���
           for(i=312;i>264;i--){ Put_pixel(68,i,Blue);}
	   for(i=68;i<172;i++) { Put_pixel(i,264,Blue);}
           for(i=264;i<312;i++){ Put_pixel(172,i,Blue);} 
	   for(i=172;i>68;i--) { Put_pixel(i,312,Blue);}
 
	   delayms(20000);  //��ʾһ��ʱ��
	   CLR_Screen(Red); //�ñ���ɫ����
	  }
 
  }



